/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <cstdint>
#include <boost/json.hpp>

#include "zmbt/core/json_number_cast.hpp"


namespace zmbt
{

boost::json::value real_to_number(double value_double)
{
    std::int64_t const value_int64 = static_cast<std::int64_t>(value_double);
    std::int64_t const value_uint64 = static_cast<std::size_t>(value_double);

    if (value_double == value_uint64)
    {
        return value_uint64;
    }
    else if (value_double == value_int64)
    {
        return value_int64;
    }
    else {
        return value_double;
    }
}

boost::json::value real_to_number(std::int64_t value)
{
    if (value >= 0)
    {
        return static_cast<std::uint64_t>(value);
    }
    else {
        return value;
    }
}

} // namespace zmbt
