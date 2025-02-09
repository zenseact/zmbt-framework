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


#define GENERIC_T1(OP) [](boost::json::value const& rhs) { return OP zmbt::GenericSignalOperator(rhs); }
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


boost::json::value SignalOperatorHandler::apply(expr::Keyword const& keyword, boost::json::value const& lhs, boost::json::value const& rhs) const
{
    switch (keyword)
    {
    case expr::Keyword::Bool: return operators.is_truth_(rhs);
    case expr::Keyword::Nil: return !operators.is_truth_(rhs);
    case expr::Keyword::Not: return operators.not_(rhs);
    case expr::Keyword::And: return operators.land_(lhs, rhs);
    case expr::Keyword::Or: return operators.lor_(lhs, rhs);

    case expr::Keyword::Eq: return operators.is_equal_(lhs, rhs);
    case expr::Keyword::Ne: return !operators.is_equal_(lhs, rhs);

    case expr::Keyword::Le: return operators.is_le_(lhs, rhs);
    case expr::Keyword::Gt: return !operators.is_le_(lhs, rhs);
    case expr::Keyword::Ge: return operators.is_le_(rhs, lhs);
    case expr::Keyword::Lt: return !operators.is_le_(rhs, lhs);

    case expr::Keyword::Add: return operators.add_(lhs, rhs);
    case expr::Keyword::Sub: return operators.sub_(lhs, rhs);
    case expr::Keyword::Mul: return operators.mul_(lhs, rhs);
    case expr::Keyword::Div: return operators.div_(lhs, rhs);
    case expr::Keyword::Mod: return operators.mod_(lhs, rhs);

    case expr::Keyword::Neg   : return operators.neg_(rhs);
    case expr::Keyword::BitNot: return operators.compl_(rhs);
    case expr::Keyword::BitAnd: return operators.conj_(lhs, rhs);
    case expr::Keyword::BitOr : return operators.disj_(lhs, rhs);
    case expr::Keyword::BitXor: return operators.bxor_(lhs, rhs);

    // case expr::Keyword::BitLshift: return operators TODO
    // case expr::Keyword::BitRshift: return operators TODO

    case expr::Keyword::SetEq: return is_subset(lhs, rhs) && is_subset(rhs, lhs); // TODO: optimize
    case expr::Keyword::Subset: return is_subset(lhs, rhs);
    case expr::Keyword::Superset: return is_subset(rhs, lhs);
    case expr::Keyword::ProperSubset  : return is_subset(lhs, rhs) && !is_subset(rhs, lhs); // TODO: optimize
    case expr::Keyword::ProperSuperset: return is_subset(rhs, lhs) && !is_subset(lhs, rhs); // TODO: optimize

    case expr::Keyword::In: return contains(rhs, lhs);
    case expr::Keyword::Ni: return contains(lhs, rhs);
    case expr::Keyword::NotIn: return !contains(rhs, lhs);
    case expr::Keyword::NotNi: return !contains(lhs, rhs);
    case expr::Keyword::Approx: return is_approx(lhs, rhs);

    case expr::Keyword::Pow:
    {
        return GenericSignalOperator(lhs).pow(rhs);
    }
    // case expr::Keyword::Log: return TODO
    // case expr::Keyword::Quot: return TODO

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
