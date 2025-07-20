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


bool set_default_param(Keyword const& keyword, E &param)
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
        return true;
    }
    default:
    {
        return false;
    }
    }
}

}

namespace zmbt {
namespace lang {

void Expression::handle_binary_args(V const& x, Expression &lhs, Expression &rhs) const
{
    bool const is_binary = attributes(keyword()) & attr::is_binary;
    if (!is_binary | has_subexpr())
    {
        lhs = E(x);
        rhs = subexpr();
    }
    else if (set_default_param(keyword(), rhs))
    {
        lhs = E(x);
    }
    else // treat x as argument pair
    {
        auto const if_array = x.if_array();
        if (if_array && if_array->size() == 2)
        {
            lhs = E(if_array->front());
            rhs = E(if_array->back());
        }
        else
        {
            lhs = E(x);
            rhs = E(nullptr);
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


} // namespace lang
} // namespace zmbt

