

# File tuple\_indexator.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**tuple\_indexator.hpp**](tuple__indexator_8hpp.md)

[Go to the documentation of this file](tuple__indexator_8hpp.md)


```C++

#ifndef ZMBT_CORE_TUPLE_INDEXATOR_HPP_
#define ZMBT_CORE_TUPLE_INDEXATOR_HPP_

#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail
{

template <class I>
struct tuple_indexator;

template <size_t... I>
struct tuple_indexator<std::index_sequence<I...>> {
    using type = tuple<std::integral_constant<size_t, I>...>;
    using sequence = std::index_sequence<I...>;
};
template <class... A>
struct tuple_indexator<tuple<A...>> {
    using type = typename tuple_indexator<std::make_index_sequence<sizeof...(A)>>::type;
    using sequence = typename tuple_indexator<std::make_index_sequence<sizeof...(A)>>::sequence;
};

} // namespace detail

template <class T>
using  indexator_for = typename detail::tuple_indexator<T>::type;



}  // namespace zmbt

#endif
```


