/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/global_flags.hpp"

namespace zmbt {


std::atomic<bool>& flags::TestIsRunning::flag() noexcept
{
    // avoid destruction
    static auto* flag = new std::atomic<bool>(false);
    return *flag;
}

}
