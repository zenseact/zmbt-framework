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


ZMBT_DEFINE_EVALUATE_IMPL(Dbg)
{
    EvalContext local_ctx {};
    local_ctx.op = curr_ctx().op;
    local_ctx.log = EvalLog::make();

    auto const result = rhs().eval_e(lhs(), local_ctx);

    ZMBT_LOG_JSON(INFO).WithSrcLoc("ZMBT_EXPR_DEBUG") << *local_ctx.log.stack;
    ZMBT_LOG_CERR(DEBUG).WithSrcLoc("ZMBT_EXPR_DEBUG") << "\n" << local_ctx.log.str(2);

    if (curr_ctx().log.stack)
    {
        curr_ctx().log.stack->reserve(curr_ctx().log.stack->capacity() + local_ctx.log.stack->size());
        for (auto line_it = std::make_move_iterator(local_ctx.log.stack->begin()),
        log_end = std::make_move_iterator(local_ctx.log.stack->end());
        line_it != log_end; ++line_it)
        {
            line_it->as_array().at(0).as_uint64() += (curr_ctx().depth);
            curr_ctx().log.stack->push_back(*line_it);
        }
    }
    return result;
}

ZMBT_DEFINE_EVALUATE_IMPL(Trace)
{
    ZMBT_LOG_JSON(INFO).WithSrcLoc("ZMBT_EXPR_TRACE") << rhs().data() << " " << lhs();
    ZMBT_LOG_CERR(DEBUG).WithSrcLoc("ZMBT_EXPR_TRACE") << rhs().data() << " " << lhs();
    return lhs();
}



} // namespace lang
} // namespace zmbt

