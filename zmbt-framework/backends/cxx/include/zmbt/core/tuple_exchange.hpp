/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_TUPLE_EXCHANGE_HPP_
#define ZMBT_CORE_TUPLE_EXCHANGE_HPP_

#include <tuple>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail {

struct tuple_exchange_helper
{
    template <class T>
    using is_assignable_reference = mp_and<std::is_lvalue_reference<T>, mp_not<std::is_const<T>>>;

    template <class A, class B>
    static void assign(A & a, B const& b)
    {
        a = b;
    }

    template <class A, class B>
    static void assign(A const&, B const&) {}
};

}


template <class T1, class T2>
void tuple_exchange(T1 & target, T2 const& src)
{
    static_assert(mp_same<mp_size<T1>, mp_size<T2>>::value, "tuples shall be of equal size");

    using helper = detail::tuple_exchange_helper;
    mp_for_each<mp_iota<mp_size<T1>>>([&](auto N){
        using A = mp_at_c<T1, N>;
        if (helper::is_assignable_reference<A>::value) {
            helper::assign(
                std::forward<A>(std::get<N>(target)),
                std::get<N>(src)
            );
        }
    });
}

}  // namespace zmbt

#endif  // ZMBT_CORE_TUPLE_CONVERSION_HPP_
