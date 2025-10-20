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

    static boost::json::string to_string(void const* ptr);

    static boost::json::string to_string(boost::json::string_view str);

    template <class T>
    using is_ptr = mp_if<is_convertible<T, void const*>, bool>;

    template <class T>
    using is_not_ptr = mp_if<mp_not<is_convertible<T, void const*>>, bool>;


  public:

    struct string_key final {};

    using entity_id::entity_id;
    using entity_id::operator boost::json::value;
    using entity_id::operator==;
    using entity_id::operator!=;


    object_id(boost::json::string_view str) : entity_id(to_string(str), typeid(string_key))
    {
    }
    object_id(std::string const& str) : entity_id(str, typeid(string_key))
    {
    }
    // required to bypass is_ptr<T> overload
    object_id(char const* str) : object_id(boost::json::string_view(str))
    {
    }

    template<class T, is_ptr<T> = true>
    object_id(T obj)
        : entity_id(
            to_string(static_cast<void const*>(obj)),
            typeid(remove_cvptr_t<T>)
        )
    {
    }

    template <class T>
    object_id(std::shared_ptr<T> const& ptr)
        : object_id{ptr.get()}
    {
    }

    template<class T, is_not_ptr<T> = true>
    object_id(T const& ref)
        : object_id(std::addressof(ref))
    {
    }


    friend std::ostream& operator<< (std::ostream& os, object_id const& id)
    {
        os << id.str();
        return os;
    }

};


} // namespace zmbt


template <>
struct std::hash<zmbt::object_id>
{
    std::size_t operator()(const zmbt::object_id& k) const
    {
        return hash_value(k);
    }
};


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
