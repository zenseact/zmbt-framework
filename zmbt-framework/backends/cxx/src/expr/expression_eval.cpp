/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include <memory>
#include <utility>

#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_impl.hpp"

namespace zmbt {
namespace lang {

boost::json::value ExpressionView::eval(boost::json::value const& x, EvalContext ctx) const
{
    auto const enc = encoding_view();
    auto const a = attributes(enc.head());
    if (!enc.empty() && ((a & attr::is_literal) || (a & attr::is_preproc)))
    {
        return *enc.front().data;
    }
    Expression tmp(x); // TODO: literal view on x without copies
    auto res = eval_e(tmp, std::move(ctx));
    return res.to_json();
}

bool ExpressionView::eval_as_predicate(ExpressionView const& x, Expression& err_sts, EvalContext ctx) const
{
    auto const res = eval_maybe_predicate(x, std::move(ctx));
    if (auto const if_bool = res.if_bool())
    {
        err_sts = nullptr;
        return *if_bool;
    }
    else if (res.is_error())
    {
        err_sts = res;
        return false;
    }
    else
    {
        err_sts = expr::Err("invalid predicate", keyword_to_str());
        return false;
    }
}

bool ExpressionView::eval_as_predicate(boost::json::value const& x, Expression& err_sts, EvalContext ctx) const
{
    Expression const tmp(x);
    return eval_as_predicate(tmp, err_sts, std::move(ctx));
}

Expression ExpressionView::eval_maybe_predicate(ExpressionView const& x, EvalContext ctx) const
{
    if (!is_noop() && is_const())
    {
        if (!const_predicate_cache_)
        {
            auto cached = Expression(Expression::encodeNested(
                Keyword::Eq,
                {Expression(encoding_view().freeze())}
            ));
            const_predicate_cache_ = std::make_shared<Expression>(std::move(cached));
        }
        return const_predicate_cache_->eval_e(x, std::move(ctx));
    }
    return eval_e(x, std::move(ctx));
}


bool ExpressionView::match(boost::json::value const& x, Operator const& op) const
{
    auto result = eval(x, EvalContext::make(op));
    auto const if_bool = result.if_bool();
    return if_bool ? *if_bool : false;
}

boost::json::value operator*(ExpressionView lhs, ExpressionView const& rhs)
{
    return lhs.eval_e(rhs, {}).to_json();
}

boost::json::value operator*(ExpressionView expr)
{
    return expr.eval();
}

} // namespace expr
} // namespace zmbt
