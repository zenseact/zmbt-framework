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
#include "zmbt/expr/exceptions.hpp"
#include "zmbt/expr/keyword_codegen_type.hpp"


#define ASSERT(E)      if (!(E)) { throw zmbt::expression_error("%s#%d - " #E, __FILE__, __LINE__);}

namespace
{

using V = boost::json::value;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;

static V const kNullValue = nullptr;
static V const kDefaultKeyFn = E(Keyword::Id);

// TODO: handle it in codegen
void set_default_param(Keyword const& keyword, V const*& param)
{
    switch(keyword)
    {
    case Keyword::Sort:
    case Keyword::Min:
    case Keyword::Max:
    case Keyword::Argmin:
    case Keyword::Argmax:
        {
            param = &kDefaultKeyFn;
            break;
        }
    default:
        {
            break;
        }
    }
}

}

namespace zmbt {
namespace lang {

void Expression::handle_binary_args(V const& x, V const*& lhs, V const*& rhs) const
{
    if (has_params())
    {
        lhs = &x;
        rhs = params_ptr_;
    }
    else if (set_default_param(keyword(), rhs), rhs != nullptr)
    {
        lhs = &x;
    }
    else // treat x as argument pair
    {
        if (x.is_array() && x.get_array().size() == 2)
        {
            lhs = x.get_array().cbegin();
            rhs = lhs + 1;
        }
        else
        {
            lhs = &x;
            rhs = &kNullValue;
        }
    }
}

boost::json::value Expression::eval(boost::json::value const& x, EvalContext const& ctx) const
{
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;

    boost::json::value result {};

    if(is_literal())
    {
        result = underlying();
    }
    else if (is(Keyword::C))
    {
        result = params();
    }
    else if (is(Keyword::Id))
    {
        result = x;
        ctx.log.push(underlying(), x, result, ctx.depth);
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
                result = is_hiord() ? eval_HiOrd(x, ctx) : eval_Special(x, ctx);
                break;

            }
        }
        ctx.log.push(underlying(),x, result, ctx.depth);
    }
    return result;
}


} // namespace lang
} // namespace zmbt

