/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_MACHINE_PP_HPP_
#define ZMBT_MAPPING_DEFINITION_MACHINE_PP_HPP_

#include <boost/preprocessor.hpp>

/// Transition<Source, Target>
#define ZMBT_MDM_TRANSITION_CLASS(seq)\
DefinitionMachine::BOOST_PP_SEQ_ELEM(0, seq)\
<DefinitionMachine::BOOST_PP_SEQ_ELEM(1, seq),\
DefinitionMachine::BOOST_PP_SEQ_ELEM(2, seq)>

/// Transform Transitions sequence by inserting State
#define ZMBT_MDM_TRANS_TRIPLES(State, Transitions) \
    BOOST_PP_SEQ_TRANSFORM(ZMBT_MDM_TRANS_TRIPLE_APPLY, State, Transitions)
#define ZMBT_MDM_TRANS_TRIPLE_APPLY(s, state, seq) BOOST_PP_SEQ_INSERT(seq, 1, state)

/// Transition as base class for state node
#define ZMBT_MDM_STATE_TRANSITION(z, N, seq) , public ZMBT_MDM_TRANSITION_CLASS(BOOST_PP_SEQ_ELEM(N, seq))

/// MDM State class definition
#define ZMBT_MDM_STATE_IMPL(State, Base, Transitions)  \
class DefinitionMachine::State                          \
    : public DefinitionMachine::Base                    \
    BOOST_PP_REPEAT(                                    \
        BOOST_PP_SEQ_SIZE(Transitions),                 \
        ZMBT_MDM_STATE_TRANSITION,                     \
        ZMBT_MDM_TRANS_TRIPLES(State, Transitions))    \
{                                                       \
  private:                                              \
    friend class DefinitionMachine;                     \
    State(DefinitionHelper& m) : Base(m) {}             \
    State(State const&) = delete;                       \
    State(State&&) = default;                           \
  public:                                               \
    ~State() { }                                        \
};

/// Generate state class definitions from transition table
#define ZMBT_MDM_GENERATE_STATES(table) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(table), ZMBT_MDM_GENERATE_STATES_APPLY, table)
#define ZMBT_MDM_GENERATE_STATES_APPLY(z, N, table) ZMBT_MDM_GENERATE_STATES_APPLY_IMPL(BOOST_PP_SEQ_ELEM(N, table))
#define ZMBT_MDM_GENERATE_STATES_APPLY_IMPL(row_seq) ZMBT_MDM_STATE_IMPL(BOOST_PP_SEQ_ELEM(0, row_seq), BOOST_PP_SEQ_ELEM(1, row_seq), BOOST_PP_SEQ_ELEM(2, row_seq))


/// Instantiate transition templates for one state
#define ZMBT_MDM_TEMPLATE_INST(macro, state_tt_row) ZMBT_MDM_TEMPLATE_INST_IMPL(macro, BOOST_PP_SEQ_ELEM(0, state_tt_row), BOOST_PP_SEQ_ELEM(2, state_tt_row))
#define ZMBT_MDM_TEMPLATE_INST_IMPL(macro, State, Transitions) \
BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(Transitions), macro, ZMBT_MDM_TRANS_TRIPLES(State, Transitions))

#define ZMBT_MDM_TEMPLATE_INST_REPOP(z, N, seq) template class ZMBT_MDM_TRANSITION_CLASS(BOOST_PP_SEQ_ELEM(N, seq));
#define ZMBT_MDM_TEMPLATE_EXTERN_REPOP(z, N, seq) extern template class ZMBT_MDM_TRANSITION_CLASS(BOOST_PP_SEQ_ELEM(N, seq));


/// Generate list of transition template explicit instantiations
#define ZMBT_MDM_GENERATE_TEMPLATE_INST(table) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(table), ZMBT_MDM_GENERATE_TEMPLATE_INST_APPLY, table)
#define ZMBT_MDM_GENERATE_TEMPLATE_INST_APPLY(z, N, table) ZMBT_MDM_TEMPLATE_INST(ZMBT_MDM_TEMPLATE_INST_REPOP, BOOST_PP_SEQ_ELEM(N, table))

/// Generate list of transition template explicit instantiations
#define ZMBT_MDM_GENERATE_TEMPLATE_EXTERN(table) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(table), ZMBT_MDM_GENERATE_TEMPLATE_EXTERN_APPLY, table)
#define ZMBT_MDM_GENERATE_TEMPLATE_EXTERN_APPLY(z, N, table) ZMBT_MDM_TEMPLATE_INST(ZMBT_MDM_TEMPLATE_EXTERN_REPOP, BOOST_PP_SEQ_ELEM(N, table))


/// Transition table row (state, base, (transition_1, source_1), ..., (transition_n), (source_n))
#define ZMBT_MDM_TT_ROW(State, Base, ...) ((State) (Base) ZMBT_MDM_TT_ROW_TRANSITIONS(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))
#define ZMBT_MDM_TT_ROW_TRANSITIONS(trs) (BOOST_PP_SEQ_TRANSFORM(ZMBT_MDM_TT_ROW_TRANSITIONS_TO_SEQ, NIL, trs))
#define ZMBT_MDM_TT_ROW_TRANSITIONS_TO_SEQ(s, data, pair) BOOST_PP_TUPLE_TO_SEQ(pair)



/**
 * How to use:
 *
 * Define MDM transition table using ZMBT_MDM_TT_ROW
 *
 * ZMBT_MDM_GENERATE_STATES
 *
 * ZMBT_MDM_GENERATE_TEMPLATE_INST in cpp
 *
 * ZMBT_MDM_GENERATE_TEMPLATE_EXTERN in hpp
 *
 * Maintain implementation for abstract transition classes
 */


// #define TT_ROW ZMBT_MDM_TT_ROW

// #define TT \
// TT_ROW(N_Lol, N_Kek, (T_Foo, N_Lol), (T_Bar, N_Kek)) \
// TT_ROW(N_Foo, N_Bar, (T_Bar, N_Foo), (T_Bar, N_Bar))


// // STATES
// ZMBT_MDM_GENERATE_STATES(TT)

// ZMBT_MDM_GENERATE_TEMPLATE_INST(TT)

// ZMBT_MDM_GENERATE_TEMPLATE_EXTERN(TT)


#endif
