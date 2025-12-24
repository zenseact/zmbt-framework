/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_TEST_PARAMETER_RESOLVER_HPP_
#define ZMBT_MAPPING_TEST_PARAMETER_RESOLVER_HPP_

#include <zmbt/core/json_iter.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/model/environment.hpp>
#include <list>
#include <memory>

namespace zmbt {
namespace mapping {

/// Test spec generator that handles parameter resolution
class TestParameterResolver
{
    JsonNode model_;
    std::list<std::shared_ptr<JsonIter>> iterators_{};
    Environment env{};

    void log_debug(JsonNode next_model);
    void resolve_deferred_params(JsonNode& next_model);
    void init_param_iters();

public:

    TestParameterResolver(JsonNode const& model);
    JsonNode Next();
};


} // namespace mapping
} // namespace zmbt

#endif // ZMBT_MAPPING_RESOLVER_HPP_
