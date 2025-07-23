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

using compare_fn_t = std::function<bool(V const&, V const&)>;

using get_element_fn_t =
std::function<L::const_iterator(L::const_iterator, L::const_iterator, compare_fn_t const&)>;

L::const_iterator find_argminmax(L const& samples, E const& param, O const& op, get_element_fn_t const& get_element)
{
    E const& key_fn = param.is_literal() && param.is_null() ? zmbt::expr::Id : param;

    std::function<bool(V const&, V const&)> is_less = [key_fn, op](V const& lhs, V const& rhs) ->bool {
        return op.apply(K::Lt, key_fn.eval(lhs), key_fn.eval(rhs)).as_bool();
    };
    return get_element(samples.cbegin(), samples.cend(), is_less);
}
} // namespace


namespace zmbt {
namespace lang {



ZMBT_DEFINE_EVALUATE_IMPL(Argmin)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument");
    auto const& samples = *if_arr;
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, rhs(), curr_ctx().op, &std::min_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

ZMBT_DEFINE_EVALUATE_IMPL(Argmax)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument");
    auto const& samples = *if_arr;
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, rhs(), curr_ctx().op, &std::max_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

ZMBT_DEFINE_EVALUATE_IMPL(Min)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument");
    auto const& samples = *if_arr;
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, rhs(), curr_ctx().op, &std::min_element<L::const_iterator, compare_fn_t>);
}

ZMBT_DEFINE_EVALUATE_IMPL(Max)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument");
    auto const& samples = *if_arr;
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, rhs(), curr_ctx().op, &std::max_element<L::const_iterator, compare_fn_t>);
}


} // namespace lang
} // namespace zmbt

