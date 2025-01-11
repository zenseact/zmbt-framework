/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>
#include <ostream>
#include <string>

#include "zmbt/core/json_pretty_print.hpp"


std::ostream& zmbt::pretty_print(std::ostream& os, boost::json::value const& jv, std::string indent)
{
    // TODO: implement pretty print
    os << boost::json::serialize(jv);
    return os;
}
