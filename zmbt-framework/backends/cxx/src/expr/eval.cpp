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
#include "zmbt/expr/attributes.hpp"
#include "zmbt/expr/eval_context.hpp"

#include "zmbt/expr/exceptions.hpp"


namespace
{

using V = boost::json::value;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;

static V const kNullValue = nullptr;
static V const kDefaultKeyFn = E(Keyword::Id);

// TODO: handle it in codegen
bool set_default_param(Keyword const& keyword, V &param)
{
    switch(keyword)
    {
    case Keyword::Sort:
    case Keyword::Min:
    case Keyword::Max:
    case Keyword::Argmin:
    case Keyword::Argmax:
        {
            param = kDefaultKeyFn;
            break;
        }
    default:
        {
            break;
        }
    }
    return nullptr != param;
}

}

namespace zmbt {
namespace lang {

void Expression::handle_binary_args(V const& x, V &lhs, V &rhs) const
{
    if (has_subexpr())
    {
        lhs = x;
        rhs = subexpr().to_json();
    }
    else if (set_default_param(keyword(), rhs))
    {
        lhs = x;
    }
    else // treat x as argument pair
    {
        auto const if_array = x.if_array();
        if (if_array && if_array->size() == 2)
        {
            lhs = if_array->front();
            rhs = if_array->back();
        }
        else
        {
            lhs = x;
            rhs = kNullValue;
        }
    }
}

bool Expression::to_predicate_if_const(Expression& e)
{
    if (!e.is_noop() && e.is_const())
    {
        e = Expression(encodeNested(Keyword::Eq, {e}));
        return true;
    }
    return false;
}

boost::json::value  Expression::eval_as_predicate(boost::json::value const& x, EvalContext const& ctx) const
{
    if (!is_noop() && is_const())
    {
        return Expression(encodeNested(Keyword::Eq, {*this})).eval(x, ctx); // TODO: optimize
    }
    return eval(x, ctx);
}

boost::json::value Expression::eval(boost::json::value const& x) const
{
    return eval(x, {});
}

boost::json::value Expression::eval(boost::json::value const& x, EvalContext const& ctx) const
try
{
    boost::json::value result {};

    if(is_literal() || is_preproc())
    {
        result = encoding().data.at(0);
    }
    else if (is(Keyword::Q))
    {
        auto const child = encoding_view().child(0);
        if (child.empty())
        {
            result = nullptr;
        }
        else
        {
            result = Expression(child.freeze()).to_json();
        }
    }
    else if (is(Keyword::Id))
    {
        result = x;
        ctx.log.push(*this, x, result, ctx.depth);
    }
    else {
        auto const a = attributes(keyword());

        if (a & (attr::is_const))
        {
            result = eval_Const(x);
        }
        else if ((a & attr::is_operator) && (a & attr::is_unary))
        {
            result = eval_UnaryOp  (x, ctx);
        }
        else if ((a & attr::is_operator) && (a & attr::is_binary))
        {
            result = eval_BinaryOp  (x, ctx);
        }
        else if (a & attr::is_autogen)
        {
            result = eval_CodegenFn(x, ctx);
        }
        else if (a & attr::is_variadic)
        {
            result = eval_Variadic(x, ctx);
        }
        else if (a & attr::is_hiord)
        {
            result = eval_HiOrd(x, ctx);
        }
        else
        {
            result = eval_Special(x, ctx);
        }
        ctx.log.push(*this, x, result, ctx.depth);
    }
    return result;
}
catch(const std::exception& e)
{
    return detail::make_error_expr(e.what(), keyword_to_str());
}


} // namespace lang
} // namespace zmbt

