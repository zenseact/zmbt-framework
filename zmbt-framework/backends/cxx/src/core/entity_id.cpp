/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
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

namespace {

class annotation_registry {
  public:
    annotation_registry();

    bool contains(std::size_t id) const;

    std::size_t ensure(std::size_t annotation_id, std::function<boost::json::string()>);
    std::size_t ensure(std::size_t id, boost::json::string_view annotation);
    std::size_t ensure(boost::json::string_view annotation);
    boost::json::string_view get(std::size_t id) const;

  private:

    struct annotation_storage
    {
        std::shared_ptr<boost::json::string> str_;

      public:

        annotation_storage() = default;

        annotation_storage(std::function<boost::json::string()> f)
            : str_(std::make_shared<boost::json::string>(f()))
        {
        }

        annotation_storage(boost::json::string_view v)
            : str_(std::make_shared<boost::json::string>(v))
        {
        }

        boost::json::string_view view() const
        {
            return *str_;
        }

    };

    boost::concurrent_flat_map<std::size_t, annotation_storage> id_to_annotation_;
};

annotation_registry::annotation_registry()
{
    id_to_annotation_.reserve(255);
}

std::size_t annotation_registry::ensure(std::size_t annotation_id, std::function<boost::json::string()> f)
{
    id_to_annotation_.emplace(annotation_id, f);
    return annotation_id;
}
std::size_t annotation_registry::ensure(std::size_t annotation_id, boost::json::string_view annotation)
{
    id_to_annotation_.emplace(annotation_id, annotation);
    return annotation_id;
}

std::size_t annotation_registry::ensure(boost::json::string_view annotation)
{
    return ensure(boost::hash_value(annotation), annotation);
}



boost::json::string_view annotation_registry::get(std::size_t id) const
{
    boost::json::string_view v;
    id_to_annotation_.cvisit(id, [&v](auto& record){
        v = record.second.view();
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

entity_id::entity_id(boost::json::string_view key, boost::json::string_view annotation)
    : entity_id(key, registry().ensure(annotation))
{
}

entity_id::entity_id(boost::json::string_view key, std::size_t annotation_id)
    : key_{key}
    , annotation_id_{annotation_id}
    , hash_{boost::hash_value(key_)}
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
        annotation_ = registry().get(annotation_id_);
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


std::size_t entity_id::ensure_annotation(std::size_t annotation_id, std::function<boost::json::string()> f)
{
    return registry().ensure(annotation_id, f);
}


} // namespace zmbt


template <>
struct std::hash<zmbt::entity_id> {
    std::size_t operator()(zmbt::entity_id const& k) const
    {
        return hash_value(k);
    }
};
