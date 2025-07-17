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

boost::json::value Expression::eval(boost::json::value const& x) const
{
    return eval(x, {});
}

boost::json::value Expression::eval(boost::json::value const& x, EvalContext const& ctx) const
try
{
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;

    boost::json::value result {};

    if(is_literal() || is_preproc())
    {
        result = encoding().data.at(0);
    }
    else if (is(Keyword::Q))
    {
        auto const pl = parameter_list();
        if (pl.size() > 1)
        {
            return detail::make_error_expr("corrupted data", keyword_to_str());
        }
        result = pl.empty() ? nullptr : boost::json::value_from(pl.front());
    }
    else if (is(Keyword::Id))
    {
        result = x;
        ctx.log.push(*this, x, result, ctx.depth);
    }
    else {
        CodegenType const classifier = getCodegenType(keyword());
        switch (classifier)
        {
            case CodegenType::Const:       { result = eval_Const    (x);      break; }
            case CodegenType::UnaryOp:     { result = eval_UnaryOp  (x, ctx); break; }
            case CodegenType::BinaryOp:    { result = eval_BinaryOp (x, ctx); break; }
            case CodegenType::CodegenFn:   { result = eval_CodegenFn(x, ctx); break; }
            case CodegenType::None:
            default:
            {

                if (is_variadic())
                {
                    result = eval_Variadic(x, ctx);
                }
                else if (is_hiord())
                {
                    result = eval_HiOrd(x, ctx);
                }
                else
                {
                    result = eval_Special(x, ctx);
                }
                break;
            }
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

