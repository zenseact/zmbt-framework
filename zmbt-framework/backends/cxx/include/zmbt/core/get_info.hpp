/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_GET_INFO_HPP_
#define ZMBT_CORE_GET_INFO_HPP_


namespace zmbt
{

/// get thread id in string
std::string get_tid();

/// get timestamp
std::size_t get_ts();

} // namespace zmbt

#endif
