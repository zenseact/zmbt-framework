

# File interface\_id.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**interface\_id.hpp**](interface__id_8hpp.md)

[Go to the documentation of this file](interface__id_8hpp.md)


```C++

#ifndef ZMBT_CORE_INTERFACE_ID_HPP_
#define ZMBT_CORE_INTERFACE_ID_HPP_

#include <iostream>


#include <cstdint>
#include <iomanip>
#include <sstream>

#include "aliases.hpp"

#include "entity_id.hpp"
#include "interface_traits.hpp"
#include "type_info.hpp"

namespace zmbt {


class interface_id : public entity_id {

    template <class T>
    boost::json::string ifc_addr(T const& ifc)
    {
        struct {
            ifc_pointer_t<T const&> ptr;
        } wrapper {get_ifc_pointer(ifc)};

        std::array<char, sizeof(wrapper)> repr {};
        std::memcpy(repr.data(), &wrapper, sizeof(wrapper));

        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setfill('0') << "0x";
        for (auto const c: repr)
        {
            ss  << std::setw(2) << (0xFFU & static_cast<std::uint32_t>(c));
        }
        return ss.str().c_str();
    }


  public:

    using entity_id::entity_id;
    using entity_id::operator boost::json::value;
    using entity_id::operator==;
    using entity_id::operator!=;
    using entity_id::operator<;
    using entity_id::operator>;
    using entity_id::operator<=;
    using entity_id::operator>=;

    template <class T, class = mp_if<is_ifc_handle<T const&>, void>>
    interface_id(T const& ifc)
        : entity_id(
            ifc_addr(ifc),
            type_name<
              remove_pointer_t<remove_cvref_t<ifc_handle_t<T const&>>>
            >()
        )
    {
    }
};


} // namespace zmbt


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
```


