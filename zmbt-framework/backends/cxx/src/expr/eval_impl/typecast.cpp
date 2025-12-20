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
    auto const enc = self().encoding_view();
    ASSERT(
        enc.arity() == 2,
        "invalid parameters, expected reference + Fn");
    auto const operator_reference = ExpressionView(enc.child(0)).eval({}, curr_ctx());
    ExpressionView const F(enc.child(1));

    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    EvalContext ctx = curr_ctx();
    ctx.op = O{*if_str};
    return F.eval_e(lhs(), ctx);
}

ZMBT_DEFINE_EVALUATE_IMPL(Cast)
{
    auto const operator_reference = rhs().eval({}, curr_ctx());
    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    auto const op = O{*if_str};
    return op.decorate(lhs().data());
}

ZMBT_DEFINE_EVALUATE_IMPL(Uncast)
{
    auto const operator_reference = rhs().eval({}, curr_ctx());
    auto const if_str = operator_reference.if_string();
    ASSERT(if_str, "invalid parameter");
    auto const op = O{*if_str};
    return op.undecorate(lhs().data());
}



} // namespace lang
} // namespace zmbt

