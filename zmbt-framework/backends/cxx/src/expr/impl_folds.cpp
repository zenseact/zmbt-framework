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

} // namespace


namespace zmbt {
namespace lang {

ZMBT_DEFINE_EVALUATE_IMPL(Fold)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument")
    auto const& samples = *if_arr;
    if (samples.empty())
    {
        return nullptr;
    }
    auto const& F = rhs();
    auto it = samples.cbegin();

    // take init term
    boost::json::value ret = *it++;

    while (it != samples.cend())
    {
        ret = F.eval({ret, *it},context++);
        it++;
    }

    return ret;
}

ZMBT_DEFINE_EVALUATE_IMPL(Sum)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    ASSERT(x.is_array(), "invalid argument");
    return expr::Fold(expr::Add).eval(x, context++);
}

ZMBT_DEFINE_EVALUATE_IMPL(Prod)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    ASSERT(x.is_array(), "invalid argument");
    return expr::Fold(expr::Mul).eval(x, context++);
}

ZMBT_DEFINE_EVALUATE_IMPL(Avg)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    ASSERT(x.is_array(), "invalid argument");

    auto const N = x.as_array().size();
    auto const sum = expr::Fold(expr::Add).eval(x, context++);
    return context.op.apply(Keyword::Div, sum, N);
}

} // namespace lang
} // namespace zmbt

