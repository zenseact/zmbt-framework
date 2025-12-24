/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
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

template<class Interface, class DecayedInterface = decay_t<Interface>>
using ifc_is_pmf_handle = is_member_function_pointer<DecayedInterface>;

template<class Interface, class DecayedInterface = decay_t<Interface>>
using ifc_is_pmd_handle = is_member_object_pointer<DecayedInterface>;

template<class Interface, class DecayedInterface = decay_t<Interface>>
using ifc_is_member_handle = is_member_pointer<DecayedInterface>;

template<class Interface, class DecayedInterface = decay_t<Interface>>
using ifc_is_fn_ref = mp_all<
    is_lvalue_reference<Interface>,
    is_function<remove_pointer_t<DecayedInterface>>
>;

template<class Interface, class DecayedInterface = decay_t<Interface>>
using ifc_is_fn_ptr = mp_all<
    is_pointer<Interface>,
    is_function<remove_pointer_t<DecayedInterface>>
>;

template<class Interface>
using ifc_is_fn_handle = mp_any<
    ifc_is_fn_ref<Interface>, ifc_is_fn_ptr<Interface>
>;


namespace detail {
template<class Interface, class = void>
struct support_calltraits_function : mp_false {};

template<class Interface>
struct support_calltraits_function<Interface, void_t<ct::function_type_t<Interface>>> : mp_true {};
}




template<class Interface, class InterfacePointer = remove_reference_t<Interface>, class Functor = remove_pointer_t<InterfacePointer>>
using ifc_is_functor_ptr = mp_all<
    is_pointer<InterfacePointer>, is_class<Functor>,
    detail::support_calltraits_function<Functor>
>;

template<class Interface, class Functor = remove_reference_t<Interface>>
using ifc_is_functor_ref = mp_all<
    is_reference<Interface>, is_class<Functor>,
    detail::support_calltraits_function<Functor>
>;

template <class Interface>
using ifc_is_functor_handle = mp_any<
    ifc_is_functor_ptr<Interface>,
    ifc_is_functor_ref<Interface>
>;


template<class Interface>
using is_ifc_handle = mp_any<
    ifc_is_pmf_handle<Interface>,
    ifc_is_pmd_handle<Interface>,
    ifc_is_fn_handle<Interface>,
    ifc_is_functor_handle<Interface>
>;


/// Get pointer to callable object
//@{
template <class Interface, class Result = decay_t<Interface>>
auto get_ifc_pointer (Interface  x) -> first_if_t<Result,
    ifc_is_member_handle<Interface>
>
{
    return x;
}

template <class Interface, class Result = decay_t<Interface>>
auto get_ifc_pointer (Interface x) -> first_if_t<Result,
    ifc_is_fn_handle<Interface>
>
{
    return x;
}

template <class Interface, class Result = add_pointer_t<remove_reference_t<Interface>>>
auto get_ifc_pointer (Interface*  x) -> first_if_t<Result,
    ifc_is_functor_ptr<Interface*>
>
{
    return x;
}

template <class Interface, class Result = add_pointer_t<remove_reference_t<Interface>>>
auto get_ifc_pointer (Interface&  x) -> first_if_t<Result,
    ifc_is_functor_ref<Interface&>
>
{
    return &x;
}
//@}


/// Get reference to callable object
//@{
template <class Interface, class Result = decay_t<Interface> const>
auto get_ifc_handle (Interface const  x) -> first_if_t<Result,
    ifc_is_member_handle<Interface>
>
{
    return x;
}

template <class Interface, class Result = decay_t<Interface> const>
auto get_ifc_handle (Interface  const x) -> first_if_t<Result,
    ifc_is_fn_handle<Interface>
>
{
    return x;
}

template <class Interface, class Result = add_lvalue_reference_t<Interface> const>
auto get_ifc_handle (Interface *  x) -> first_if_t<Result,
    ifc_is_functor_ptr<Interface*>
>
{
    return &x;
}

template <class Interface, class Result = add_lvalue_reference_t<Interface> const>
auto get_ifc_handle (Interface &  x) -> first_if_t<Result,
    ifc_is_functor_ref<Interface&>
>
{
    return x;
}
//@}


namespace detail {
template <class Interface>
using ifc_pointer_valid_t = decltype(get_ifc_pointer(std::declval<Interface>()));
}

template <class Interface>
struct ifc_pointer  {
    using type = mp_eval_or<nullptr_t, detail::ifc_pointer_valid_t, Interface>;
};

template <class Interface>
using ifc_pointer_t = typename ifc_pointer<Interface>::type;


namespace detail {
template <class Interface>
using ifc_handle_valid_t = decltype(get_ifc_handle(std::declval<Interface>()));
}

template <class Interface>
struct ifc_handle  {
    using type = mp_eval_or<nullptr_t, detail::ifc_handle_valid_t, Interface>;
};

template <class Interface>
using ifc_handle_t = typename ifc_handle<Interface>::type;



// PART TRAITS

namespace detail {
template <class T>
struct qualified_class_of {
    using type = ct::qualified_class_of_t<T>;
};
}

template <class Interface, class = void>
struct ifc_host {
    using type = typename mp_if<ifc_is_member_handle<Interface>,  ct::qualified_class_of<Interface>, mp_identity<nullptr_t>>::type;
};


template <class Interface>
using ifc_host_t = typename ifc_host<Interface>::type;

/// @brief Resolves to H*{} for member function pointers of H,
/// or to nullptr_t for other callables
template <class Interface>
constexpr add_pointer_t<remove_reference_t<ifc_host_t<Interface>>> ifc_host_nullptr {};


namespace detail {
template <class Interface, class = void>
struct ifc_args_impl {
    using type = ct::args_t<remove_pointer_t<Interface>>;
};

template <class Interface>
struct ifc_args_impl <Interface, first_if_t<void_t<ct::args_t<Interface>>, ifc_is_member_handle<Interface>>> {
    using type = mp_rest<ct::args_t<Interface>>;
};

template <class Interface>
struct ifc_args_impl <Interface, first_if_none_t<void_t<ct::args_t<Interface>>, ifc_is_member_handle<Interface> >> {
    using type = ct::args_t<Interface>;
};

} // namespace detail

template <class Interface>
struct ifc_args
{
    using type = typename mp_eval_or<mp_identity<void>, detail::ifc_args_impl, ifc_handle_t<Interface>>::type;
};

template <class Interface>
using ifc_args_t = typename ifc_args<Interface>::type;


template <class Interface>
struct ifc_return : ct::return_type<Interface> {
    using type = typename mp_eval_or<mp_identity<void>, ct::return_type, ifc_handle_t<Interface>>::type;
};

template <class Interface>
using ifc_return_t = typename ifc_return<Interface>::type;

}  // namespace zmbt

#endif  // ZMBT_CORE_INTERFACE_TRAITS_HPP_
