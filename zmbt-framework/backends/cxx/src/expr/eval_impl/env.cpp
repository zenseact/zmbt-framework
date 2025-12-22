/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/global_env.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"



namespace zmbt {
namespace lang {


ZMBT_DEFINE_EVALUATE_IMPL(EnvLoad)
{
    auto const rhs_eval = rhs().eval_e({}, curr_ctx());
    auto const if_str_key = rhs_eval.if_string();
    ASSERT(if_str_key, "reference is not a string");

    return GlobalEnv().Load(*if_str_key);
}


ZMBT_DEFINE_EVALUATE_IMPL(EnvStore)
{
    auto const rhs_eval = rhs().eval_e({}, curr_ctx());
    auto const if_str_key = rhs_eval.if_string();
    ASSERT(if_str_key, "reference is not a string");

    return GlobalEnv().Store(*if_str_key, lhs().data());
}

} // namespace lang
} // namespace zmbt

