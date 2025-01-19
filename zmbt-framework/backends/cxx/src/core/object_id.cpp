/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>
#include <cstddef>
#include <sstream>
#include <typeindex>

#include "zmbt/core/object_id.hpp"

boost::json::string zmbt::object_id::to_string(void const* ptr)
{
    std::stringstream ss;
    ss << ptr;
    return ss.str().c_str();
}

template <>
struct std::hash<zmbt::object_id>
{
  std::size_t operator()(const zmbt::object_id& k) const
  {
    using std::hash;
    return hash<boost::json::string>()(k.str());
  }
};
