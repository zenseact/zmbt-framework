/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_

#include "zmbt/mapping/model_definition.hpp"
#include "zmbt/mapping/model_definition_states.hpp"
#include "zmbt/mapping/model_definition_transitions.hpp"


namespace zmbt {
namespace mapping {



class ModelDefinition::N_Main
    : protected ModelDefinition
    , public ModelDefinition::T_OnTrigger<ModelDefinition::N_Main, ModelDefinition::N_Channel>
{
    friend class ModelDefinition;

public:

    /**
     * @brief Begin the model definition
     *
     * @tparam T
     * @param name model name or format pattern string
     * @param args format arguments
     */
    template <class... T>
    N_Main(boost::json::string_view name, T&&... args) : ModelDefinition()
    {
        this->state().set_deferred_param("/name", boost::json::array{name, std::forward<T>(args)...});
    }

    ~N_Main()
    {
        this->state().execute();
    }
};



/// \brief Signal Mapping Test Model
/// \see \ref signal-mapping-model "Signal Mapping Model"
using SignalMapping = ModelDefinition::N_Main;

}  // namespace mapping
}  // namespace zmbt

#endif
