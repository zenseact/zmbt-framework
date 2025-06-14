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

#include <boost/json.hpp>
#include <zmbt/core/exceptions.hpp>
#include <zmbt/core/type_info.hpp>

#include "zmbt/expr/generic_signal_operator.hpp"
#include "zmbt/expr/signal_operator_handler.hpp"


namespace
{
} // namespace


namespace zmbt {


bool SignalOperatorHandler::exchangeHandle(Handle& handle, bool const retrieve)
{
    using Table = std::map<boost::json::string, Handle>;

    static std::mutex ctor_mt;
    std::lock_guard<std::mutex> guard(ctor_mt);

    static std::shared_ptr<Table> table;

    if (not table)
    {
        table = std::make_shared<Table>();
    }

    auto const found = table->find(handle.annotation);

    if (!retrieve && found == table->cend())
    {
        table->emplace_hint(found, handle.annotation, handle);
        return true;
    }
    else if (retrieve && found != table->cend())
    {
        handle = found->second;
        return true;
    }
    else
    {
        return found != table->cend();
    }
}

SignalOperatorHandler::SignalOperatorHandler() : SignalOperatorHandler{Handle{}}
{
}

SignalOperatorHandler::SignalOperatorHandler(Handle const handle) : handle_{handle}
{
    exchangeHandle(handle_, false);
}

SignalOperatorHandler::SignalOperatorHandler(boost::json::string_view annotation)
    : handle_{}
{
    handle_.annotation = annotation;
    if (!exchangeHandle(handle_, true))
    {
        throw expression_error("`%s` operator not found", annotation);
    }
}


boost::json::value SignalOperatorHandler::apply(dsl::Keyword const& keyword, boost::json::value const& lhs, boost::json::value const& rhs) const
{
    switch (keyword)
    {
    case dsl::Keyword::Bool: return handle_.logic.bool_(rhs);
    case dsl::Keyword::Not: return !handle_.logic.bool_(rhs);
    case dsl::Keyword::And: return handle_.logic.and_(lhs, rhs);
    case dsl::Keyword::Or: return handle_.logic.or_(lhs, rhs);

    case dsl::Keyword::Eq: return handle_.comp.equal_to(lhs, rhs);
    case dsl::Keyword::Ne: return !handle_.comp.equal_to(lhs, rhs);

    case dsl::Keyword::Le: return handle_.comp.less_equal(lhs, rhs);
    case dsl::Keyword::Gt: return !handle_.comp.less_equal(lhs, rhs);
    case dsl::Keyword::Ge: return handle_.comp.less_equal(rhs, lhs);
    case dsl::Keyword::Lt: return !handle_.comp.less_equal(rhs, lhs);

    case dsl::Keyword::Add: return handle_.arithmetics.add(lhs, rhs);
    case dsl::Keyword::Sub: return handle_.arithmetics.sub(lhs, rhs);
    case dsl::Keyword::Mul: return handle_.arithmetics.mul(lhs, rhs);
    case dsl::Keyword::Div: return handle_.arithmetics.div(lhs, rhs);
    case dsl::Keyword::Mod: return handle_.arithmetics.mod(lhs, rhs);
    case dsl::Keyword::Neg   : return handle_.arithmetics.neg(rhs);

    case dsl::Keyword::BitNot: return handle_.bitwise.compl_(rhs);
    case dsl::Keyword::BitAnd: return handle_.bitwise.and_(lhs, rhs);
    case dsl::Keyword::BitOr : return handle_.bitwise.or_(lhs, rhs);
    case dsl::Keyword::BitXor: return handle_.bitwise.xor_(lhs, rhs);

    case dsl::Keyword::BitLshift: return handle_.shift.left(lhs, rhs);
    case dsl::Keyword::BitRshift: return handle_.shift.right(lhs, rhs);

    case dsl::Keyword::SetEq: return is_subset(lhs, rhs) && is_subset(rhs, lhs); // TODO: optimize
    case dsl::Keyword::Subset: return is_subset(lhs, rhs);
    case dsl::Keyword::Superset: return is_subset(rhs, lhs);
    case dsl::Keyword::ProperSubset  : return is_subset(lhs, rhs) && !is_subset(rhs, lhs); // TODO: optimize
    case dsl::Keyword::ProperSuperset: return is_subset(rhs, lhs) && !is_subset(lhs, rhs); // TODO: optimize

    case dsl::Keyword::In: return contains(rhs, lhs);
    case dsl::Keyword::Ni: return contains(lhs, rhs);
    case dsl::Keyword::NotIn: return !contains(rhs, lhs);
    case dsl::Keyword::NotNi: return !contains(lhs, rhs);
    case dsl::Keyword::Approx: return is_approx(lhs, rhs);

    case dsl::Keyword::Pow:      return GenericSignalOperator(lhs).pow(rhs);
    case dsl::Keyword::Log:      return GenericSignalOperator(lhs).log(rhs);
    case dsl::Keyword::Quot:     return GenericSignalOperator(lhs).quot(rhs);

    default:
        throw expression_not_implemented("unsupported operator");
        return nullptr;
    }
}




bool SignalOperatorHandler::is_approx(boost::json::value const& sample, boost::json::value const& expr) const
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))

    // TODO: handle invalid expr
    auto arr = expr.as_array();
    double reference = arr.at(0).as_double();
    double rtol = arr.at(1).as_double();
    double atol = arr.size() == 3 ? arr.at(2).as_double() : std::numeric_limits<double>::epsilon();
    double value = boost::json::value_to<double>(sample);

    return std::abs(value - reference) <= (atol + rtol * std::abs(reference));
}

/// Is subset of
bool SignalOperatorHandler::is_subset(boost::json::value const& lhs, boost::json::value const& rhs) const
{
    if (lhs.is_array() && rhs.is_array())
    {
        boost::json::array const& a = lhs.get_array();
        boost::json::array const& b = rhs.get_array();

        for (auto const& value: a)
        {
            if (std::find_if(b.cbegin(), b.cend(), [&](auto const& other){ return handle_.comp.equal_to(value, other); }) == b.cend())
            {
                return false;
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
    else
    {
        throw expression_error("undefined operation: `%s ⊆ %s`", lhs , rhs);
    }
    return false;
}


/// Is element of
bool SignalOperatorHandler::contains(boost::json::value const& set, boost::json::value const& element) const
{
    // array item
    if (set.if_array())
    {
        return is_subset(boost::json::array{element}, set);
    }
    else if(element.is_string() && set.is_string())
    {
        return is_subset(element, set);
    }
    // object key
    else if(element.is_string() && set.is_object())
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
    else
    {
        throw expression_error("undefined operation: `%s ∈ %s`", element, set);
    }
    return false;
}


} // namespace zmbt
