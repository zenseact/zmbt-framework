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

#define MAX_RECURSION_DEPTH INT32_MAX


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


ZMBT_DEFINE_EVALUATE_IMPL(Recur)
{
    auto const subexpressions = self().subexpressions_list();
    ASSERT(subexpressions.size() == 2,
    "invalid parameters, expected (initial, Fn)");
    ExpressionView const init(subexpressions.front());
    ExpressionView const F (subexpressions.back());
    E item = init.eval_e({}, curr_ctx());


    E const dummy(false);

    auto const maybe_depth
        = lhs().is_literal()
        ? boost::json::try_value_to<std::uint64_t>(lhs().data())
        : boost::system::result<std::uint64_t>(MAX_RECURSION_DEPTH);
    ASSERT(maybe_depth.has_value(), "invalid parameter")
    std::uint64_t max_recursion_depth = maybe_depth.value();

    auto const& cond = lhs();

    std::function<bool(ExpressionView const&)> shall_halt = [](ExpressionView const&) -> bool { return false; };

    if (!cond.is_literal())
    {
        shall_halt = [&cond](ExpressionView const& next) -> bool {
            auto const maybe_exit = cond.eval_e(next, {}); // TODO? pass ctx if cond is non-trivial
            auto const if_bool = maybe_exit.if_bool();
            return (if_bool && *if_bool) || next.is_error();
        };
    }

    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {
        auto const next = F.eval_e(item,curr_ctx());
        if (shall_halt(next))
        {
            break;
        }
        item = next;
    }
    return item;
}

ZMBT_DEFINE_EVALUATE_IMPL(Unfold)
{
    auto const subexpressions = self().subexpressions_list();

    ASSERT(subexpressions.size() == 2,
    "invalid parameters, expected (initial, Fn)");
    ExpressionView const init(subexpressions.front());
    ExpressionView const F (subexpressions.back());
    E item = init.eval_e({}, curr_ctx());

    E const dummy(false);

    auto const maybe_depth
        = lhs().is_literal()
        ? boost::json::try_value_to<std::uint64_t>(lhs().data())
        : boost::system::result<std::uint64_t>(MAX_RECURSION_DEPTH);
    ASSERT(maybe_depth.has_value(), "invalid parameter")
    std::uint64_t max_recursion_depth = maybe_depth.value();

    auto const& cond = lhs();

    std::function<bool(ExpressionView const&)> shall_halt = [](ExpressionView const&) -> bool { return false; };

    if (!cond.is_literal())
    {
        shall_halt = [&cond](ExpressionView const& next) -> bool {
            auto const maybe_exit = cond.eval_e(next, {}); // TODO? pass ctx if cond is non-trivial
            auto const if_bool = maybe_exit.if_bool();
            return (if_bool && *if_bool) || next.is_error();
        };
    }

    boost::json::array result {};
    result.push_back(item.to_json());
    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {

        auto const next = F.eval_e(item,curr_ctx());
        if (shall_halt(next))
        {
            break;
        }
        item = next;
        result.push_back(item.to_json());
    }
    return result;
}


} // namespace lang
} // namespace zmbt

