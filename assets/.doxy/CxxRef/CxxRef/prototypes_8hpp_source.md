

# File prototypes.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**prototypes.hpp**](prototypes_8hpp.md)

[Go to the documentation of this file](prototypes_8hpp.md)


```C++

#ifndef ZMBT_REFLECT_PROTOTYPES_HPP_
#define ZMBT_REFLECT_PROTOTYPES_HPP_

#include "zmbt/core.hpp"
#include "signal_traits.hpp"
#include "serialization.hpp"
#include "invocation.hpp"

namespace zmbt {
namespace reflect {

namespace detail
{
template <class T>
auto init_return() -> mp_if<is_void<T>, boost::json::value>
{
    return nullptr;
}

template <class T>
auto init_return() -> mp_if<mp_not<is_void<T>>, boost::json::value>
{
    return json_from(reflect::signal_traits<T>::init());
}

template <class T>
auto init_args()
{
    return json_from(reflect::init_tuple<T>()).as_array();
}

} // namespace detail


template<class T>
boost::json::object prototypes()
{
    static_assert(is_ifc_handle<T>::value, "");

    using args_unqf_t = reflect::invocation_args_unqf_t<T>;
    using unqf_ret_t = reflect::invocation_unqf_ret_t<T>;
    return {
        {"args"  , detail::init_args<args_unqf_t>()},
        {"return", detail::init_return<unqf_ret_t>()}
    };
}

} // namespace reflect
} // namespace zmbt

#endif // ZMBT_REFLECT_PROTOTYPES_HPP_
```


