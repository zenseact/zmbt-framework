/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_NS_HPP_
#define ZMBT_CORE_NS_HPP_


#include <type_traits>
#include <tuple>

#include <boost/callable_traits.hpp>
#include <boost/mp11.hpp>


#define USING_STD_TYPETRAIT(trait) template <class T> using trait = std::trait<T>;
#define USING_TYPETRAIT_V(trait) template<class T> static constexpr bool trait##_v = trait<T>::value;
#define USING_TYPETRAIT_T(trait) template<class T> using trait##_t = typename trait<T>::type;

#define USING_STD_TYPETRAIT_BINARY(trait)                                       \
template<class A, class B> using trait = std::trait<A, B>;                      \
template<class A, class B> static constexpr bool trait##_v = trait<A, B>::value;

#define USING_STD_TYPETRAIT_PROPERTY(trait)  USING_STD_TYPETRAIT(trait) USING_TYPETRAIT_V(trait)
#define USING_STD_TYPETRAIT_TRANSFORM(trait) USING_STD_TYPETRAIT(trait) USING_TYPETRAIT_T(trait)


namespace zmbt {


using namespace boost::mp11;
namespace ct = boost::callable_traits;


using std::nullptr_t;
using std::size_t;

template <class...> using void_t = void;

template<class T, T V>
using integral_constant = std::integral_constant<T, V>;

template<bool V>
using bool_constant = integral_constant<bool, V>;


template<class... T>
using tuple = std::tuple<T...>;

template< class T >
using tuple_size = std::tuple_size<T>;

template< class T >
static constexpr size_t tuple_size_v = tuple_size<T>::value;

using std::make_tuple;
using std::tie;

USING_STD_TYPETRAIT_PROPERTY(is_void)
USING_STD_TYPETRAIT_PROPERTY(is_null_pointer)
USING_STD_TYPETRAIT_PROPERTY(is_integral)
USING_STD_TYPETRAIT_PROPERTY(is_floating_point)
USING_STD_TYPETRAIT_PROPERTY(is_array)
USING_STD_TYPETRAIT_PROPERTY(is_enum)
USING_STD_TYPETRAIT_PROPERTY(is_union)
USING_STD_TYPETRAIT_PROPERTY(is_class)
USING_STD_TYPETRAIT_PROPERTY(is_function)
USING_STD_TYPETRAIT_PROPERTY(is_pointer)
USING_STD_TYPETRAIT_PROPERTY(is_lvalue_reference)
USING_STD_TYPETRAIT_PROPERTY(is_rvalue_reference)
USING_STD_TYPETRAIT_PROPERTY(is_member_object_pointer)
USING_STD_TYPETRAIT_PROPERTY(is_member_function_pointer)
USING_STD_TYPETRAIT_PROPERTY(is_fundamental)
USING_STD_TYPETRAIT_PROPERTY(is_arithmetic)
USING_STD_TYPETRAIT_PROPERTY(is_scalar)
USING_STD_TYPETRAIT_PROPERTY(is_object)
USING_STD_TYPETRAIT_PROPERTY(is_compound)
USING_STD_TYPETRAIT_PROPERTY(is_reference)
USING_STD_TYPETRAIT_PROPERTY(is_member_pointer)
USING_STD_TYPETRAIT_PROPERTY(is_const)
USING_STD_TYPETRAIT_PROPERTY(is_volatile)
USING_STD_TYPETRAIT_PROPERTY(is_trivial)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_copyable)
USING_STD_TYPETRAIT_PROPERTY(is_standard_layout)
USING_STD_TYPETRAIT_PROPERTY(is_empty)
USING_STD_TYPETRAIT_PROPERTY(is_polymorphic)
USING_STD_TYPETRAIT_PROPERTY(is_abstract)
USING_STD_TYPETRAIT_PROPERTY(is_final)
USING_STD_TYPETRAIT_PROPERTY(is_signed)
USING_STD_TYPETRAIT_PROPERTY(is_unsigned)

USING_STD_TYPETRAIT_PROPERTY(is_default_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_default_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_default_constructible)

USING_STD_TYPETRAIT_PROPERTY(is_copy_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_copy_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_copy_constructible)

USING_STD_TYPETRAIT_PROPERTY(is_move_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_move_constructible)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_move_constructible)

USING_STD_TYPETRAIT_PROPERTY(is_destructible)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_destructible)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_destructible)

USING_STD_TYPETRAIT_PROPERTY(has_virtual_destructor)

USING_STD_TYPETRAIT_PROPERTY(is_copy_assignable)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_copy_assignable)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_copy_assignable)
USING_STD_TYPETRAIT_PROPERTY(is_move_assignable)
USING_STD_TYPETRAIT_PROPERTY(is_trivially_move_assignable)
USING_STD_TYPETRAIT_PROPERTY(is_nothrow_move_assignable)


USING_STD_TYPETRAIT_BINARY(is_same)
USING_STD_TYPETRAIT_BINARY(is_base_of)
USING_STD_TYPETRAIT_BINARY(is_convertible)

USING_STD_TYPETRAIT_BINARY(is_assignable)
USING_STD_TYPETRAIT_BINARY(is_trivially_assignable)
USING_STD_TYPETRAIT_BINARY(is_nothrow_assignable)


USING_STD_TYPETRAIT_TRANSFORM(remove_cv)
USING_STD_TYPETRAIT_TRANSFORM(remove_const)
USING_STD_TYPETRAIT_TRANSFORM(remove_volatile)
USING_STD_TYPETRAIT_TRANSFORM(add_cv)
USING_STD_TYPETRAIT_TRANSFORM(add_const)
USING_STD_TYPETRAIT_TRANSFORM(add_volatile)
USING_STD_TYPETRAIT_TRANSFORM(remove_reference)
USING_STD_TYPETRAIT_TRANSFORM(add_lvalue_reference)
USING_STD_TYPETRAIT_TRANSFORM(add_rvalue_reference)
USING_STD_TYPETRAIT_TRANSFORM(remove_pointer)
USING_STD_TYPETRAIT_TRANSFORM(add_pointer)
USING_STD_TYPETRAIT_TRANSFORM(make_signed)
USING_STD_TYPETRAIT_TRANSFORM(make_unsigned)

USING_STD_TYPETRAIT_TRANSFORM(underlying_type)

USING_STD_TYPETRAIT_TRANSFORM(decay)


template <class T, class... Args>
using is_constructible = std::is_constructible<T, Args...>;

template <class T, class... Args>
using is_trivially_constructible = std::is_trivially_constructible<T, Args...>;

template <class T, class... Args>
using is_nothrow_constructible = std::is_nothrow_constructible<T, Args...>;


template <class T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <class T>
using remove_cvptr_t = remove_cv_t<remove_pointer_t<T>>;


template< bool B, class T = void >
using enable_if = std::enable_if<B, T>;

template< bool B, class T = void >
using enable_if_t = typename enable_if<B, T>::type;

template<bool B, class T, class F>
using conditional = std::conditional<B, T, F>;

template<bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

template<class... T>
using common_type = std::common_type<T...>;

template<class... T>
using common_type_t = typename common_type<T...>::type;




// rank
// extent
// remove_extent
// remove_all_extents

}

#endif
