/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_

#include "zmbt/model/expression_api.hpp"
#include "zmbt/mapping/model_definition.hpp"
#include "zmbt/mapping/model_definition_states.hpp"
#include "zmbt/mapping/model_definition_transitions.hpp"


namespace zmbt {
namespace mapping {

/// \brief Signal Mapping Test Model
/// \see \ref signal-mapping-model "Signal Mapping Model"
// using SignalMapping = ModelDefinition::N_Main;
class SignalMapping : public ModelDefinition::N_Main
{
  public:

    /**
     * @brief Begin the model definition
     *
     * @tparam T
     * @param name model name or format pattern string
     * @param args format arguments
     */
    template <class... T>
    SignalMapping(boost::json::string_view name, T&&... args) : N_Main()
    {
        this->state().set_deferred_param("/name", name | expr::Fmt(args...));
    }

    ~SignalMapping()
    {
        this->state().execute();
    }
};


}  // namespace mapping
}  // namespace zmbt

#endif
