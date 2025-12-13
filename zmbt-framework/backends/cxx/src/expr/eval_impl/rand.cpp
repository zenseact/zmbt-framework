/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include <atomic>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <random>
#include <utility>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"
#include "zmbt/application.hpp"



namespace zmbt {
namespace lang {

namespace
{

double uniform01()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(::zmbt::Config().Rng());
}

std::uint64_t uniform_int(std::uint64_t const lower, std::uint64_t const upper)
{
    std::uniform_int_distribution<std::uint64_t> dist(lower, upper);
    return dist(::zmbt::Config().Rng());
}

} // namespace


ZMBT_DEFINE_EVALUATE_IMPL(Rand)
{
    return uniform01();
}

ZMBT_DEFINE_EVALUATE_IMPL(RandInt)
{
    auto const params = self().subexpressions_list();
    ASSERT(params.size() < 3, "invalid arity");

    std::uint64_t lower = 0;
    std::uint64_t upper = RAND_MAX;

    if (params.size() == 1)
    {
        upper = boost::json::value_to<std::uint64_t>(params.at(0).eval({}, curr_ctx()));
    }
    else if (params.size() == 2)
    {
        lower = boost::json::value_to<std::uint64_t>(params.at(0).eval({}, curr_ctx()));
        upper = boost::json::value_to<std::uint64_t>(params.at(1).eval({}, curr_ctx()));
    }

    ASSERT(upper > lower, "invalid range");

    return uniform_int(lower, upper);
}

} // namespace lang
} // namespace zmbt
