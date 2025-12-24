

# File tuple\_exchange.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**tuple\_exchange.hpp**](tuple__exchange_8hpp.md)

[Go to the documentation of this file](tuple__exchange_8hpp.md)


```C++

#ifndef ZMBT_CORE_TUPLE_EXCHANGE_HPP_
#define ZMBT_CORE_TUPLE_EXCHANGE_HPP_

#include <tuple>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {

namespace detail {

struct tuple_exchange_helper
{
    template <class T>
    using is_assignable_reference = mp_and<std::is_lvalue_reference<T>, mp_not<std::is_const<T>>>;

    template <class A, class B>
    static void assign(A & a, B const& b)
    {
        a = b;
    }

    template <class A, class B>
    static void assign(A const&, B const&) {}
};

}


template <class T1, class T2>
void tuple_exchange(T1 & target, T2 const& src)
{
    static_assert(mp_same<mp_size<T1>, mp_size<T2>>::value, "tuples shall be of equal size");

    using helper = detail::tuple_exchange_helper;
    mp_for_each<mp_iota<mp_size<T1>>>([&](auto N){
        using A = mp_at_c<T1, N>;
        if (helper::is_assignable_reference<A>::value) {
            helper::assign(
                std::forward<A>(std::get<N>(target)),
                std::get<N>(src)
            );
        }
    });
}

}  // namespace zmbt

#endif  // ZMBT_CORE_TUPLE_CONVERSION_HPP_
```


