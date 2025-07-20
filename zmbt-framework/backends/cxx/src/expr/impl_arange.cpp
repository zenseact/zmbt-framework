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


ZMBT_DEFINE_EVALUATE_IMPL(Repeat)
{
    UNUSED_CTX;
    auto const param = rhs().eval();
    ASSERT(param.is_number(), "invalid parameter");
    std::uint64_t count = boost::json::value_to<std::uint64_t>(param);
    boost::json::array ret {};

    if (!count)
    {
        return ret;
    }
    ret.reserve(count);
    for (std::uint64_t i = 0; i < count; i++)
    {
        ret.push_back(lhs());
    }
    return ret;
}



ZMBT_DEFINE_EVALUATE_IMPL(Arange)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    ASSERT(x.is_number() || x.is_array() || x.is_string(), "invalid argument");

    std::int64_t start = 0;
    std::int64_t stop = 0;
    std::int64_t step = 1;

    if (x.is_number())
    {
        stop = boost::json::value_to<int>(x);
    }
    else if (x.is_array())
    {
        auto const& params = x.get_array();
        ASSERT(params.size() >= 1 && params.size() <= 3, "invalid argument");
        if (params.size() == 1)
        {
            stop = boost::json::value_to<std::int64_t>(params.at(0));
        }
        else if (params.size() >= 2)
        {
            start = boost::json::value_to<std::int64_t>(params.at(0));
            stop = boost::json::value_to<std::int64_t>(params.at(1));
        }
        if (params.size() == 3)
        {
            step = boost::json::value_to<std::int64_t>(params.at(2));
        }
    }
    else if (x.is_string())
    {
        auto const slice_idx = zmbt::detail::str_to_slice_idx(x.get_string());
        start = slice_idx.at(0);
        stop = slice_idx.at(1);
        step = slice_idx.at(2);
    }

    ASSERT(step != 0, "invalid parameter")

    L out {};
    if ((step > 0 && start >= stop) || (step < 0 && start <= stop))
    {
        return out;
    }

    out.reserve(std::max(0l, (stop - start) / step));
    for (std::int64_t i = start; (stop > start) ? i < stop : i > stop; i += step)
    {
        out.push_back(i);
    }
    return out;
}


} // namespace lang
} // namespace zmbt

