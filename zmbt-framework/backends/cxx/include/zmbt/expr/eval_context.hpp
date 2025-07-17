/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EVAL_CONTEXT_HPP_
#define ZMBT_EXPR_EVAL_CONTEXT_HPP_

#include "operator.hpp"
#include "eval_log.hpp"


namespace zmbt {
namespace lang {

/// Expression evaluation context
struct EvalContext
{
    /// Operator
    Operator op;
    /// Evaluation log
    EvalLog log;
    /// Evaluation stack depth
    std::uint64_t const depth;

    /// Copy context with depth increment
    EvalContext operator++(int) const;
};

}  // namespace lang
}  // namespace zmbt

#endif
