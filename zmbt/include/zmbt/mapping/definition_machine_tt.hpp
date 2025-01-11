/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_MACHINE_TT_HPP_
#define ZMBT_MAPPING_DEFINITION_MACHINE_TT_HPP_

#include "zmbt/mapping/definition_machine_pp.hpp"

#define ZMBT_MDM_TRANSITION_TABLE                                         \
ZMBT_MDM_TT_ROW(N_Term      , N_Node     , (T_Null       , N_Node      )) \
ZMBT_MDM_TT_ROW(N_Descr     , N_Term     , (T_Description, N_Term      )) \
ZMBT_MDM_TT_ROW(N_Post      , N_Descr    , (T_PostRun    , N_Descr     )) \
ZMBT_MDM_TT_ROW(N_Pre       , N_Post     , (T_PreRun     , N_Post      )) \
ZMBT_MDM_TT_ROW(N_ParamTable, N_Pre      , (T_ParamRow   , N_ParamTable), \
                                            (T_Zip        , N_ParamTable), \
                                            (T_Prod       , N_ParamTable)) \
ZMBT_MDM_TT_ROW(N_Param     , N_Pre      , (T_Zip        , N_ParamTable), \
                                            (T_Prod       , N_ParamTable)) \
ZMBT_MDM_TT_ROW(N_TestTable , N_Param    , (T_TestRow    , N_TestTable ), \
                                            (T_TestComment, N_TestTable )) \
ZMBT_MDM_TT_ROW(N_Test      , N_Node     , (T_Test       , N_TestTable )) \
ZMBT_MDM_TT_ROW(N_Channel   , N_Test     , (T_InjectTo   , N_KindIn    ), \
                                            (T_ObserveOn  , N_KindOut   )) \
ZMBT_MDM_TT_ROW(N_ChannelOut, N_Test     , (T_ObserveOn  , N_KindOut   )) \
ZMBT_MDM_TT_ROW(N_Combine   , N_Channel  , (T_InSeries   , N_ChannelOut), \
                                            (T_Join       , N_ChannelOut)) \
ZMBT_MDM_TT_ROW(N_Alias     , N_Channel  , (T_Alias      , N_Channel   )) \
ZMBT_MDM_TT_ROW(N_AliasOut  , N_Combine  , (T_Alias      , N_Combine   )) \
ZMBT_MDM_TT_ROW(N_Call      , N_Alias    , (T_CallRangeIn, N_Alias     ), \
                                            (T_OnCall     , N_Alias     )) \
ZMBT_MDM_TT_ROW(N_CallOut   , N_AliasOut , (T_CallRange  , N_AliasOut  ), \
                                            (T_OnCall     , N_AliasOut  )) \
ZMBT_MDM_TT_ROW(N_Decor     , N_Call     , (T_As         , N_Call      )) \
ZMBT_MDM_TT_ROW(N_DecorOut  , N_CallOut  , (T_As         , N_CallOut   )) \
ZMBT_MDM_TT_ROW(N_KindIn    , N_Decor    , (T_Return     , N_Decor     ), \
                                            (T_Args       , N_Decor     ), \
                                            (T_Exception  , N_Call      )) \
ZMBT_MDM_TT_ROW(N_KindOut   , N_DecorOut , (T_Return     , N_DecorOut  ), \
                                            (T_Args       , N_DecorOut  ), \
                                            (T_Exception  , N_CallOut   ), \
                                            (T_CallCount  , N_AliasOut  ), \
                                            (T_Timestamp  , N_CallOut   ), \
                                            (T_ThreadId   , N_CallOut   ))

// Entry point N_Main with T_OnTrigger transition is defined separately
// with default ctor

#endif
