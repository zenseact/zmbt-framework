/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_REFLECT_INIT_POLICY_HPP_
#define ZMBT_REFLECT_INIT_POLICY_HPP_

#include <functional>
#include <type_traits>

#include "zmbt/core.hpp"

// TODO: move to common header
#define ZMBT_PP_UNPACK(...) __VA_ARGS__

#define ZMBT_DEFINE_CUSTOM_INIT(Type, Args) \
template<> struct zmbt::reflect::custom_initialization<Type> { \
    static Type init() { return Type(ZMBT_PP_UNPACK Args); } \
};

namespace zmbt {
namespace reflect {

/**
 * @brief Template policy to handle customized object initialization
 *
 * @tparam T
 * @tparam E
 */
template <class T, class E = void>
struct initialization;

template <class T, class E = void>
struct custom_initialization;

namespace detail {


template <class T, class E = void>
struct default_initialization;

template<class T>
using default_initialization_t = decltype(default_initialization<T>::init());

template<class T>
using has_default_initialization = mp_valid<default_initialization_t, T>;

template<class T>
using custom_initialization_t = decltype(custom_initialization<T>::init());

template<class T>
using has_custom_initialization = mp_valid<custom_initialization_t, T>;


template <class T>
using enable_default_initialization = mp_if<mp_not<has_custom_initialization<T>>, void>;

template <class T>
using enable_custom_initialization = mp_if<has_custom_initialization<T>, void>;

template <class T>
using enable_initialization_trap = first_if_none_t<void, has_custom_initialization<T>, has_default_initialization<T>>;


template <class T>
struct default_initialization<T, first_if_t<void, is_default_constructible<T>>>
{
    static constexpr T
    init ()
    {
        return T{};
    }
};


template <template <class...> class C, class... T>
struct default_initialization< C<T...>, first_if_none_t<void, is_default_constructible<C<T...>>>>
{
    static inline C<T...>
    init() {
        return C<T...> {initialization<T>::init()...};
    }
};



template <class T, std::size_t N>
struct array_initializer {
    using tuple_form = boost::mp11::mp_repeat_c<std::tuple<T>, N>;
    std::array<T, N> value;

    array_initializer() : array_initializer(
        initialization<tuple_form>::init(),
        std::make_index_sequence<N>()) {}

    private:
    template <std::size_t... I>
    array_initializer (tuple_form tuple, std::index_sequence<I...>)
    : value{std::get<I>(tuple)...} {}
};


template <class T, std::size_t N>
struct default_initialization<std::array<T, N>, first_if_none_t<void, is_default_constructible<std::array<T, N>>>>
{
    static inline std::array<T, N>
    init() {
        return detail::array_initializer<T, N>().value;
    }
};



} // ns detail





template <class T>
struct initialization<T, detail::enable_default_initialization<T>> : detail::default_initialization<T> {};

template <class T>
struct initialization<T, detail::enable_custom_initialization<T>> : custom_initialization<T> {};


/**
 * @brief Unhandled types catcher
 *
 * Allows to compile but will throw an error on runtime call to init
 * @tparam T
 */
template <class T>
struct initialization<T, detail::enable_initialization_trap<T>>
{
    static inline T
    init() {
        return instantiation_assert<false, T>(R"(

            Type T has no defined zmbt::reflect::initialization specialization to instantiate.

            To resolve this error, provide a custom_initialization partial specialization
            with the static function init(void) -> T

        )");
    }
};


template<> struct initialization<void>
{
    static inline void
    init() {}
};



template <class T>
struct init_tuple_impl;

template <class... T>
struct init_tuple_impl<std::tuple<T...>>
{
    static constexpr std::tuple<T...> init()
    {
        return {initialization<T>::init()...};
    }
};

template <class T>
constexpr T init_tuple()
{
    return init_tuple_impl<T>::init();
}


template <class T>
constexpr T init_value()
{
    return initialization<T>::init();
}

} // namespace reflect
} // namespace zmbt

#endif // ZMBT_REFLECT_INIT_POLICY_HPP_
