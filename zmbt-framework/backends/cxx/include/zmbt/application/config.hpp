/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_APPLICATION_CONFIG_HPP_
#define ZMBT_APPLICATION_CONFIG_HPP_


#include <boost/json.hpp>
#include <functional>
#include <memory>

#include "test_failure.hpp"


namespace zmbt {

/// Global app config
class Config final
{
public:
    using FailureHandler = std::function<void(boost::json::value const&)>;

    Config();

    Config(Config const&) = default;
    Config(Config &&) = default;
    Config& operator=(Config const&) = default;
    Config& operator=(Config &&) = default;

    ~Config() = default;

    /// Set custom test failure handler
    Config& SetFailureHandler(FailureHandler const& fn);

    /// Reset the test handler to default
    Config& ResetFailureHandler();

    Config& HandleTestFailure(boost::json::value const& diagnostics);

private:
    struct PersistentConfig
    {
        FailureHandler failure_handler {&zmbt::default_test_failure};
    };

    std::shared_ptr<PersistentConfig> config_;
};

}

#endif // ZMBT_APPLICATION_CONFIG_HPP_
