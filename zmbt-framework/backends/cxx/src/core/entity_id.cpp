/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/core/entity_id.hpp"

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/assert.hpp>
#include <boost/json.hpp>
#include <boost/container_hash/hash.hpp>
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK
#include <boost/unordered/concurrent_flat_map.hpp>

#include "zmbt/core/format_string.hpp"
#include "zmbt/core/shared_resource.hpp"

namespace {

class annotation_registry {
  public:
    annotation_registry();

    bool contains(std::size_t id) const;

    std::size_t ensure(std::type_index type_index);
    std::type_index get(std::size_t id) const;

  private:


    boost::concurrent_flat_map<std::size_t, zmbt::shared_resource<std::type_index>> id_to_annotation_;
};

annotation_registry::annotation_registry()
{
    id_to_annotation_.reserve(255);
}

std::size_t annotation_registry::ensure(std::type_index type_index)
{
    id_to_annotation_.try_emplace(type_index.hash_code(), zmbt::emplace_shared, type_index);
    return type_index.hash_code();
}



std::type_index annotation_registry::get(std::size_t id) const
{
    std::type_index v{typeid(void)};
    id_to_annotation_.cvisit(id, [&v](auto& record){
        v = *record.second;
    });
    return v;
}

annotation_registry& registry()
{
    static annotation_registry instance;
    return instance;
}

}  // namespace


namespace zmbt {

entity_id::entity_id(boost::json::string_view key, std::type_index type_index)
    : entity_id(key, registry().ensure(type_index))
{
}

entity_id::entity_id(boost::json::string_view key, std::size_t type_index_hash)
    : key_{key}
    , type_index_hash_{type_index_hash}
    , hash_{boost::hash_value(std::make_pair(type_index_hash_, key_))}
{
}

entity_id::entity_id(boost::json::array const& arr)
    : entity_id(arr.at(0).as_string(), arr.at(1).as_uint64())
{
}

entity_id::entity_id(boost::json::value const& val)
    : entity_id(val.as_array())
{
}

boost::json::string_view entity_id::annotation() const
{
    if (annotation_.empty())
    {
        annotation_ = boost::core::demangle(registry().get(type_index_hash_).name());
    }
    return annotation_;
}

boost::json::string_view entity_id::str() const
{
    if (str_.empty() && !key_.empty())
    {
        auto formatted = format("[%s](%s)", key(), annotation());
        str_ = formatted;
    }
    return str_;
}

} // namespace zmbt
