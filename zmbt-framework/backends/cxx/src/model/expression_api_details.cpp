
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/expression_api.hpp"

namespace zmbt {

namespace detail
{
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
}

Expression operator|(Expression const& lhs, Expression const& rhs)
{
    return api::Compose(rhs, lhs);
}

Expression operator<<=(Expression const& lhs, boost::json::value const& rhs)
{
    return api::Apply(lhs, rhs);
}

} // namespace zmbt

