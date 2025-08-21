/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/application/config.hpp"
#include "zmbt/application/log.hpp"
#include "zmbt/application/test_failure.hpp"

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
