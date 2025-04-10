/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_OBJECT_ID_HPP_
#define ZMBT_CORE_OBJECT_ID_HPP_

#include <cstdint>
#include <memory>

#include "aliases.hpp"
#include "entity_id.hpp"
#include "type_info.hpp"



namespace zmbt {



/// Pointer-based object id with type annotation
class object_id : public entity_id {

    boost::json::string to_string(void const* ptr);

    boost::json::string to_string(boost::json::string_view str)
    {
        return str;
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




    object_id(boost::json::string_view str)
        : entity_id(str, "string")
    {
        // TODO: escape str
    }

    template <class T>
    object_id(std::shared_ptr<T> ptr)
        : object_id{(remove_cvref_t<T>*)(ptr.get())}
    {
    }


    object_id(char const* str) : object_id(boost::json::string_view(str))
    {
    }

    object_id(std::string const& str) : object_id(boost::json::string_view(str))
    {
    }

    object_id(nullptr_t)
        : entity_id(to_string(0), type_name<nullptr_t>())
    {
    }

    template<class O>
    object_id(O const* obj)
        : entity_id(to_string(static_cast<void const*>(obj)), type_name<O>().c_str())
    {
    }

    template<class O, class E = mp_if<mp_not<mp_or<is_pointer<O>, is_null_pointer<O>, is_function<O>>>, void>>
    object_id(O const& obj) : object_id(std::addressof(obj))
    {
        static_assert(!is_null_pointer<O>::value, "");
    }


    friend std::ostream& operator<< (std::ostream& os, object_id const& id)
    {
        os << id.str();
        return os;
    }

};


} // namespace zmbt


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
