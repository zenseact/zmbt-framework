/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */
#include <regex>
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


ZMBT_DEFINE_EVALUATE_IMPL(Re)
{
    auto const param = rhs().eval();
    auto const x = lhs().eval();

    ASSERT(param.is_string(), "invalid parameter");
    ASSERT(x.is_string(), "invalid argument");

    auto const pattern = param.get_string().c_str();
    auto const sample = x.get_string().c_str();
    std::regex const re(pattern);
    return std::regex_match(sample, re);
}

} // namespace lang
} // namespace zmbt

