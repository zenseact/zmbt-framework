/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/application/config.hpp"
#include "zmbt/application/log.hpp"
#include "zmbt/application/test_failure.hpp"
#include <mutex>

namespace
{

std::uint64_t splitmix64(std::uint64_t x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

}

namespace zmbt {

Config::Config()
{
    static std::mutex ctor_mt;
    std::lock_guard<std::mutex> guard(ctor_mt);

    static std::shared_ptr<PersistentConfig> config;
    config_ = config;
    if (not config_)
    {
        config_ = std::make_shared<PersistentConfig>();
        config = config_;
    }
}

std::uint64_t Config::RngSeed() const
{
    return config_->rng_seed;
}

Config& Config::SetRngSeed(std::uint64_t const seed)
{
    config_->rng_seed = seed;
    ResetRng();
    return *this;
}

Config& Config::ResetRng()
{
    config_->rng_counter.store(0, std::memory_order_relaxed);
    config_->rng_epoch.fetch_add(1, std::memory_order_acq_rel);
    return *this;
}

std::mt19937& Config::Rng()
{
    auto& cfg = *config_;
    thread_local std::uint64_t cached_epoch{~std::uint64_t{}};
    thread_local std::mt19937 gen{};

    auto const epoch = cfg.rng_epoch.load(std::memory_order_acquire);
    if (cached_epoch != epoch)
    {
        auto const seed = splitmix64(cfg.rng_seed + cfg.rng_counter.fetch_add(1, std::memory_order_relaxed));
        gen.seed(static_cast<std::mt19937::result_type>(seed));
        cached_epoch = epoch;
    }
    return gen;
}

Config& Config::SetFailureHandler(FailureHandler const& fn)
{
    config_->failure_handler = fn;
    return *this;
}

Config& Config::ResetFailureHandler()
{
    config_->failure_handler = default_test_failure;
    return *this;
}

Config& Config::HandleTestFailure(boost::json::value const& diagnostics)
{
    ZMBT_LOG_JSON(WARNING) << diagnostics;
    config_->failure_handler(diagnostics);
    return *this;
}

} // namespace zmbt
