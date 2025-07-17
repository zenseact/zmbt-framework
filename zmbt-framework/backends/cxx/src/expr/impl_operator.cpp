/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/logging.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/exceptions.hpp"
#include "zmbt/expr/keyword_codegen_type.hpp"


namespace
{

using V = boost::json::value;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;
}

namespace zmbt {
namespace lang {

boost::json::value Expression::eval_UnaryOp(boost::json::value const& x, EvalContext const& ctx) const
{
    return ctx.op.apply(keyword(), nullptr, Expression(x).eval(nullptr, ctx++));
}

boost::json::value Expression::eval_BinaryOp(boost::json::value const& x, EvalContext const& ctx) const
{
    V lhs {}; // binary LHS or expr params
    V rhs {}; // binary RHS or expr arg

    handle_binary_args(x, lhs, rhs);
    // if (not (lhs and rhs))
    // {
        // return detail::make_error_expr("invalid operands", prettify());
    // }
    return ctx.op.apply(keyword(), Expression(lhs).eval(nullptr, ctx++), Expression(rhs).eval(nullptr, ctx++));
}


} // namespace lang
} // namespace zmbt

