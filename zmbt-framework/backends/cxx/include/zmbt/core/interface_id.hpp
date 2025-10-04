/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

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


/// Pointer-based interface id with type annotation
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
    using entity_id::operator<;
    using entity_id::operator>;
    using entity_id::operator<=;
    using entity_id::operator>=;

    template <class Interface, class = mp_if<is_ifc_handle<Interface const&>, void>>
    interface_id(Interface const& ifc)
        : entity_id(
            ifc_addr(ifc),
            type<remove_pointer_t<remove_cvref_t<ifc_handle_t<Interface const&>>>>
        )
    {
    }
};


} // namespace zmbt


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
