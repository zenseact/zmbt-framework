

# File type\_info.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**type\_info.hpp**](type__info_8hpp.md)

[Go to the documentation of this file](type__info_8hpp.md)


```C++

#ifndef ZMBT_CORE_TYPE_INFO_HPP_
#define ZMBT_CORE_TYPE_INFO_HPP_


#include <type_traits>
#include <boost/type_index.hpp>


#include "aliases.hpp"

namespace zmbt {


template <class T>
inline std::string type_name()
{
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

template <class T>
inline std::string type_name(T)
{
    return type_name<T>();
}


}  // namespace zmbt

#endif  // ZMBT_CORE_TYPE_INFO_HPP_
```


