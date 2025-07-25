/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * app: codegen.expr
 * template: codegen/expr/templates/zmbt-framework/backends/cxx/src/expr/eval_autogen/operators.cpp
 *
 * How to update:
 * 1. Update the template
 * 2. Run `python -m codegen` in the project root
 * 3. Commit changes
 */


#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/keyword.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"


namespace
{
using Keyword = zmbt::lang::Keyword;
using Expression = zmbt::lang::Expression;
using EvalContext = zmbt::lang::EvalContext;

Expression apply_op(Keyword const k, Expression const& lhs, Expression const& rhs, EvalContext const& ctx)
{
    if (lhs.is_error())
    {
        return lhs;
    }
    else if(rhs.is_error())
    {
        return rhs;
    }
    else
    {
        return ctx.op.apply(k, {lhs, ctx}, {rhs, ctx});
    }
}

}

namespace zmbt {
namespace lang {

ZMBT_DEFINE_EVALUATE_IMPL(Neg) { return apply_op(Keyword::Neg, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Add) { return apply_op(Keyword::Add, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Sub) { return apply_op(Keyword::Sub, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Mul) { return apply_op(Keyword::Mul, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Div) { return apply_op(Keyword::Div, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Pow) { return apply_op(Keyword::Pow, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Log) { return apply_op(Keyword::Log, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Mod) { return apply_op(Keyword::Mod, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Quot) { return apply_op(Keyword::Quot, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(BitNot) { return apply_op(Keyword::BitNot, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(BitAnd) { return apply_op(Keyword::BitAnd, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(BitOr) { return apply_op(Keyword::BitOr, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(BitXor) { return apply_op(Keyword::BitXor, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Lshift) { return apply_op(Keyword::Lshift, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Rshift) { return apply_op(Keyword::Rshift, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Eq) { return apply_op(Keyword::Eq, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Ne) { return apply_op(Keyword::Ne, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Lt) { return apply_op(Keyword::Lt, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Le) { return apply_op(Keyword::Le, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Gt) { return apply_op(Keyword::Gt, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Ge) { return apply_op(Keyword::Ge, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(SetEq) { return apply_op(Keyword::SetEq, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Subset) { return apply_op(Keyword::Subset, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Superset) { return apply_op(Keyword::Superset, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(PSubset) { return apply_op(Keyword::PSubset, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(PSuperset) { return apply_op(Keyword::PSuperset, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(In) { return apply_op(Keyword::In, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(NotIn) { return apply_op(Keyword::NotIn, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Ni) { return apply_op(Keyword::Ni, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(NotNi) { return apply_op(Keyword::NotNi, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Bool) { return apply_op(Keyword::Bool, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Not) { return apply_op(Keyword::Not, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(And) { return apply_op(Keyword::And, lhs(), rhs(), curr_ctx()); }
ZMBT_DEFINE_EVALUATE_IMPL(Or) { return apply_op(Keyword::Or, lhs(), rhs(), curr_ctx()); }


} // namespace lang
} // namespace zmbt

