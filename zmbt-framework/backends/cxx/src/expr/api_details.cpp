
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/api.hpp"


namespace zmbt {
namespace lang {

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
    if (lhs.is(Expression::Keyword::Fork))
    {
        new_params = lhs.params().as_array();
        new_params.push_back(rhs);
    }
    else
    {
        new_params.push_back(lhs);
        new_params.push_back(rhs);
    }
    return Expression(Expression::Keyword::Fork, new_params);
}

Expression operator<<(Expression const& lhs, Expression const& rhs)
{
    return expr::Compose(lhs, rhs);
}

boost::json::value operator*(Expression const& lhs, Expression const& rhs)
{
    return lhs.eval(rhs);
}

boost::json::value operator*(Expression const& expr)
{
    return expr.eval();
}

} // namespace lang
} // namespace zmbt

