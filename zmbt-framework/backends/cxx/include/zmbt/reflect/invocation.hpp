/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_REFLECT_INTERFACE_HPP_
#define ZMBT_REFLECT_INTERFACE_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "zmbt/core.hpp"


#define DEFAULT_INVOCATION_TYPEDEFS  \
using host_t   = ifc_host_t<type>;   \
using return_t = ifc_return_t<type>; \
using args_t   = ifc_args_t<type>;


namespace zmbt {
namespace reflect {


/**
 * @brief Interface reflection metafunction
 *
 * @details Provides type definitions and 'apply' method for callables
 *
 * @tparam T callable type
 * @tparam E SFINAE enabler
 */
template <class T, class E = void>
struct invocation;


/**
 * @brief Customize the interface reflection metafunction
 *
 * @details Provide custom type definitions and 'apply' method for callables.
 * Required fields:
 * - type: callable type (metafunction template parameter)
 * - host_t: host object type
 * - return_t: callable return type
 * - args_t: callable arguments tuple type
 * - apply: callable invocation method with signature:
 *      1. If host_t is nullptr_t:
 * ```
 * static return_t apply(nullptr_t, type ifc, args_t args);
 * ```
 *      2. If host_t is class:
 * ```
 * template <class H>
 * static return_t apply(H&& object, type ifc, args_t args);
 * ```
 *
 * For class methods, the `apply` function must support references and pointers,
 * including smart pointers. Therefore, a templated version of the `apply` is recommended
 * but can be replaced with a set of overloads if necessary.
 *
 * @tparam T callable type
 * @tparam E SFINAE enabler
 */
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

} // namespace detail

/**
 * @brief Interface reflection metafunction
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
    DEFAULT_INVOCATION_TYPEDEFS

    template <class H>
    static return_t apply(H&& object, type ifc, args_t args)
    {
        return boost::mp11::tuple_apply(std::mem_fn(ifc), std::tuple_cat(std::forward_as_tuple(object), args));
    }
};

template <class S>
struct default_invocation<S, first_if_any_t<void, ifc_is_fn_handle<S>, ifc_is_functor_ref<S>>>
{
    using type = S;
    DEFAULT_INVOCATION_TYPEDEFS

    static return_t apply(host_t, type ifc, args_t args)
    {
        static_assert(std::is_same<host_t, nullptr_t>::value, "");
        return boost::mp11::tuple_apply(ifc, args);
    }
};

template <class S>
struct default_invocation<S, first_if_t<void, ifc_is_functor_ptr<S>>>
{
    using type = S;
    DEFAULT_INVOCATION_TYPEDEFS

    static return_t apply(host_t, type ifc, args_t args)
    {
        static_assert(std::is_same<host_t, nullptr_t>::value, "");
        return boost::mp11::tuple_apply(*ifc, args);
    }
};




template <class S>
struct invocation<S, detail::enable_default_invocation<S>> : default_invocation<ifc_pointer_t<S>> {};

template <class S>
struct invocation<S, detail::enable_custom_invocation<S>> : custom_invocation<ifc_pointer_t<S>> {};

/// Trait check for reflect::invocation<T>
template<class T, class E = void>
struct has_invocation_for : std::false_type { };

template<class T>
struct has_invocation_for<T, void_t<typename invocation<T>::type>> : std::true_type { };


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
using invocation_ret_unqf_t = remove_cvref_t<invocation_ret_t<T>>;

/**
 * @brief Interface has no return
 *
 * @tparam T
 */
template <class T>
using invocation_has_void_ret = std::is_void<invocation_ret_t<T>>;


template <class T>
using invocation_ret_unqf_or_nullptr_t = mp_if<invocation_has_void_ret<T>, invocation_ret_unqf_t<T>, nullptr_t>;




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

}  // namespace reflect
}  // namespace zmbt


#undef DEFAULT_INVOCATION_TYPEDEFS
#endif  // ZMBT_REFLECT_INTERFACE_PLUGIN_DEFAULT_HPP_
