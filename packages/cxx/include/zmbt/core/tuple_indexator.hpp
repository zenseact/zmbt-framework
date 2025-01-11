/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_TUPLE_INDEXATOR_HPP_
#define ZMBT_CORE_TUPLE_INDEXATOR_HPP_

#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail
{

template <class I>
struct tuple_indexator;

template <size_t... I>
struct tuple_indexator<std::index_sequence<I...>> {
    using type = tuple<std::integral_constant<size_t, I>...>;
    using sequence = std::index_sequence<I...>;
};
template <class... A>
struct tuple_indexator<tuple<A...>> {
    using type = typename tuple_indexator<std::make_index_sequence<sizeof...(A)>>::type;
    using sequence = typename tuple_indexator<std::make_index_sequence<sizeof...(A)>>::sequence;
};

} // namespace detail

/**
 * @brief Make indexator tuple for T
 *
 * @details Create std::tuple of std::integral_constant with 0, ..., N-1 values for input tuple of size N
 * @tparam T input tuple
 */
template <class T>
using  indexator_for = typename detail::tuple_indexator<T>::type;



}  // namespace zmbt

#endif
