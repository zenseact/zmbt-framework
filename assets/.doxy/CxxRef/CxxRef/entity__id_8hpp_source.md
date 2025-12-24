

# File entity\_id.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**entity\_id.hpp**](entity__id_8hpp.md)

[Go to the documentation of this file](entity__id_8hpp.md)


```C++

#ifndef ZMBT_CORE_ENTITY_ID_HPP_
#define ZMBT_CORE_ENTITY_ID_HPP_

#include <cstddef>
#include <cstdint>
#include <utility>
#include <typeindex>
#include <boost/json.hpp>

#include "aliases.hpp"
#include "type_tag.hpp"
#include "type_info.hpp"


namespace zmbt {


class entity_id {

    boost::json::string key_;
    std::size_t type_index_hash_;
    std::size_t hash_;
    mutable boost::json::string annotation_;
    mutable boost::json::string str_;


  public:

    entity_id() = default;


    entity_id(boost::json::string_view key, std::type_index type_index);
    entity_id(boost::json::string_view key, std::size_t type_index_hash);

    explicit entity_id(boost::json::array const& str);
    explicit entity_id(boost::json::value const& val);

    entity_id(entity_id const&) = default;
    entity_id(entity_id &&) = default;
    entity_id& operator=(entity_id const&) = default;
    entity_id& operator=(entity_id &&) = default;
    virtual ~entity_id() = default;

    entity_id& operator=(boost::json::value const& v)
    {
        *this = entity_id(v);
        return *this;
    };

    entity_id& operator=(boost::json::value && v)
    {
        *this = entity_id(std::move(v));
        return *this;
    };



    bool operator==(entity_id const& other) const {
        return (type_index_hash_ == other.type_index_hash_) && (key_ == other.key_) ;
    }
    bool operator!=(entity_id const& other) const {
        return !this->operator==(other);
    }

    friend std::ostream& operator<<(std::ostream& os, entity_id const& id)
    {
        os << id.str();
        return os;
    }

    friend inline std::size_t hash_value(entity_id const& v)
    {
        return v.hash_;
    }

    boost::json::string_view key() const
    {
        return key_;
    }

    boost::json::string_view annotation() const;

    boost::json::string_view str() const;

    operator boost::json::value() const
    {
        return boost::json::array{key_, type_index_hash_};
    }
};


} // namespace zmbt

template <>
struct std::hash<zmbt::entity_id> {
    std::size_t operator()(zmbt::entity_id const& k) const
    {
        return hash_value(k);
    }
};


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
```


