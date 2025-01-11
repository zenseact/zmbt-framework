/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_REFLECT_INTERFACE_POLICY_HPP_
#define ZMBT_REFLECT_INTERFACE_POLICY_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "zmbt/core.hpp"


#define USE_IMLPEMENTATION_TYPEDEFS         \
using host_t   = ifc_host_t<type>;    \
using return_t = ifc_return_t<type>;  \
using args_t   = ifc_args_t<type>;


namespace zmbt {
namespace reflect {


/**
 * @brief Generic interface reflection template policy
 *
 * @details Provides type definitions and 'apply' method for generic interfaces
 *
 * @tparam S
 * @tparam E
 */
template <class S, class E = void>
struct invocation;


template <class T, class E = void>
struct custom_invocation;

namespace detail {


template<class T>
using custom_invocation_t = typename custom_invocation<T>::type;

template<class T>
using has_custom_invocation = mp_valid<custom_invocation_t, T>;

template <class T>
using enable_default_invocation = first_if_t<void,
    is_ifc_handle<T>,
    mp_not<has_custom_invocation<ifc_pointer_t<T>>>
>;


template <class T>
using enable_custom_invocation = first_if_t<void, has_custom_invocation<ifc_pointer_t<T>>>;



/**
 * @brief Generic interface reflection template policy
 *
 * @details Provides type definitions and 'apply' method for generic interfaces
 *
 * @tparam S
 * @tparam E
 */
template <class S, class E = void>
struct default_invocation;



template <class S>
struct default_invocation<S, first_if_t<void, ifc_is_member_handle<S>>>
{
    using type = S;
    USE_IMLPEMENTATION_TYPEDEFS

    template <class H>
    static return_t apply(H&& object, type ifc, args_t args)
    {
        return boost::mp11::tuple_apply(std::mem_fn(ifc), std::tuple_cat(std::forward_as_tuple(object), args));

    }

    template <class H, class... T>
    static return_t apply(H&& object, type ifc, std::tuple<T...> args)
    {
        return default_invocation<S>::apply(std::forward<H>(object), ifc, convert_tuple_to<args_t>(args));
    }
};

template <class S>
struct default_invocation<S, first_if_any_t<void, ifc_is_fn_handle<S>, ifc_is_functor_ref<S>>>
{
    using type = S;
    USE_IMLPEMENTATION_TYPEDEFS

    template <class Ignore>
    static return_t apply(Ignore, type ifc, args_t args)
    {
        return boost::mp11::tuple_apply(ifc, args);
    }

    template <class Ignore, class... T>
    static return_t apply(Ignore, type ifc, std::tuple<T...> args)
    {
        return default_invocation<S>::apply(nullptr, ifc, convert_tuple_to<args_t>(args));
    }
};

template <class S>
struct default_invocation<S, first_if_t<void, ifc_is_functor_ptr<S>>>
{
    using type = S;
    USE_IMLPEMENTATION_TYPEDEFS

    template <class Ignore>
    static return_t apply(Ignore, type ifc, args_t args)
    {
        return boost::mp11::tuple_apply(*ifc, args);
    }

    template <class Ignore, class... T>
    static return_t apply(Ignore, type ifc, std::tuple<T...> args)
    {
        return default_invocation<S>::apply(nullptr, ifc, convert_tuple_to<args_t>(args));
    }
};

} // detail



template <class S>
struct invocation<S, detail::enable_default_invocation<S>> : detail::default_invocation<ifc_pointer_t<S>> {};

template <class S>
struct invocation<S, detail::enable_custom_invocation<S>> : custom_invocation<ifc_pointer_t<S>> {};


template<class T, class E = void>
struct has_invocation_policy_for : std::false_type { };

template<class T>
struct has_invocation_policy_for<T, void_t<typename invocation<T>::type>> : std::true_type { };


template <class I>
using interface_t = typename reflect::invocation<I>::type;


/**
 * @brief Interface arguments tuple type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_args_t = typename invocation<T>::args_t;

/**
 * @brief Interface cv-unqualified arguments tuple type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_args_unqf_t = tuple_unqf_t<invocation_args_t<T>>;

/**
 * @brief Interface has no args
 *
 * @tparam T
 */
template <class T>
using invocation_has_void_args = std::integral_constant<bool, 0 == boost::mp11::mp_size<invocation_args_t<T>>::value >;


/**
 * @brief Interface return type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_ret_t = typename invocation<T>::return_t;

/**
 * @brief Interface cv-unqualified return type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_unqf_ret_t = remove_cvref_t<invocation_ret_t<T>>;

/**
 * @brief Interface has no return
 *
 * @tparam T
 */
template <class T>
using invocation_has_void_ret = std::is_void<invocation_ret_t<T>>;


/**
 * @brief Interface host object type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_host_t = typename invocation<T>::host_t;

/**
 * @brief Interface cv-unqualified host object type
 *
 * @tparam T Interface pointer type
 */
template <class T>
using invocation_unqf_host_t = remove_cvref_t<invocation_host_t<T>>;





template<class Host, class T, class E = void>
struct invocation_is_on_host;

template<class Host, class T>
struct invocation_is_on_host<Host, T,
    std::enable_if_t<std::is_base_of<invocation_unqf_host_t<T>, remove_cvref_t<Host>>::value>
> : std::true_type {};

template<class Host, class T>
struct invocation_is_on_host<Host, T,
    std::enable_if_t<not std::is_base_of<invocation_unqf_host_t<T>, remove_cvref_t<Host>>::value>
> : std::false_type {};

template<class Host, class T>
struct invocation_is_on_host<Host, T,
    std::enable_if_t<not has_invocation_policy_for<T>::value>
> : std::false_type {};



}  // namespace reflect
}  // namespace zmbt

#endif  // ZMBT_REFLECT_INTERFACE_PLUGIN_DEFAULT_HPP_
