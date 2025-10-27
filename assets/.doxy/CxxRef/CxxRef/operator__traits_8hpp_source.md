

# File operator\_traits.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**operator\_traits.hpp**](operator__traits_8hpp.md)

[Go to the documentation of this file](operator__traits_8hpp.md)


```C++

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
```


