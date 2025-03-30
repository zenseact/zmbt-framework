/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_

#include "zmbt/mapping/model_definition.hpp"


namespace zmbt {
namespace mapping {


class ModelDefinition::N_Node : protected ModelDefinition
{
  private:
    friend class ModelDefinition;
    N_Node(detail::DefinitionHelper& m) : ModelDefinition(m) {}
    N_Node() : ModelDefinition() {}
    N_Node(N_Node const&) = delete;
    N_Node(N_Node&&) = default;
  public:
    ~N_Node() { };
};


}  // namespace mapping
}  // namespace zmbt

#endif
