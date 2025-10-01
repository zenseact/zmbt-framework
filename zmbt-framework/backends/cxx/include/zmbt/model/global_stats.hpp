/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_GLOBAL_STATS_HPP_
#define ZMBT_MODEL_GLOBAL_STATS_HPP_

#include <atomic>
#include <chrono>
#include <cstdint>

namespace zmbt
{
namespace flags
{

class InjectionTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept
    {
        static auto* accumulator = new std::atomic<std::uint64_t>(0);
        return *accumulator;
    }
};

class RecordingTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept
    {
        static auto* accumulator = new std::atomic<std::uint64_t>(0);
        return *accumulator;
    }
};

class ConversionTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept
    {
        static auto* accumulator = new std::atomic<std::uint64_t>(0);
        return *accumulator;
    }
};

} // namespace flags
} // namespace zmbt

#endif
