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


ZMBT_DEFINE_EVALUATE_IMPL(Cartesian)
{
    ASSERT(rhs().is_null(), "invalid parameter");
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument");
    auto const& arr = *if_arr;
    if (arr.empty()) return arr;
    std::size_t n {1};

    ASSERT(std::all_of(arr.cbegin(), arr.cend(), [&n](auto const& el) -> bool {
        if (el.is_array())
        {
            n *= el.get_array().size();
            return true;
        }
        return false;
    }), "invalid argument"
    );

    zmbt::JsonProdIter zip {arr};
    boost::json::array out {};
    out.reserve(n);
    while(!zip.halt())
    {
        out.push_back(*zip);
        zip++;
    }
    return out;
}


ZMBT_DEFINE_EVALUATE_IMPL(Uniques)
{
    ASSERT(lhs().if_array(), "invalid argument")
    return boost::json::value_from(
        boost::json::value_to<std::unordered_set<V>>(lhs().data())
    );
}



ZMBT_DEFINE_EVALUATE_IMPL(Intersect)
{
    ASSERT(lhs().if_array(), "invalid argument");
    ASSERT(rhs().if_array(), "invalid parameter");
    auto const lhs_set = boost::json::value_to<std::unordered_set<V>>(lhs().data());
    auto const rhs_set = boost::json::value_to<std::unordered_set<V>>(rhs().data());

    auto const& op = curr_ctx().op;
    L intersection {};
    intersection.reserve(std::max(lhs_set.size(), rhs_set.size()));
    auto out_it = std::back_inserter(intersection);

    auto lhs_it = lhs_set.cbegin();
    while (lhs_it != lhs_set.cend())
    {
        if (std::find_if(
            rhs_set.cbegin(),
            rhs_set.cend(),
            [&op, lhs_it](V const& v){ return op.apply(Keyword::Eq, v, *lhs_it).as_bool();}) != rhs_set.cend()
        )
        {
            *out_it = *lhs_it;
            out_it++;
        }
        lhs_it++;
    }
    intersection.shrink_to_fit();
    return intersection;
}

ZMBT_DEFINE_EVALUATE_IMPL(Diff)
{
    ASSERT(lhs().if_array(), "invalid argument");
    ASSERT(rhs().if_array(), "invalid parameter");
    auto const lhs_set = boost::json::value_to<std::unordered_set<V>>(lhs().data());
    auto const rhs_set = boost::json::value_to<std::unordered_set<V>>(rhs().data());

    auto const& op = curr_ctx().op;
    L difference {};
    difference.reserve(std::max(lhs_set.size(), rhs_set.size()));
    auto out_it = std::back_inserter(difference);

    auto lhs_it = lhs_set.cbegin();
    while (lhs_it != lhs_set.cend())
    {
        if (std::find_if(
            rhs_set.cbegin(),
            rhs_set.cend(),
            [&op, lhs_it](V const& v){ return op.apply(Keyword::Eq, v, *lhs_it).as_bool();}) == rhs_set.cend()
        )
        {
            *out_it = *lhs_it;
            out_it++;
        }
        lhs_it++;
    }
    difference.shrink_to_fit();
    return difference;
}


ZMBT_DEFINE_EVALUATE_IMPL(Union)
{
    ASSERT(lhs().if_array(), "invalid argument");
    ASSERT(rhs().if_array(), "invalid parameter");
    auto const lhs_set = boost::json::value_to<std::unordered_set<V>>(lhs().data());
    auto const rhs_set = boost::json::value_to<std::unordered_set<V>>(rhs().data());

    auto const& op = curr_ctx().op;
    L set_union {};
    set_union.reserve(lhs_set.size() + rhs_set.size());

    auto out_it = std::back_inserter(set_union);
    std::copy(lhs_set.cbegin(), lhs_set.cend(), out_it);
    std::copy_if(
        rhs_set.cbegin(),
        rhs_set.cend(),
        out_it,
        [&op, &lhs_set](V const& relem){
            return std::find_if(
                lhs_set.cbegin(),
                lhs_set.cend(),
                [&op, &relem](V const& lelem){
                    return op.apply(Keyword::Eq, lelem, relem).as_bool();
                }
            ) == lhs_set.cend();
        }
    );

    return set_union;
}


} // namespace lang
} // namespace zmbt

