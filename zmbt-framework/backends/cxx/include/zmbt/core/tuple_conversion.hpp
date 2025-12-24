/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_TUPLE_CONVERSION_HPP_
#define ZMBT_CORE_TUPLE_CONVERSION_HPP_

#include <tuple>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail {

template <class T, class... A>
struct tuple_conversion_handler {

    T target;
    tuple_conversion_handler(tuple<A...>& source) : tuple_conversion_handler{source, std::make_index_sequence<sizeof...(A)>()}
    {
    }

  private:

    template <size_t... I>
    tuple_conversion_handler(tuple<A...>& source, std::index_sequence<I...>)
    // TODO: refactor with static_cast
    : target{std::get<I>(source)...}
    {
    }
};

}  // namespace detail


/**
 * @brief Convert given tuple to a tuple of type T
 *
 * @tparam T target tuple type
 * @tparam A source tuple parameters
 * @param source input tuple
 * @return T
 */
template <class T, class... A>
inline T convert_tuple_to(tuple<A...> &source) {
    return detail::tuple_conversion_handler<T, A...> (source).target;
}

}  // namespace zmbt

#endif  // ZMBT_CORE_TUPLE_CONVERSION_HPP_
