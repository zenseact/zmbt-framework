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

ZMBT_DEFINE_EVALUATE_IMPL(Transp)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(param.is_null(), "invalid parameter");
    ASSERT(x.is_array(), "invalid argument");
    auto const& arr = x.get_array();
    if (arr.empty()) return arr;
    auto const N = arr.at(0).as_array().size();
    ASSERT(
        std::all_of(arr.cbegin(), arr.cend(), [N](auto const& el) -> bool { return el.is_array() && el.as_array().size() == N;})
        , "invalid argument"
    );

    zmbt::JsonZipIter zip {arr};
    boost::json::array out {};
    out.reserve(N);
    while(!zip.halt())
    {
        out.push_back(*zip);
        zip++;
    }
    return out;
}




ZMBT_DEFINE_EVALUATE_IMPL(Cat)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(x.kind() == param.kind(), "invalid argument");
    ASSERT(x.is_string() || x.is_array(), "invalid argument");

    if (x.is_string())
    {
        return {zmbt::format("%s%s", x, param)};
    }
    else if (x.is_array())
    {
        boost::json::array out = x.get_array();
        out.reserve(out.size() + param.get_array().size());
        for (auto const& el: param.get_array())
        {
            out.push_back(el);
        }
        return out;
    }
    return nullptr;
}

ZMBT_DEFINE_EVALUATE_IMPL(Push)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(x.is_string() || x.is_array(), "invalid argument");

    if (x.is_string())
    {
        return {zmbt::format("%s%s", param, x)};
    }
    else if (x.is_array())
    {
        boost::json::array out {};
        out.reserve(1 + x.get_array().size());
        out.push_back(param);
        for (auto const& el: x.get_array())
        {
            out.push_back(el);
        }
        return out;
    }
    return nullptr;
}


ZMBT_DEFINE_EVALUATE_IMPL(Reverse)
{
    UNUSED_CTX;
    auto const x = lhs().eval();

    ASSERT(x.is_array(), "invalid argument");
    boost::json::array out = x.get_array();
    std::reverse(out.begin(), out.end());
    return out;
}



ZMBT_DEFINE_EVALUATE_IMPL(Slide)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(x.is_array(), "invalid argument");
    ASSERT(param.is_number(), "invalid parameter");

    auto const W = boost::json::value_to<std::int64_t>(param);
    ASSERT(W > 0, "invalid parameter");

    auto const& samples = x.get_array();
    boost::json::array out {};
    if (samples.empty() || (0 == W))
    {
        return out;
    }
    if (samples.size() <= static_cast<std::size_t>(W))
    {
        return out;
    }
    out.reserve(samples.size() - W);
    auto window_begin = samples.cbegin();
    auto window_end = window_begin + W;
    while (window_end <= samples.cend())
    {
        out.push_back(boost::json::array(window_begin, window_end));
        window_begin++;
        window_end++;
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Chunks)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(x.is_array(), "invalid argument");
    ASSERT(param.is_number(), "invalid parameter");
    auto const W = boost::json::value_to<std::uint64_t>(param);
    ASSERT(W > 0, "invalid parameter");
    auto const& samples = x.get_array();
    auto const N = samples.size();
    boost::json::array out {};
    if (samples.empty() || (0 == W))
    {
        return out;
    }
    if (samples.size() <= W)
    {
        out.push_back(samples);
        return out;
    }
    auto stride_begin = samples.cbegin();
    auto stride_end = stride_begin + W;
    auto stride_boundary = samples.cbegin() + (N - (N % W));

    while (stride_end <= stride_boundary)
    {
        out.push_back(boost::json::array(stride_begin, stride_end));
        stride_begin += W;
        stride_end += W;
    }
    if (stride_boundary != samples.cend())
    {
        out.push_back(boost::json::array(stride_boundary, samples.cend()));
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Stride)
{
    UNUSED_CTX;
    auto const x = lhs().eval();
    auto const param = rhs().eval();

    // auto result = eval_impl<Keyword::Chunks>()(x, param, context).as_array();
    auto result =  (x | expr::Chunks(param)).eval({}, context).as_array();

    if (!result.empty() && result.back().as_array().size() != param)
    {
        result.pop_back();
    }
    return result;
}



ZMBT_DEFINE_EVALUATE_IMPL(Enumerate)
{
    UNUSED_CTX;
    auto const x = lhs().eval();

    ASSERT(x.is_array(), "invalid argument");
    auto const& arr = x.get_array();
    boost::json::array out {};
    out.reserve(arr.size());
    std::size_t i = 0;
    for (auto const& el: arr)
    {
        out.push_back({i++, el});
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Flatten)
{
    UNUSED_CTX;
    auto const x = lhs().eval();

    ASSERT(x.is_array(), "invalid argument");
    auto const& arr = x.get_array();
    boost::json::array out {};
    out.reserve(arr.size());
    for (auto const& el: arr)
    {
        if (el.is_array())
        {
            for (auto const& subel: el.get_array())
            {
                out.push_back(subel);
            }
        }
        else
        {
            out.push_back(el);
        }
    }
    return out;
}




} // namespace lang
} // namespace zmbt

