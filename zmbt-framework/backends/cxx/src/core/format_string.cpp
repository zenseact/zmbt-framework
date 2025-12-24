/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/format.hpp>
#include <boost/json.hpp>
#include <zmbt/core/format_string.hpp>
#include <string>

std::string
zmbt::detail::format_impl(boost::json::string_view fmtstr, boost::json::array const& args)
{
    boost::format fmt {fmtstr};
    for (auto const& a: args)
    {
        if (a.is_string())
        {
            fmt = fmt % a.as_string().c_str();
        }
        else
        {
            fmt = fmt % a;
        }
    }
    return fmt.str().c_str();
}
