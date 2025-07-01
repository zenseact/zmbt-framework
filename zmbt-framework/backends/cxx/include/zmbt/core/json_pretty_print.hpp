/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_JSON_PRETTY_PRINT_HPP_
#define ZMBT_CORE_JSON_PRETTY_PRINT_HPP_

#include "aliases.hpp"


namespace zmbt {


/**
 * @brief Pretty print JSON data
 *
 * @param os
 * @param jv
 * @param indent initial indent
 * @return std::ostream&
 */
std::ostream& pretty_print(std::ostream& os, boost::json::value const& jv, int const indent = 0);



} // namespace zmbt

#endif // ZMBT_CORE_JSON_PRETTY_PRINT_HPP_
