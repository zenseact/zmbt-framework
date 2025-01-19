/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_TEST_RUNNER_HPP_
#define ZMBT_MAPPING_TEST_RUNNER_HPP_

#include <memory>
#include "zmbt/core.hpp"

namespace zmbt {
namespace mapping {

/// Test runner for SignalMapping model
class TestRunner
{
    class Impl;

    std::unique_ptr<Impl> impl_;

public:
    TestRunner(JsonNode const& model);
    void Run();
    ~TestRunner();
};

} // namespace mapping
} // namespace zmbt

#endif
