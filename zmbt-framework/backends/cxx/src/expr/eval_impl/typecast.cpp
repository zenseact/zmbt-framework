/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include <limits>
#include <boost/math/constants/constants.hpp>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"

namespace
{
using V = boost::json::value;
using L = boost::json::array;
using E = zmbt::lang::Expression;
using K = zmbt::lang::Keyword;
using O = zmbt::lang::Operator;

} // namespace


namespace zmbt {
namespace lang {



ZMBT_DEFINE_EVALUATE_IMPL(Op)
{
    auto const pl = self().parameter_list();

    auto const fork = self().encoding_view().child(0);
    ASSERT(
        (fork.head() == K::Fork || fork.head() == K::Pack)
        && fork.arity() == 2,
    "invalid parameters, expected reference + Fn");
    auto const operator_reference = E(fork.child(0).freeze()).eval({}, curr_ctx() MAYBE_INCR);
    auto const F = E(fork.child(1).freeze());

    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    EvalContext ctx = curr_ctx() MAYBE_INCR;
    ctx.op = O{*if_str};
    auto result = F.eval(lhs(), ctx);

    return (F.is_const() && !F.is_boolean()) ? ctx.op.decorate(result) : result;
}

ZMBT_DEFINE_EVALUATE_IMPL(Cast)
{
    auto const operator_reference = rhs().eval({}, curr_ctx() MAYBE_INCR);
    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    auto const op = O{*if_str};
    return op.decorate(lhs());
}

ZMBT_DEFINE_EVALUATE_IMPL(Uncast)
{
    auto const operator_reference = rhs().eval({}, curr_ctx() MAYBE_INCR);
    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    auto const op = O{*if_str};
    return op.undecorate(lhs());
}



} // namespace lang
} // namespace zmbt

