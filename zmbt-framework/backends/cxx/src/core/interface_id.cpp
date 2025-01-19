/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <typeindex>

#include "zmbt/core/interface_id.hpp"

boost::json::string
zmbt::interface_id::bytes_to_str(std::uint8_t (&bytes)[zmbt::interface_id::mfp_size])
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (auto const c: bytes)
    {
        ss  << std::setw(2) << (0xFFU & static_cast<std::uint32_t>(c));
    }
    return ss.str().c_str();
}

template <>
struct std::hash<zmbt::interface_id>
{
  std::size_t operator()(const zmbt::interface_id& k) const
  {
    using std::hash;
    return hash<boost::json::string>()(k.key());
  }
};
