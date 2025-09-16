/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_ENTITY_ID_HPP_
#define ZMBT_CORE_ENTITY_ID_HPP_

#include <cstdint>
#include <boost/json.hpp>

#include "aliases.hpp"


namespace zmbt {


/// Base class for annotated key objects
class entity_id {

    boost::json::string str_;
    boost::json::string_view key_;
    boost::json::string_view annotation_;
    std::size_t hash_;

    explicit entity_id(boost::json::string&& str);


  public:

    entity_id() = default;


    entity_id(boost::json::string_view key, boost::json::string_view type);
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
        return hash_ == other.hash_;
    }
    bool operator!=(entity_id const& other) const {
        return !this->operator==(other);
    }
    bool operator<(entity_id const& other) const {
        return hash_ < other.hash_;
    }
    bool operator>(entity_id const& other) const {
        return other.operator<(*this);
    }
    bool operator<=(entity_id const& other) const {
        return !other.operator<(*this);
    }
    bool operator>=(entity_id const& other) const {
        return !this->operator<(other);
    }

    friend std::ostream& operator<<(std::ostream& os, entity_id const& id)
    {
        os << id.str();
        return os;
    }

    /// Boost.Hash customization point
    friend inline std::size_t hash_value(entity_id const& v)
    {
        return v.hash_;
    }

    boost::json::string_view str() const
    {
        return str_;
    }

    boost::json::string_view key() const
    {
        return key_;
    }

    boost::json::string_view annotation() const
    {
        return annotation_;
    }

    operator boost::json::value() const
    {
        return str();
    }

};


} // namespace zmbt


#endif // ZMBT_CORE_INTERFACE_ID_HPP_
