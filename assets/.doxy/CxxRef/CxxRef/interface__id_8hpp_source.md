

# File interface\_id.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**interface\_id.hpp**](interface__id_8hpp.md)

[Go to the documentation of this file](interface__id_8hpp.md)


```C++

#ifndef ZMBT_CORE_INTERFACE_ID_HPP_
#define ZMBT_CORE_INTERFACE_ID_HPP_

#include <iostream>


#include <array>
#include <cstdint>
#include <cstring>
#include <typeinfo>
#include <utility>

#include <boost/mp11/tuple.hpp>

#include "aliases.hpp"

#include "entity_id.hpp"
#include "interface_traits.hpp"
#include "type_info.hpp"

namespace zmbt {


class interface_id : public entity_id {


    template <class Interface>
    boost::json::string ifc_addr(Interface const& ifc)
    {
        struct {
            ifc_pointer_t<Interface const&> ptr;
        } wrapper {get_ifc_pointer(ifc)};

        std::array<unsigned char, sizeof(wrapper)> repr {};
        std::memcpy(repr.data(), &wrapper, repr.size());

        constexpr char digits[] = "0123456789ABCDEF";

        boost::json::string encoded;
        encoded.reserve(repr.size() * 2 + 4);

        std::size_t pending_zero_bytes = 0U;
        std::size_t emitted_bytes = 0U;

        boost::mp11::tuple_for_each(repr, [&](unsigned char byte){
            if (byte == 0U)
            {
                ++pending_zero_bytes;
                return;
            }

            while (pending_zero_bytes > 0U)
            {
                encoded.push_back('0');
                encoded.push_back('0');
                --pending_zero_bytes;
                ++emitted_bytes;
            }
            encoded.push_back(digits[(byte >> 4U) & 0x0FU]);
            encoded.push_back(digits[byte & 0x0FU]);
            ++emitted_bytes;
        });

        if (emitted_bytes == 0U)
        {
            encoded.push_back('0');
            emitted_bytes = 1U;
            pending_zero_bytes = 0U;
        }

        if (pending_zero_bytes > 0U)
        {
            // Put num of trailing zero bytes after underscore in decimal
            encoded.push_back('_');
            std::array<char, 20U> buffer {};
            std::size_t idx = buffer.size();
            std::size_t value = pending_zero_bytes * 2U;
            do
            {
                buffer[--idx] = static_cast<char>('0' + (value % 10U));
                value /= 10U;
            }
            while (value != 0U);

            encoded.append(buffer.data() + idx, buffer.data() + buffer.size());
        }

        return encoded;
    }


  public:

    using entity_id::entity_id;
    using entity_id::operator boost::json::value;
    using entity_id::operator==;
    using entity_id::operator!=;

    template <class Interface, class = mp_if<is_ifc_handle<Interface const&>, void>>
    interface_id(Interface const& ifc)
        : entity_id(
            ifc_addr(ifc),
            typeid(remove_pointer_t<remove_cvref_t<ifc_handle_t<Interface const&>>>)
        )
    {
    }
};


} // namespace zmbt

template <>
struct std::hash<zmbt::interface_id>
{
    std::size_t operator()(const zmbt::interface_id& k) const
    {
        return hash_value(k);
    }
};


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
```


