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
    auto const fork = self().encoding_view().child(0);
    ASSERT(
        (fork.head() == K::Fork || fork.head() == K::Pack)
        && fork.arity() == 2,
    "invalid parameters, expected initial + Fn");
    auto const initial = E(fork.child(0).freeze()).eval();
    auto const F = E(fork.child(1).freeze());


    E const dummy(false);

    auto const maybe_depth
        = lhs().is_literal()
        ? boost::json::try_value_to<std::uint64_t>(lhs())
        : boost::json::result<std::uint64_t>(MAX_RECURSION_DEPTH);
    ASSERT(maybe_depth.has_value(), "invalid parameter")
    std::uint64_t max_recursion_depth = maybe_depth.value();

    auto const& cond = lhs();

    std::function<bool(Expression const&)> shall_halt = [](Expression const&) -> bool { return false; };

    if (!cond.is_literal())
    {
        shall_halt = [&cond](Expression const& next) -> bool {
            auto const maybe_exit = cond.eval_e(next, {}); // TODO? pass ctx if cond is non-trivial
            auto const if_bool = maybe_exit.if_bool();
            return (if_bool && *if_bool) || next.is_error();
        };
    }


    boost::json::value result = initial;

    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {
        auto const next = F.eval_e(result,curr_ctx() MAYBE_INCR);
        if (shall_halt(next))
        {
            break;
        }
        result = next;
    }
    return result;
}

ZMBT_DEFINE_EVALUATE_IMPL(Unfold)
{
    auto const fork = self().encoding_view().child(0);
    ASSERT(
        (fork.head() == K::Fork || fork.head() == K::Pack)
        && fork.arity() == 2,
    "invalid parameters, expected initial + Fn");
    auto const initial = E(fork.child(0).freeze()).eval();
    auto const F = E(fork.child(1).freeze());

    E const dummy(false);

    auto const maybe_depth
        = lhs().is_literal()
        ? boost::json::try_value_to<std::uint64_t>(lhs())
        : boost::json::result<std::uint64_t>(MAX_RECURSION_DEPTH);
    ASSERT(maybe_depth.has_value(), "invalid parameter")
    std::uint64_t max_recursion_depth = maybe_depth.value();

    auto const& cond = [&]() -> E const& {
        return lhs().is_literal() ? dummy : lhs();
    }();

    boost::json::array result {};
    result.push_back(initial);
    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {
        auto const next = F.eval_e(result.back(),curr_ctx() MAYBE_INCR);
        auto const maybe_exit = cond.eval_e(next, {});
        auto const if_bool = maybe_exit.if_bool();
        if ((if_bool && *if_bool) || next.is_error())
        {
            break;
        }
        result.push_back(next);
    }
    return result;
}


} // namespace lang
} // namespace zmbt

