/**
 * \file
 * \copyright (c) Copyright 2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * app: codegen.expr
 * template: codegen/mdm/templates/model_definition_transition_templates.cpp
 *
 * How to update:
 * 1. Update the template
 * 2. Run `python -m codegen` in the project root
 * 3. Commit changes
 */
#include "zmbt/mapping/model_definition_states.hpp"
#include "zmbt/mapping/model_definition_transitions.hpp"


namespace zmbt {
namespace mapping {

template class ModelDefinition::T_Description<ModelDefinition::N_Term>;
template class ModelDefinition::T_PostRun<ModelDefinition::N_Descr>;
template class ModelDefinition::T_PreRun<ModelDefinition::N_Post>;
template class ModelDefinition::T_Param<ModelDefinition::N_ParamTable>;
template class ModelDefinition::T_CustomParam<ModelDefinition::N_Param>;
template class ModelDefinition::T_ParamRow<ModelDefinition::N_ParamTable>;
template class ModelDefinition::T_Test<ModelDefinition::N_TestTable>;
template class ModelDefinition::T_TestRow<ModelDefinition::N_TestTable>;
template class ModelDefinition::T_At<ModelDefinition::N_Filter>;
template class ModelDefinition::T_TerminatePipe<ModelDefinition::N_MaybeChannel>;
template class ModelDefinition::T_ContinuePipe<ModelDefinition::N_Channel>;
template class ModelDefinition::T_As<ModelDefinition::N_ChannelEnd>;
template class ModelDefinition::T_Alias<ModelDefinition::N_Decor>;
template class ModelDefinition::T_Take<ModelDefinition::N_Alias>;
template class ModelDefinition::T_Filter<ModelDefinition::N_Take>;
template class ModelDefinition::T_Repeat<ModelDefinition::N_MaybeChannel>;
template class ModelDefinition::T_OnTrigger<ModelDefinition::N_Repeat>;

}  // namespace mapping
}  // namespace zmbt
