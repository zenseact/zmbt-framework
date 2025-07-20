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



// ZMBT_DEFINE_EVALUATE_IMPL(Arange)


ZMBT_DEFINE_EVALUATE_IMPL(Items)
{
    auto const x = lhs().eval();
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back({kv.key(), kv.value()});
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Keys)
{
    auto const x = lhs().eval();
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.key());
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Values)
{
    auto const x = lhs().eval();
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.value());
    }
    return out;
}


ZMBT_DEFINE_EVALUATE_IMPL(Size)
{
    auto const x = lhs().eval();
    auto const param = rhs().eval();
    ASSERT(param.is_null(), "invalid parameter")
    ASSERT(x.is_structured(), "invalid argument")
    V size = nullptr;
    if (x.is_array())
    {
        return x.get_array().size();
    }
    else
    {
        return x.get_object().size();
    }
}


ZMBT_DEFINE_EVALUATE_IMPL(Card)
{
    auto const x = lhs().eval();
    auto const param = rhs().eval();
    ASSERT(param.is_null(), "invalid parameter")
    ASSERT(x.is_structured(), "invalid argument")

    if (x.is_array())
    {
        // TODO: optimize
        return (x | expr::Uniques).eval().as_array().size();
    }
    else
    {
        return x.get_object().size();
    }
}



} // namespace lang
} // namespace zmbt

