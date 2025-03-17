
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/expression_api.hpp"

namespace
{
    bool maybe_kv_pair(boost::json::value const& val)
    {
        if (!val.is_array())
        {
            return false;
        }
        auto const& pair = val.get_array();
        if (pair.size() != 2)
        {
            return false;
        }
        return pair.at(0).is_string();
    }

    bool maybe_object(boost::json::array const& value_list)
    {
        return value_list.cend() == std::find_if_not(value_list.cbegin(), value_list.cend(), maybe_kv_pair);
    }
} // namespace


namespace zmbt {
namespace dsl {
namespace detail {
boost::json::value handle_list_init(std::initializer_list<Expression> set)
{
    auto const value_list = boost::json::value_from(set).as_array();

    if (maybe_object(value_list))
    {
        boost::json::object out{};
        out.reserve(value_list.size());
        for (auto const& pair: value_list)
        {
            auto const& kv = pair.get_array();
            out.insert_or_assign(kv.at(0).as_string(), kv.at(1));
        }
        return out;
    }
    else
    {
        return value_list;
    }
}
} // namespace detail
} // namespace dsl

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
    return expr::Apply(lhs, rhs);
}

} // namespace zmbt

