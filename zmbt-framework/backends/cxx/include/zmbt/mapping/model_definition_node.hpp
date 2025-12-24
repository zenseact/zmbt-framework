/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_

#include "zmbt/mapping/model_definition.hpp"


namespace zmbt {
namespace mapping {


class ModelDefinition::BaseTransition
{
  detail::DefinitionHelper mapping_;

  public:
    virtual ~BaseTransition() { };

  protected:

    BaseTransition(detail::DefinitionHelper && m) : mapping_{m} {}
    BaseTransition() : BaseTransition(detail::DefinitionHelper{}) {}
    BaseTransition(BaseTransition const&) = delete;
    BaseTransition(BaseTransition&&) = default;

    detail::DefinitionHelper& state()
    {
        return mapping_;
    }

    template <class Target>
    Target transit_to()
    {
        return Target(std::move(mapping_));
    }
};


class ModelDefinition::N_Term : protected virtual ModelDefinition::BaseTransition
{
  private:
    friend class ModelDefinition;
    N_Term(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Term(N_Term const&) = delete;
    N_Term(N_Term&&) = default;

  public:
  N_Term() : N_Term(detail::DefinitionHelper{}) {}
    virtual ~N_Term()
    {
    }
};

}  // namespace mapping
}  // namespace zmbt

#endif
