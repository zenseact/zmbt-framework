/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/core/entity_id.hpp"

#include <boost/json.hpp>
#include <cstddef>
#include <typeindex>

#include "zmbt/core/format_string.hpp"

namespace {

boost::json::string_view get_key_view(boost::json::string_view str)
{
    return str.substr(1, str.find(']') - 1);
}

boost::json::string_view get_annotation_view(boost::json::string_view str)
{
    auto const pos = str.find('(') + 1;
    auto const n = str.find_last_of(')') - pos;
    return str.substr(pos, n);
}

}  // namespace

zmbt::entity_id::entity_id(boost::json::string&& str)
    : str_{std::move(str)}
    , key_{get_key_view(str_)}
    , annotation_{get_annotation_view(str_)}
    , hash_{[key=key_]() -> std::size_t {
        using std::hash;
        return hash<boost::json::string>()(key);
    }()}
{
}

zmbt::entity_id::entity_id(boost::json::string_view key, boost::json::string_view type)
    : entity_id(boost::json::string{format("[%s](%s)", key, type).c_str()})
{
}

zmbt::entity_id::entity_id(boost::json::value const& val)
    : entity_id(boost::json::string{val.as_string()})
{
}

template <>
struct std::hash<zmbt::entity_id> {
    std::size_t operator()(zmbt::entity_id const& k) const
    {
        return k.hash();
    }
};
