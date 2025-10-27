/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/global_stats.hpp"

namespace zmbt {


std::atomic<std::uint64_t>& flags::InjectionTime::counter() noexcept
{
    static auto* accumulator = new std::atomic<std::uint64_t>(0);
    return *accumulator;
}

std::atomic<std::uint64_t>& flags::RecordingTime::counter() noexcept
{
    static auto* accumulator = new std::atomic<std::uint64_t>(0);
    return *accumulator;
}

std::atomic<std::uint64_t>& flags::ConversionTime::counter() noexcept
{
    static auto* accumulator = new std::atomic<std::uint64_t>(0);
    return *accumulator;
}

}
