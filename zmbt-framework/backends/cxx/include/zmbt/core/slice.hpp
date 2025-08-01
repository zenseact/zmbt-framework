/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_SLICE_ARRAY_HPP_
#define ZMBT_MODEL_SLICE_ARRAY_HPP_

#include <array>
#include <functional>
#include "aliases.hpp"

namespace zmbt {

/// @brief Slice array with inclusive boundaries and subsignal cherry-pick
/// @param src source array
/// @param jp JSON Pointer for subsignal cherry-pick
/// @param start start index, 0-based, inclusive; negative value yields reverse index;
/// @param stop stop index, 0-based, inclusive; negative value yields reverse index;
/// @param step step value, negative value yields reverse iteration
/// @return slice array
boost::json::array slice(boost::json::array const& src, boost::json::string_view jp, std::int64_t const start, std::int64_t const stop = -1, std::int64_t const step = 1);

/// @brief Slice array with inclusive boundaries
/// @param src source array
/// @param start start index, 0-based, inclusive; negative value yields reverse index;
/// @param stop stop index, 0-based, inclusive; negative value yields reverse index;
/// @param step step value, negative value yields reverse iteration
/// @return slice array
boost::json::array slice(boost::json::array const& src, std::int64_t const start, std::int64_t const stop = -1, std::int64_t const step = 1);

boost::json::string slice(boost::json::string_view const src, std::int64_t const start, std::int64_t const stop = -1, std::int64_t const step = 1);

using js_array_slice_gen = std::function<boost::json::array::iterator()>;
using js_array_slice_const_gen = std::function<boost::json::array::const_iterator()>;
using js_string_slice_gen = std::function<boost::json::string_view::const_iterator()>;

js_array_slice_gen make_slice_generator(boost::json::array& src, std::int64_t const start, std::int64_t const stop = -1, std::int64_t const step = 1);
js_array_slice_const_gen make_slice_const_generator(boost::json::array const& src, std::int64_t const start = 0, std::int64_t const stop = -1, std::int64_t const step = 1);
js_string_slice_gen make_slice_const_generator(boost::json::string_view const src, std::int64_t const start = 0, std::int64_t const stop = -1, std::int64_t const step = 1);


namespace detail
{
    std::array<std::int64_t, 3> str_to_slice_idx(boost::json::string_view slice_expr);
} // namespace detail
} // namespace zmbt

#endif