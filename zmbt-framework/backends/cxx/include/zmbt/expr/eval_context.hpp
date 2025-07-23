/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EVAL_CONTEXT_HPP_
#define ZMBT_EXPR_EVAL_CONTEXT_HPP_

#include <memory>
#include <boost/json.hpp>

#include "operator.hpp"
#include "eval_log.hpp"


namespace zmbt {
namespace lang {

/// Expression evaluation context
struct EvalContext
{
    static EvalContext make(Operator const& op = {});
    /// Operator
    Operator op;
    /// Evaluation log
    EvalLog log;
    /// reference -> value map
    std::shared_ptr<boost::json::object> captures;
    // boost::json::object captures;
    /// Evaluation stack depth
    std::uint64_t depth;

    EvalContext();

    /// Copy context with depth increment
    EvalContext operator++() const;
};

}  // namespace lang
}  // namespace zmbt

#endif
