/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_INTERFACE_ID_HPP_
#define ZMBT_CORE_INTERFACE_ID_HPP_

#include <cstdint>

#include "aliases.hpp"

#include "entity_id.hpp"
#include "interface_traits.hpp"
#include "type_info.hpp"

namespace zmbt {


/// Pointer-based interface id with type annotation
class interface_id : public entity_id {
    struct dummy {
        virtual void f() = 0;
        virtual ~dummy() = default;
    };

    static constexpr size_t mfp_size = sizeof(&dummy::f);

    static boost::json::string bytes_to_str(std::uint8_t (&dat)[mfp_size]);

    template <class T>
    static boost::json::string ifc_addr(T const& ifc)
    {
        union {
            ifc_pointer_t<T const&> ptr;
            std::uint8_t bytes[mfp_size];
        } u;
        std::memset(u.bytes, 0, mfp_size);

        u.ptr = get_ifc_pointer(ifc);
        return bytes_to_str(u.bytes);
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
