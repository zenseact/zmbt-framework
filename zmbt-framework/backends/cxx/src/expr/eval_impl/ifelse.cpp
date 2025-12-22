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

E if_continue(
    zmbt::lang::ExpressionView const& arg,
    zmbt::lang::ExpressionView const& F,
    zmbt::lang::ExpressionView const& then_value,
    zmbt::lang::EvalContext ctx
)
{
    auto keyword = K::_Continue;
    zmbt::lang::Expression payload = arg;

    E sts;
    if (F.eval_as_predicate(payload, sts, ctx))
    {
        keyword = K::_Resolve;
        payload = then_value.eval_e(payload, ctx);
    }
    else if (sts.is_error())
    {
        payload = sts;
    }
    return E::encodeNested(keyword, {payload});
}

} // namespace


namespace zmbt {
namespace lang {


ZMBT_DEFINE_EVALUATE_IMPL(If)
{
    auto const subexpressions = self().subexpressions_list();
    auto const N = subexpressions.size();
    ASSERT((N == 2) || (N == 3),
    "invalid parameters, expected (predicate, then_expr) or (predicate, then_expr, else_expr)");
    ExpressionView const& arg = x();
    ExpressionView const& F = subexpressions.at(0);
    ExpressionView const& then_expr = subexpressions.at(1);

    E const result = if_continue(arg, F, then_expr, curr_ctx());
    if (N == 3)
    {
        switch (result.keyword())
        {
        case K::_Resolve:
            return result.data();
        case K::_Continue:
            return subexpressions.at(2).eval_e(arg, curr_ctx()); // else_expr
        default:
            return result;
        }
    }
    return result;
}

ZMBT_DEFINE_EVALUATE_IMPL(Elif)
{
    if(x().is(Keyword::_Resolve))
    {
        return x();
    }
    ASSERT(x().is(Keyword::_Continue), "missing If");

    auto const subexpressions = self().subexpressions_list();

    ASSERT(subexpressions.size() == 2,
    "invalid parameters, expected (predicate, then_value)");
    Expression payload = x().data();
    return if_continue(payload, subexpressions.at(0), subexpressions.at(1), curr_ctx());
}

ZMBT_DEFINE_EVALUATE_IMPL(Else)
{
    if(x().is(Keyword::_Resolve))
    {
        return x().data();
    }
    else if (x().is(Keyword::_Continue))
    {
        return rhs().eval(x().data(), curr_ctx());
    }
    else
    {
        ASSERT(false, "missing If");
    }
    return nullptr;
}



} // namespace lang
} // namespace zmbt

