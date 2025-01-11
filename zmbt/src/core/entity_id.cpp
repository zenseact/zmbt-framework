/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */



#include <boost/json.hpp>
#include <cstddef>
#include <typeindex>

#include "zmbt/core/entity_id.hpp"
#include "zmbt/core/format_string.hpp"

namespace {

boost::json::string get_addr(boost::json::string_view id_str)
{
    return id_str.substr(1, id_str.find(']')-1);
}

boost::json::string get_type(boost::json::string_view id_str)
{
    auto const pos = id_str.find('(') + 1;
    auto const n = id_str.find_last_of(')') - pos;
    return id_str.substr(pos, n);
}

} // namespace


zmbt::entity_id::entity_id(boost::json::string_view key, boost::json::string_view type)
    : key_{key}
    , annotation_{type}
    , str_{format("[%s](%s)", key, type)}
{
}

zmbt::entity_id::entity_id(boost::json::value const& val)
    : key_{get_addr(val.as_string())}
    , annotation_{get_type(val.as_string())}
    , str_{val.as_string()}
{
}


template <>
struct std::hash<zmbt::entity_id>
{
  std::size_t operator()(const zmbt::entity_id& k) const
  {
    using std::hash;
    return hash<boost::json::string>()(k.key());
  }
};
