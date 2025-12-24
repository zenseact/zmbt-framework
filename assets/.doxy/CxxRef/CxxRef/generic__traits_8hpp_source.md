

# File generic\_traits.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**generic\_traits.hpp**](generic__traits_8hpp.md)

[Go to the documentation of this file](generic__traits_8hpp.md)


```C++

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

template <class T, class... A>
T construct_or_default(A&&... args)
{
    return detail::construct_or_default_impl<T, mp_list<A...>>::construct(std::forward_as_tuple(args...));
}

} // namespace zmbt

#endif // ZMBT_CORE_GENERIC_TRAITS_HPP_
```


