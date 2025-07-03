

# File tuple\_conversion.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**tuple\_conversion.hpp**](tuple__conversion_8hpp.md)

[Go to the documentation of this file](tuple__conversion_8hpp.md)


```C++

#ifndef ZMBT_CORE_TUPLE_CONVERSION_HPP_
#define ZMBT_CORE_TUPLE_CONVERSION_HPP_

#include <tuple>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail {

template <class T, class... A>
struct tuple_conversion_handler {

    T target;
    tuple_conversion_handler(tuple<A...>& source) : tuple_conversion_handler{source, std::make_index_sequence<sizeof...(A)>()}
    {
    }

  private:

    template <size_t... I>
    tuple_conversion_handler(tuple<A...>& source, std::index_sequence<I...>)
    // TODO: refactor with static_cast
    : target{std::get<I>(source)...}
    {
    }
};

}  // namespace detail


template <class T, class... A>
inline T convert_tuple_to(tuple<A...> &source) {
    return detail::tuple_conversion_handler<T, A...> (source).target;
}

}  // namespace zmbt

#endif  // ZMBT_CORE_TUPLE_CONVERSION_HPP_
```


