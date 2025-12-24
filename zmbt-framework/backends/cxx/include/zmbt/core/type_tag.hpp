/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_TYPE_TAG_HPP_
#define ZMBT_CORE_TYPE_TAG_HPP_

#include "aliases.hpp"

namespace zmbt {

/// Type tag to pass template parameters as function arguments
template <class T>
struct type_tag {
    using type = T;
};

/**
 * @brief type tag value
 *
 * @tparam T
 */
template <class T = void>
constexpr type_tag<T> const type{};

template <class T>
using is_type_tag = mp_similar<decay_t<T>, type_tag<void>>;



}  // namespace zmbt

#endif  // ZMBT_CORE_TYPE_TAG_HPP_
