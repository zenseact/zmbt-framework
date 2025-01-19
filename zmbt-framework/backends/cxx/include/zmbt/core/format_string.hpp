/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_FORMAT_STRING_HPP_
#define ZMBT_CORE_FORMAT_STRING_HPP_


#include <boost/json.hpp>
#include <string>
#include <utility>

namespace zmbt {


namespace detail
{
    std::string format_impl(boost::json::string_view fmtstr, boost::json::array const& args);
}

/// boost::format wrapper with printf-like api
template <class... A>
std::string format(boost::json::string_view fmtstr, A&&... arg)
{
    return detail::format_impl(fmtstr, {std::forward<A>(arg)...});
}



}  // namespace zmbt

#endif
