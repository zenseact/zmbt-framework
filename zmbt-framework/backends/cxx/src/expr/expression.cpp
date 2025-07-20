/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <cstdint>
#include <ostream>
#include <sstream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/attributes.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/api.hpp"


namespace
{

using V = boost::json::value;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;


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
        param = zmbt::expr::Id;
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


Expression::Expression() : Expression(Keyword::Void)
{
}

Expression::Expression(Keyword const& keyword)
    : Expression(encodeNested(keyword, {}))
{
    if (keyword == Keyword::Literal || keyword == Keyword::PreProc)
    {
        throw zmbt::expression_error("expression requires a value");
    }
}

Expression::Expression(boost::json::value const& expr)
    : encoding_{expr}
{
}

Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(encodeLiteral(boost::json::value(items)))
{
}

boost::json::value Expression::to_json() const
{
    return (is_literal() || is_preproc()) ? (*encoding_view().at(0).data) : boost::json::value_from(encoding());
}

bool Expression::is_const() const
{
    return encoding_view().is_const();
}

boost::json::string_view Expression::keyword_to_str() const
{
    return ::zmbt::lang::keyword_to_str(keyword());
}

bool Expression::is_boolean() const
{
    return encoding_view().is_boolean();
}


std::list<Expression> Expression::parameter_list() const
{
    std::list<Expression> result;

    auto subtrees = encoding_view().children();

    for (auto const& s: subtrees)
    {
        result.emplace_back(s.freeze());
    }
    return result;
}


bool Expression::match(boost::json::value const& observed, Operator const& op) const
{
    auto result = eval(observed, {op, {}, 0});
    auto const if_bool = result.if_bool();
    return if_bool ? *if_bool : false;
}

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


}  // namespace lang
}  // namespace zmbt
