/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_GENERIC_TRAITS_HPP_
#define ZMBT_CORE_GENERIC_TRAITS_HPP_

#include <type_traits>
#include <array>

#include "aliases.hpp"


namespace zmbt {


template <class R, class... C>
using first_if_t = mp_if<mp_all<C...>, R>;

template <class R, class... C>
using first_if_any_t = mp_if<mp_any<C...>, R>;

template <class R, class... C>
using first_if_none_t = mp_if<mp_not<mp_any<C...>>, R>;


template <class T>
using is_tuple = mp_or< mp_similar<tuple<>, T>, mp_similar<std::pair<void, void>, T> >;


template <class L>
using tuple_unqf_t = mp_transform<remove_cvref_t, L>;


/// Variable template that checks if a type has begin() and end() member functions
template <class, class = void>
struct is_const_iterable : std::false_type {};

template <class T>
struct is_const_iterable<T, void_t<decltype(std::declval<T>().cbegin()), decltype(std::declval<T>().cend())>> : std::true_type {};


namespace detail
{
template <class T, class L, class = void>
struct construct_or_default_impl;

template <class T, class L>
using is_tuple_constructible = mp_apply<is_constructible, mp_push_front<L, T>>;


template <class T, class L>
struct construct_or_default_impl<T, L, mp_if<is_tuple_constructible<T, L>,void>>
{
    static T construct(mp_apply<std::tuple, L> const & args)
    {
        return construct_from_tuple<T>(args);
    }
};

template <class T, class L>
struct construct_or_default_impl<T, L, mp_if<mp_not<is_tuple_constructible<T, L>>,void>>
{
    static T construct(mp_apply<std::tuple, L> const & args)
    {
        return T{};
    }
};
} // namespace detail

/// Construct T from the given arguments or return default T if construction is invalid
template <class T, class... A>
T construct_or_default(A&&... args)
{
    return detail::construct_or_default_impl<T, mp_list<A...>>::construct(std::forward_as_tuple(args...));
}

} // namespace zmbt

#endif // ZMBT_CORE_GENERIC_TRAITS_HPP_
