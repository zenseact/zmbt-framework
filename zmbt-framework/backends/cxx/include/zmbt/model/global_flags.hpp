/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_GLOBAL_FLAGS_HPP_
#define ZMBT_MODEL_GLOBAL_FLAGS_HPP_

#include <atomic>

namespace zmbt
{
namespace flags
{

class TestIsRunning
{
  public:
    static void set() noexcept
    {
        flag().store(true, std::memory_order_release);
    }

    static void clear() noexcept
    {
        flag().store(false, std::memory_order_release);
    }

    static bool status() noexcept
    {
        return flag().load(std::memory_order_acquire);
    }

    static bool exchange(bool const sts) noexcept
    {
        return flag().exchange(sts, std::memory_order_acquire);
    }

private:
    static std::atomic<bool>& flag() noexcept
    {
        // avoid destruction
        static auto* flag = new std::atomic<bool>(false);
        return *flag;
    }
};

} // namespace flags
} // namespace zmbt

#endif
