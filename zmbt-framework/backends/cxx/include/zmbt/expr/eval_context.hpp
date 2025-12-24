/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EVAL_CONTEXT_HPP_
#define ZMBT_EXPR_EVAL_CONTEXT_HPP_


#include <map>
#include <memory>
#include <deque>

#include <boost/json.hpp>

#include "operator.hpp"
#include "eval_log.hpp"



namespace zmbt {
namespace lang {

class ExpressionView;
/// Expression evaluation context
struct EvalContext
{
    static EvalContext make(Operator const& op = {});
    /// Operator
    Operator op;
    /// Evaluation log
    EvalLog log;
    /// reference -> value map
    std::shared_ptr<boost::json::array> traces;
    std::shared_ptr<std::deque<boost::json::object>> capture_links;
    std::shared_ptr<std::map<boost::json::string, ExpressionView>> expr_links;
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
