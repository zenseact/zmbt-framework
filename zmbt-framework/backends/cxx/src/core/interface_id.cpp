/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>

#include "zmbt/core/interface_id.hpp"

template <>
struct std::hash<zmbt::interface_id>
{
    std::size_t operator()(const zmbt::interface_id& k) const
    {
        return k.hash();
    }
};
