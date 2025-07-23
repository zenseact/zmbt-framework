/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/eval_context.hpp"


namespace zmbt {
namespace lang {

EvalContext EvalContext::make(Operator const& op)
{
    EvalContext next{};
    next.log = EvalLog::make();
    next.op = op;
    return next;
    // return {op, EvalLog::make(), std::make_shared<boost::json::object>(), 0};
    // return {op, EvalLog::make(), {}, 0};
}
EvalContext::EvalContext()
    : op{}
    , log{}
    , captures{std::make_shared<boost::json::object>()}
    , depth{0}
{
}

EvalContext EvalContext::operator++() const
{
    EvalContext next = *this;
    next.depth++;
    return next;
}



}  // namespace lang
}  // namespace zmbt
