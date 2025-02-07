/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <chrono>
#include <sstream>
#include <thread>

#include "zmbt/core/get_info.hpp"

namespace zmbt
{
std::string get_tid()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

std::size_t get_ts()
{
    using std::chrono::nanoseconds;
    return static_cast<std::size_t>(std::chrono::duration_cast<nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
}

} // namespace zmbt
