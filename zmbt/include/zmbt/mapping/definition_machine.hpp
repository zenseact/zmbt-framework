/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_MACHINE_HPP_
#define ZMBT_MAPPING_DEFINITION_MACHINE_HPP_

#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/deferred_format.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/core/parameter.hpp>
#include <zmbt/model/expression.hpp>
#include <zmbt/model/signal_operator_handler.hpp>
#include <zmbt/model/traits.hpp>
#include <cstddef>
#include <tuple>
#include <utility>

#include "definition_helper.hpp"
#include "zmbt/mapping/definition_machine_tt.hpp"



namespace zmbt {
namespace mapping {

/// Mapping model definition machine
class DefinitionMachine
{
    DefinitionHelper mapping_;

  public:
    DefinitionMachine() : mapping_{} {}
    DefinitionMachine(DefinitionHelper& mapping) : mapping_{mapping} {}

    DefinitionHelper& state()
    {
        return mapping_;
    }
  protected:

    // TRANSITIONS

    template <class Source, class Target>
    struct T_OnTrigger;
    template <class Source, class Target>
    struct T_InjectTo;
    template <class Source, class Target>
    struct T_ObserveOn;

    template <class Source, class Target>
    struct T_Return;
    template <class Source, class Target>
    struct T_Args;
    template <class Source, class Target>
    struct T_Exception;

    template <class Source, class Target>
    struct T_Timestamp;
    template <class Source, class Target>
    struct T_ThreadId;

    template <class Source, class Target>
    struct T_As;
    template <class Source, class Target>
    struct T_OnCall;
    template <class Source, class Target>
    struct T_CallRange;

    template <class Source, class Target>
    struct T_CallRangeIn;

    template <class Source, class Target>
    struct T_CallCount;

    template <class Source, class Target>
    struct T_Alias;

    template <class Source, class Target>
    struct T_InSeries;

    template <class Source, class Target>
    struct T_Join;

    template <class Source, class Target>
    struct T_Test;
    template <class Source, class Target>
    struct T_TestRow;
    template <class Source, class Target>
    struct T_TestComment;

    template <class Source, class Target>
    struct T_Zip;
    template <class Source, class Target>
    struct T_Prod;
    template <class Source, class Target>
    struct T_ParamRow;

    template <class Source, class Target>
    struct T_PreRun;
    template <class Source, class Target>
    struct T_PostRun;

    template <class Source, class Target>
    struct T_Description;

    /// @brief  Empty transition
    /// @tparam Source
    /// @tparam Target
    template <class Source, class Target>
    struct T_Null {};


    // NODES

    class N_DecorOut;
    class N_CallOut;
    class N_AliasIn;
    class N_AliasOut;
    class N_Combine;
    class N_Input;
    class N_ChannelOut;
    class N_Node;
    class N_Term;
    class N_Descr;
    class N_Post;
    class N_Pre;
    class N_ParamTable;
    class N_Param;
    class N_TestTable;
    class N_Test;
    class N_Channel;
    class N_Alias;
    class N_Call;
    class N_Decor;
    class N_KindIn;
    class N_KindOut;
    class N_Main;

    friend class SignalMapping;

};


}  // namespace mapping
}  // namespace zmbt

#endif
