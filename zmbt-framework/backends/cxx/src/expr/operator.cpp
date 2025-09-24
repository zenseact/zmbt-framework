/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <mutex>
#include <memory>
#include <string>

#include <boost/current_function.hpp>
#include <boost/json.hpp>
#include <zmbt/core/exceptions.hpp>
#include <zmbt/core/type_info.hpp>
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK
#include <boost/unordered/concurrent_flat_map.hpp>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/api.hpp"



namespace zmbt {
namespace lang {
namespace detail
{
boost::json::value make_error_expr(boost::json::string_view msg, boost::json::string_view ctx)
{
    return expr::Err(msg, ctx).to_json();
}

} // namespace detail


bool Operator::exchangeHandle(Handle& handle, bool const retrieve)
{
    using Table = boost::concurrent_flat_map<boost::json::string, Handle>;

    static std::shared_ptr<Table> table = std::make_shared<Table>();

    if (!retrieve)
    {
        return table->emplace(handle.annotation, handle);
    }
    else
    {
        return 1 == table->visit(handle.annotation, [&handle](auto& rec){
            handle = rec.second;
        });
    }
}

Operator::Operator() : Operator{Handle{}}
{
}

Operator::Operator(Handle const handle) : handle_{handle}
{
    exchangeHandle(handle_, false);
}

Operator::Operator(boost::json::string_view annotation)
    : handle_{}
{
    handle_.annotation = annotation;
    if (!exchangeHandle(handle_, true))
    {
        throw_exception(operator_error("`%s` operator not found", annotation));
    }
}


boost::json::value Operator::apply(Keyword const& keyword, LV lhs, LV rhs) const
try
{
    auto const negate = [](V const& maybe_err) -> V
    {
        if (not maybe_err.is_bool()) return maybe_err;
        return not maybe_err.get_bool();
    };

    auto const conj = [](V const& maybe_err_a, V const& maybe_err_b) -> V
    {
        if (not maybe_err_a.is_bool()) return maybe_err_a;
        if (not maybe_err_b.is_bool()) return maybe_err_b;
        return maybe_err_a.get_bool() && maybe_err_b.get_bool();
    };



    switch (keyword)
    {
    case Keyword::Bool: return handle_.logic.bool_(lhs);
    case Keyword::Not: return negate(handle_.logic.bool_(lhs));
    case Keyword::And: return handle_.logic.and_(lhs, rhs);
    case Keyword::Or: return handle_.logic.or_(lhs, rhs);

    case Keyword::Eq: return handle_.comp.equal_to(lhs, rhs);
    case Keyword::Ne: return negate(handle_.comp.equal_to(lhs, rhs));

    case Keyword::Le: return handle_.comp.less_equal(lhs, rhs);
    case Keyword::Gt: return negate(handle_.comp.less_equal(lhs, rhs));
    case Keyword::Ge: return handle_.comp.less_equal(rhs, lhs);
    case Keyword::Lt: return negate(handle_.comp.less_equal(rhs, lhs));

    case Keyword::Add: return handle_.arithmetics.add(lhs, rhs);
    case Keyword::Sub: return handle_.arithmetics.sub(lhs, rhs);
    case Keyword::Mul: return handle_.arithmetics.mul(lhs, rhs);
    case Keyword::Div: return handle_.arithmetics.div(lhs, rhs);
    case Keyword::Mod: return handle_.arithmetics.mod(lhs, rhs);
    case Keyword::Neg   : return handle_.arithmetics.neg(lhs);

    case Keyword::BitNot: return handle_.bitwise.compl_(lhs);
    case Keyword::BitAnd: return handle_.bitwise.and_(lhs, rhs);
    case Keyword::BitOr : return handle_.bitwise.or_(lhs, rhs);
    case Keyword::BitXor: return handle_.bitwise.xor_(lhs, rhs);

    case Keyword::Lshift: return handle_.shift.left(lhs, rhs);
    case Keyword::Rshift: return handle_.shift.right(lhs, rhs);

    case Keyword::SetEq: return conj(is_subset(lhs, rhs), is_subset(rhs, lhs)); // TODO: optimize
    case Keyword::Subset: return is_subset(lhs, rhs);
    case Keyword::Superset: return is_subset(rhs, lhs);
    case Keyword::PSubset  : return conj(is_subset(lhs, rhs), negate(is_subset(rhs, lhs))); // TODO: optimize
    case Keyword::PSuperset: return conj(is_subset(rhs, lhs), negate(is_subset(lhs, rhs))); // TODO: optimize

    case Keyword::In: return contains(rhs, lhs);
    case Keyword::Ni: return contains(lhs, rhs);
    case Keyword::NotIn: return negate(contains(rhs, lhs));
    case Keyword::NotNi: return negate(contains(lhs, rhs));

    case Keyword::Pow:      return Operator::generic_pow(lhs, rhs);
    case Keyword::Log:      return Operator::generic_log(lhs, rhs);
    case Keyword::Quot:     return Operator::generic_quot(lhs, rhs);

    default:
        return detail::make_error_expr("unsupported operator", BOOST_CURRENT_FUNCTION);
    }
}
catch(const std::exception& e)
{
    return detail::make_error_expr(e.what(), annotation());
}

/// Is subset of
boost::json::value Operator::is_subset(LV const& llhs, LV const& lrhs) const
{
    auto const lhs = llhs();
    auto const rhs = lrhs();
    if (lhs.is_array() && rhs.is_array())
    {
        boost::json::array const& a = lhs.get_array();
        boost::json::array const& b = rhs.get_array();

        for (auto const& item_a: a)
        {
            try
            {
                if (std::find_if(
                    b.cbegin(),
                    b.cend(),
                    [&](auto const& item_b){ return handle_.comp.equal_to(item_a, item_b).as_bool(); }) == b.cend()
                )
                {
                    return false;
                }
            }
            catch(const std::exception& e)
            {
                return detail::make_error_expr("invalid operands", BOOST_CURRENT_FUNCTION);
            }
        }
        return true;
    }
    else if (lhs.is_object() && rhs.is_object())
    {
        boost::json::object const& a = lhs.get_object();
        boost::json::object const& b = rhs.get_object();

        // valid optimisation as objects have no duplicates
        if (a.size() > b.size())
        {
            return false;
        }

        for (auto const& kvp: a)
        {
            auto const& key = kvp.key();
            auto const& value = kvp.value();
            if (!b.contains(key) || value != b.at(key))
            {
                return false;
            }
        }
        return true;
    }
    // string in string treated as set of substrings
    else if (lhs.is_string() && rhs.is_string())
    {
        if (lhs.get_string().empty()) { return true; }
        return std::string::npos != rhs.get_string().find(lhs.get_string());
    }
    return detail::make_error_expr("invalid operands", BOOST_CURRENT_FUNCTION);
}


/// Is element of
boost::json::value Operator::contains(LV const& lset, LV const& lelement) const
{

    // array item
    if (lset().if_array())
    {
        return is_subset(boost::json::array{lelement()}, lset);
    }
    else if(lelement().is_string() && lset().is_string())
    {
        return is_subset(lelement, lset);
    }

    auto const& set = lset();
    auto const& element = lelement();

    // object key
    if(element.is_string() && set.is_object())
    {
        return set.get_object().contains(element.get_string());
    }
    // object key-value pair
    else if(set.is_object() && element.is_array() && element.get_array().size() == 2 && element.get_array().front().is_string())
    {
        auto const& kvp = element.get_array();
        auto const& obj = set.get_object();
        auto const& key = kvp.front().as_string();
        auto const& value = kvp.at(1);
        if (!obj.contains(key)) { return false; }
        return handle_.comp.equal_to(obj.at(key), value);
    }
    return detail::make_error_expr("invalid operands", BOOST_CURRENT_FUNCTION);
}


} // namespace lang
} // namespace zmbt
