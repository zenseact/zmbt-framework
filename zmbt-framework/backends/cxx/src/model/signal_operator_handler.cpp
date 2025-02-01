/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>
#include <zmbt/core/exceptions.hpp>
#include <zmbt/core/type_info.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>

#include "zmbt/model/generic_signal_operator.hpp"
#include "zmbt/model/signal_operator_handler.hpp"


namespace zmbt {


SignalOperatorHandler::SignalOperatorHandler(
    boost::json::string_view  annotation,
    operators_t op
)
    : annotation_{annotation}
    , operators{op}
{
}


#define GENERIC_T1(OP) [](boost::json::value const& lhs) { return OP zmbt::GenericSignalOperator(lhs); }
#define GENERIC_T2(OP) [](boost::json::value const& lhs, boost::json::value const& rhs) { return zmbt::GenericSignalOperator(lhs) OP zmbt::GenericSignalOperator(rhs); }

SignalOperatorHandler::SignalOperatorHandler()
    : SignalOperatorHandler{
        type_name<GenericSignalOperator>(),
        operators_t {
            [](boost::json::value const& x) {return x;},
            [](boost::json::value const& x) {return static_cast<bool>(zmbt::GenericSignalOperator(x));},
            GENERIC_T2(==), // generic_is_equal,
            GENERIC_T2(<), // generic_is_less,
            GENERIC_T2(<=), // generic_is_less_or_eq,
            GENERIC_T1(-), // generic_negate,
            GENERIC_T1(~), // generic_complement,
            GENERIC_T1(!), // generic_logical_not,
            GENERIC_T2(+), // generic_add,
            GENERIC_T2(-), // generic_sub,
            GENERIC_T2(*), // generic_mul,
            GENERIC_T2(/), // generic_div,
            GENERIC_T2(%), // generic_mod,
            GENERIC_T2(&), // generic_band,
            GENERIC_T2(|), // generic_bor,
            GENERIC_T2(^), // generic_bxor,
            GENERIC_T2(&&), // generic_land,
            GENERIC_T2(||)  // generic_lor,
        }
    }
{
}

#undef GENERIC_T1
#undef GENERIC_T2


boost::json::value SignalOperatorHandler::apply(Keyword const& keyword, boost::json::value const& x, boost::json::value const& y) const
{
    switch (keyword)
    {
    case Keyword::Bool: return operators.is_truth_(x);
    case Keyword::Nil: return !operators.is_truth_(x);
    case Keyword::Not: return operators.not_(x);
    case Keyword::And: return operators.land_(x, y);
    case Keyword::Or: return operators.lor_(x, y);

    case Keyword::Eq: return operators.is_equal_(x, y);
    case Keyword::Ne: return !operators.is_equal_(x, y);

    case Keyword::Le: return operators.is_le_(x, y);
    case Keyword::Gt: return !operators.is_le_(x, y);
    case Keyword::Ge: return operators.is_le_(y, x);
    case Keyword::Lt: return !operators.is_le_(y, x);

    case Keyword::Add: return operators.add_(x, y);
    case Keyword::Sub: return operators.sub_(x, y);
    case Keyword::Mul: return operators.mul_(x, y);
    case Keyword::Div: return operators.div_(x, y);
    case Keyword::Mod: return operators.mod_(x, y);

    case Keyword::Neg   : return operators.neg_(x);
    case Keyword::BitNot: return operators.compl_(x);
    case Keyword::BitAnd: return operators.conj_(x, y);
    case Keyword::BitOr : return operators.disj_(x, y);
    case Keyword::BitXor: return operators.bxor_(x, y);

    // case Keyword::BitLshift: return operators TODO
    // case Keyword::BitRshift: return operators TODO

    case Keyword::SetEq: return is_subset(x, y) && is_subset(y, x); // TODO: optimize
    case Keyword::Subset: return is_subset(x, y);
    case Keyword::Superset: return is_subset(y, x);
    case Keyword::ProperSubset  : return is_subset(x, y) && !is_subset(y, x); // TODO: optimize
    case Keyword::ProperSuperset: return is_subset(y, x) && !is_subset(x, y); // TODO: optimize

    case Keyword::In: return contains(y, x);
    case Keyword::Ni: return contains(x, y);
    case Keyword::NotIn: return !contains(y, x);
    case Keyword::NotNi: return !contains(x, y);
    case Keyword::Approx: return is_approx(x, y);

    case Keyword::Pow:
    {
        return GenericSignalOperator(x).pow(y);
    }
    // case Keyword::Log: return TODO
    // case Keyword::Quot: return TODO

    default:
        throw expression_error("invalid operand");
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
            if (!value.is_structured() && std::find_if(b.cbegin(), b.cend(), [&](auto const& other){ return operators.is_equal_(value, other); }) == b.cend())
            {
                return false;
            }
            else if (value.is_structured() && std::find_if(b.cbegin(), b.cend(), [&](auto const& other){ return is_subset(value, other); }) == b.cend())
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
            if (!b.contains(kvp.key())) {
                return false;
            }
            else if (value.is_structured() && !is_subset(value, b.at(key)))
            {
                return false;
            }
            else if (!value.is_structured() && value != b.at(key))
            {
                return false;
            }
        }
        return true;
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
    if (auto arr = set.if_array())
    {
        return std::find_if(arr->cbegin(), arr->cend(), [&](auto const& other){ return operators.is_equal_(element, other); }) != arr->cend();
    }
    // object key
    else if(element.is_string() && set.if_object())
    {
        return set.get_object().contains(element.as_string());
    }
    // object key-value pair
    else if(element.is_array() && set.is_object())
    {
        auto const& kvp = element.get_array();
        if (kvp.size() != 2) { return false; }
        if (!kvp.at(0).is_string()) { return false; }

        auto const& obj = set.get_object();
        auto const& key = kvp.at(0).as_string();
        auto const& value = kvp.at(1);
        if (!obj.contains(key)) { return false; }
        return operators.is_equal_(obj.at(key), value);
    }
    else
    {
        throw expression_error("undefined operation: `%s ∈ %s`", element, set);
    }
    return false;
}


} // namespace zmbt
