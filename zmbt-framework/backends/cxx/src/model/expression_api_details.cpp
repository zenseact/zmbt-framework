
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/expression_api.hpp"

namespace zmbt {
namespace expr {
namespace detail {
    boost::json::value as_set(std::initializer_list<boost::json::value_ref> set)
    {
        boost::json::value val(set);
        if (set.size() != 1 || val.is_object())
        {
            return val;
        }
        else
        {
            return boost::json::array{val};
        }
    }
} // namespace detail
} // namespace expr

Expression operator|(Expression const& lhs, Expression const& rhs)
{
    boost::json::array new_params{};
    // unfold operator chain, reverse order
    if (lhs.is(Expression::Keyword::Compose))
    {
        auto const& old_params = lhs.params().as_array();
        new_params.reserve(old_params.size() + 1);
        new_params.push_back(rhs);
        for (auto const& fn: old_params)
        {
            new_params.push_back(fn);
        }
    }
    else
    {
        new_params = boost::json::array{rhs, lhs};
    }
    return Expression(Expression::Keyword::Compose, new_params);
}

Expression operator&(Expression const& lhs, Expression const& rhs)
{
    boost::json::array new_params{};
    // unfold operator chain, keep order
    if (lhs.is(Expression::Keyword::Pack))
    {
        new_params = lhs.params().as_array();
        new_params.push_back(rhs);
    }
    else
    {
        new_params.push_back(lhs);
        new_params.push_back(rhs);
    }
    return Expression(Expression::Keyword::Pack, new_params);
}

Expression operator+(Expression const& lhs, Expression const& rhs)
{
    boost::json::array new_params{};
    // unfold operator chain, keep order
    if (lhs.is(Expression::Keyword::Pack))
    {
        new_params = lhs.params().as_array();
        new_params.push_back(rhs);
    }
    else
    {
        new_params.push_back(lhs);
        new_params.push_back(rhs);
    }
    return Expression(Expression::Keyword::Concat, new_params);
}

Expression operator<<=(Expression const& lhs, Expression const& rhs)
{
    return api::Apply(lhs, rhs);
}

} // namespace zmbt

