
/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_OPERATOR_TRAITS_HPP_
#define ZMBT_CORE_OPERATOR_TRAITS_HPP_

#include <array>
#include <map>
#include <set>
#include <type_traits>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/type_traits.hpp>


#include "aliases.hpp"

namespace zmbt {


/// expand boost::has_operator to containers and tuples
#define ZMBT_WRAP_BOOST_TRAIT(has_operator) \
template <class T, class E = void> struct has_operator; \
template <class T> \
struct has_operator<T, enable_if_t<not is_const_iterable<T>::value and not is_tuple<T>::value>> : boost::has_operator<T> {}; \
template <class T> \
struct has_operator<T, enable_if_t<is_const_iterable<T>::value>> : has_operator<typename T::value_type> {}; \
template <class T> \
struct has_operator<T, enable_if_t<is_tuple<T>::value>> : mp_all_of<T, has_operator> {};

ZMBT_WRAP_BOOST_TRAIT(has_bit_and)
ZMBT_WRAP_BOOST_TRAIT(has_bit_or)
ZMBT_WRAP_BOOST_TRAIT(has_bit_xor)
ZMBT_WRAP_BOOST_TRAIT(has_complement)
ZMBT_WRAP_BOOST_TRAIT(has_divides)
ZMBT_WRAP_BOOST_TRAIT(has_equal_to)
ZMBT_WRAP_BOOST_TRAIT(has_greater)
ZMBT_WRAP_BOOST_TRAIT(has_greater_equal)
ZMBT_WRAP_BOOST_TRAIT(has_left_shift)
ZMBT_WRAP_BOOST_TRAIT(has_right_shift)
ZMBT_WRAP_BOOST_TRAIT(has_less)
ZMBT_WRAP_BOOST_TRAIT(has_less_equal)
ZMBT_WRAP_BOOST_TRAIT(has_logical_and)
ZMBT_WRAP_BOOST_TRAIT(has_logical_not)
ZMBT_WRAP_BOOST_TRAIT(has_logical_or)
ZMBT_WRAP_BOOST_TRAIT(has_minus)
ZMBT_WRAP_BOOST_TRAIT(has_modulus)
ZMBT_WRAP_BOOST_TRAIT(has_multiplies)
ZMBT_WRAP_BOOST_TRAIT(has_negate)
ZMBT_WRAP_BOOST_TRAIT(has_not_equal_to)
ZMBT_WRAP_BOOST_TRAIT(has_plus)


#undef ZMBT_WRAP_BOOST_TRAIT

}

#endif // ZMBT_CORE_OPERATOR_TRAITS_HPP_
