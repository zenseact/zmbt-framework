/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_INSTANTIATION_ASSERT_HPP_
#define ZMBT_CORE_INSTANTIATION_ASSERT_HPP_

#include "aliases.hpp"
#include "generic_traits.hpp"

namespace zmbt {



/**
 * @brief Replace the static_assert to provide introspection info on compilation errors
 *
 * @tparam condition
 * @tparam T optional types list to include in error message
 */
template<bool condition, class... T>
struct instantiation_assert {
    constexpr instantiation_assert() = default;
    constexpr explicit instantiation_assert(const char*) {};
};

template<class... T>
struct instantiation_assert<false, T...>;

}

#endif
