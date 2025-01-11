/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_MACHINE_STATES_HPP_
#define ZMBT_MAPPING_DEFINITION_MACHINE_STATES_HPP_

#include "zmbt/mapping/definition_machine.hpp"
#include "zmbt/mapping/definition_machine_transitions.hpp"


namespace zmbt {
namespace mapping {


class DefinitionMachine::N_Node
    : protected DefinitionMachine
{
private:
    friend class DefinitionMachine;
    N_Node(DefinitionHelper& m) : DefinitionMachine(m) {}
    N_Node(N_Node const&) = delete;
    N_Node(N_Node&&) = default;
  public:
    ~N_Node() { };
};


class DefinitionMachine::N_Main
    : protected DefinitionMachine
    , public DefinitionMachine::T_OnTrigger<DefinitionMachine::N_Main, DefinitionMachine::N_Channel>
{
    friend class DefinitionMachine;
    friend class SignalMapping;
    N_Main() : DefinitionMachine() {}

public:
    ~N_Main()
    {
        this->state().execute();
    }
};



/// DefinitionHelper definition machine entry point
class SignalMapping : public DefinitionMachine::N_Main
{
public:

    /**
     * @brief Construct the DefinitionHelper definition machine entry point
     *
     * @tparam T
     * @param name model name or format pattern string
     * @param args format arguments
     */
    template <class... T>
    SignalMapping(boost::json::string_view name, T&&... args) : DefinitionMachine::N_Main()
    {
        this->state().set_deferred_param("/name", boost::json::array{name, std::forward<T>(args)...});
    }
};


ZMBT_MDM_GENERATE_STATES(ZMBT_MDM_TRANSITION_TABLE)

}  // namespace mapping
}  // namespace zmbt

#endif
