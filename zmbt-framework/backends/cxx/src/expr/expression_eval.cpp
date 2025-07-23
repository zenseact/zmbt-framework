/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_impl.hpp"

namespace zmbt {
namespace lang {

boost::json::value Expression::eval(Expression const& x, EvalContext ctx) const
{
    auto const enc = encoding_view();
    auto const a = attributes(enc.head());
    if (!enc.empty() && ((a & attr::is_literal) || (a & attr::is_preproc)))
    {
        return *enc.front().data;
    }
    return eval_e(x, ctx).to_json();
}


bool Expression::match(boost::json::value const& observed, Operator const& op) const
{
    auto result = eval(observed, EvalContext::make(op));
    auto const if_bool = result.if_bool();
    return if_bool ? *if_bool : false;
}

boost::json::value Expression::eval_as_predicate(Expression const& x, EvalContext ctx) const
{
    if (!is_noop() && is_const())
    {
        return Expression(encodeNested(Keyword::Eq, {*this})).eval(x, ctx); // TODO: optimize
    }
    return eval(x, ctx);
}

} // namespace expr
} // namespace zmbt
