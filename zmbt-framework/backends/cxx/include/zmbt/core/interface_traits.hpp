/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_INTERFACE_TRAITS_HPP_
#define ZMBT_CORE_INTERFACE_TRAITS_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "aliases.hpp"
#include "generic_traits.hpp"



namespace zmbt {



// PREDICATES

template<class T, class D = decay_t<T>>
using ifc_is_pmf_handle = is_member_function_pointer<D>;

template<class T, class D = decay_t<T>>
using ifc_is_pmd_handle = is_member_object_pointer<D>;

template<class T, class D = decay_t<T>>
using ifc_is_member_handle = is_member_pointer<D>;

template<class T, class D = decay_t<T>>
using ifc_is_fn_ref = mp_all<
    is_lvalue_reference<T>,
    is_function<remove_pointer_t<D>>
>;

template<class T, class D = decay_t<T>>
using ifc_is_fn_ptr = mp_all<
    is_pointer<T>,
    is_function<remove_pointer_t<D>>
>;

template<class T>
using ifc_is_fn_handle = mp_any<
    ifc_is_fn_ref<T>, ifc_is_fn_ptr<T>
>;


namespace detail {
template<class T, class=void>
struct support_calltraits_function : mp_false {};

template<class T>
struct support_calltraits_function<T, void_t<ct::function_type_t<T>>> : mp_true {};
}




template<class T, class P = remove_reference_t<T>, class D = remove_pointer_t<P>>
using ifc_is_functor_ptr = mp_all<
    is_pointer<P>, is_class<D>,
    detail::support_calltraits_function<D>
>;

template<class T, class D = remove_reference_t<T>>
using ifc_is_functor_ref = mp_all<
    is_reference<T>, is_class<D>,
    detail::support_calltraits_function<D>
>;

template <class T>
using ifc_is_functor_handle = mp_any<
    ifc_is_functor_ptr<T>,
    ifc_is_functor_ref<T>
>;


template<class T>
using is_ifc_handle = mp_any<
    ifc_is_pmf_handle<T>,
    ifc_is_pmd_handle<T>,
    ifc_is_fn_handle<T>,
    ifc_is_functor_handle<T>
>;


/// Get pointer to callable object
//@{
template <class T, class R = decay_t<T>>
auto get_ifc_pointer (T  x) -> first_if_t<R,
    ifc_is_member_handle<T>
>
{
    return x;
}

template <class T, class R = decay_t<T>>
auto get_ifc_pointer (T x) -> first_if_t<R,
    ifc_is_fn_handle<T>
>
{
    return x;
}

template <class T, class R = add_pointer_t<remove_reference_t<T>>>
auto get_ifc_pointer (T*  x) -> first_if_t<R,
    ifc_is_functor_ptr<T*>
>
{
    return x;
}

template <class T, class R = add_pointer_t<remove_reference_t<T>>>
auto get_ifc_pointer (T&  x) -> first_if_t<R,
    ifc_is_functor_ref<T&>
>
{
    return &x;
}
//@}


/// Get reference to callable object
//@{
template <class T, class R = decay_t<T> const>
auto get_ifc_handle (T const  x) -> first_if_t<R,
    ifc_is_member_handle<T>
>
{
    return x;
}

template <class T, class R = decay_t<T> const>
auto get_ifc_handle (T  const x) -> first_if_t<R,
    ifc_is_fn_handle<T>
>
{
    return x;
}

template <class T, class R = add_lvalue_reference_t<T> const>
auto get_ifc_handle (T *  x) -> first_if_t<R,
    ifc_is_functor_ptr<T*>
>
{
    return &x;
}

template <class T, class R = add_lvalue_reference_t<T> const>
auto get_ifc_handle (T &  x) -> first_if_t<R,
    ifc_is_functor_ref<T&>
>
{
    return x;
}
//@}


namespace detail {
template <class T>
using ifc_pointer_valid_t = decltype(get_ifc_pointer(std::declval<T>()));
}

template <class T>
struct ifc_pointer  {
    using type = mp_eval_or<nullptr_t, detail::ifc_pointer_valid_t, T>;
};

template <class T>
using ifc_pointer_t = typename ifc_pointer<T>::type;


namespace detail {
template <class T>
using ifc_handle_valid_t = decltype(get_ifc_handle(std::declval<T>()));
}

template <class T>
struct ifc_handle  {
    using type = mp_eval_or<nullptr_t, detail::ifc_handle_valid_t, T>;
};

template <class T>
using ifc_handle_t = typename ifc_handle<T>::type;



// PART TRAITS

namespace detail {
template <class T>
struct qualified_class_of {
    using type = ct::qualified_class_of_t<T>;
};
}

template <class T, class = void>
struct ifc_host {
    using type = typename mp_if<ifc_is_member_handle<T>,  ct::qualified_class_of<T>, mp_identity<nullptr_t>>::type;
};


template <class T>
using ifc_host_t = typename ifc_host<T>::type;

/// @brief Resolves to H*{} for member function pointers of H,
/// or to nullptr_t for other callables
template <class T>
constexpr add_pointer_t<remove_reference_t<ifc_host_t<T>>> ifc_host_nullptr {};


namespace detail {
template <class T, class = void>
struct ifc_args_impl {
    using type = ct::args_t<remove_pointer_t<T>>;
};

template <class T>
struct ifc_args_impl <T, first_if_t<void_t<ct::args_t<T>>, ifc_is_member_handle<T>>> {
    using type = mp_rest<ct::args_t<T>>;
};

template <class T>
struct ifc_args_impl <T, first_if_none_t<void_t<ct::args_t<T>>, ifc_is_member_handle<T> >> {
    using type = ct::args_t<T>;
};

} // namespace detail

template <class T>
struct ifc_args
{
    using type = typename mp_eval_or<mp_identity<void>, detail::ifc_args_impl, ifc_handle_t<T>>::type;
};

template <class T>
using ifc_args_t = typename ifc_args<T>::type;


template <class T>
struct ifc_return : ct::return_type<T> {
    using type = typename mp_eval_or<mp_identity<void>, ct::return_type, ifc_handle_t<T>>::type;
};

template <class T>
using ifc_return_t = typename ifc_return<T>::type;

}  // namespace zmbt

#endif  // ZMBT_CORE_INTERFACE_TRAITS_HPP_

