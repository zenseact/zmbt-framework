/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_JSON_NUMBER_CAST_HPP_
#define ZMBT_CORE_JSON_NUMBER_CAST_HPP_

#include <boost/json.hpp>

namespace zmbt
{

/// Convert a double to a JSON number,
/// storing result as uint64_t or int64_t if possible.
boost::json::value real_to_number(double value);

/// Convert a signed integer to JSON number,
/// storing result as uint64_t if possible.
boost::json::value real_to_number(std::int64_t value);

boost::json::value maybe_real_to_number(boost::json::value const& value);

/// Convert a signed integer to JSON number in place,
/// storing result as uint64_t if possible.
bool maybe_real_to_number_inplace(boost::json::value& value);


} // namespace zmbt

#endif
