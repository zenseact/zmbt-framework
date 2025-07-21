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



ZMBT_DEFINE_EVALUATE_IMPL(Kwrd)
{
    return lhs().keyword_to_str();
}

ZMBT_DEFINE_EVALUATE_IMPL(Prms)
{
    auto enc = lhs().encoding_view().freeze();
    // TODO Pack?
    enc.keywords.front() = Keyword::Fork;
    return Expression(enc);
}


ZMBT_DEFINE_EVALUATE_IMPL(Sort)
{
    auto const& x = lhs().data();

    ASSERT(x.is_array(), "invalid argument");


    auto const& key_fn = rhs().is_literal() && rhs().is_null() ? expr::Id : rhs();

    auto const& op = curr_ctx().op;
    std::function<bool(V const&, V const&)> is_less = [&key_fn, &op](V const& lhs, V const& rhs) ->bool {
        return op.apply(Keyword::Lt, key_fn.eval(lhs), key_fn.eval(rhs)).as_bool();
    };
    boost::json::array out = x.get_array();
    std::stable_sort(out.begin(), out.end(), is_less);
    return out;
}


ZMBT_DEFINE_EVALUATE_IMPL(Eval)
{
    return lhs().eval(rhs(), curr_ctx() MAYBE_INCR);
}


ZMBT_DEFINE_EVALUATE_IMPL(Bind)
{
    auto const enc = rhs().encoding_view().freeze();
    ASSERT(enc.size() == 1, "function has parameters")

    if (zmbt::lang::attributes(enc.keywords.front()) & attr::is_variadic)
    {
        auto const if_arr = lhs().if_array();
        ASSERT(if_arr, "invalid argument")
        std::vector<E> xx;
        xx.reserve(if_arr->size());
        for (auto const& item: *if_arr)
        {
            xx.emplace_back(E(item));
        }
        return E(E::encodeNested(enc.keywords.front(), std::move(xx)));

    }
    else
    {
        return E(E::encodeNested(enc.keywords.front(), {lhs()}));
    }
}

ZMBT_DEFINE_EVALUATE_IMPL(Flip)
{
    auto const child = rhs().encoding_view().child(0);
    ASSERT(!child.empty(), "invalid parameter");
    auto const flip = E(E::encodeNested(rhs().keyword(), {lhs()}));
    return flip.eval(E(child.freeze()), curr_ctx() MAYBE_INCR);
}


ZMBT_DEFINE_EVALUATE_IMPL(Try)
{
    auto const result = rhs().eval_e(lhs(),curr_ctx() MAYBE_INCR);
    if (result.is_error()) return nullptr;
    return result;
}



ZMBT_DEFINE_EVALUATE_IMPL(Count)
{
    auto const if_arr = lhs().if_array();
    auto const if_obj = lhs().if_object();
    ASSERT(if_arr || if_obj, "invalid argument")

    std::size_t count {0};

    if (if_arr)
    {
        for (auto const& sample: *if_arr)
        {
            if (rhs().eval_as_predicate(sample,curr_ctx() MAYBE_INCR).as_bool())
            {
                count++;
            }
        }
    }
    else if (if_obj)
    {
        for (auto const& kv: *if_obj)
        {
            if (rhs().eval_as_predicate({kv.key(), kv.value()},curr_ctx() MAYBE_INCR).as_bool())
            {
                count++;
            }
        }
    }

    return count;
}

ZMBT_DEFINE_EVALUATE_IMPL(Each)
{
    auto const if_arr = lhs().if_array();
    auto const if_obj = lhs().if_object();
    ASSERT(if_arr || if_obj, "invalid argument")

    if (if_arr)
    {
        for (auto const& sample: *if_arr)
        {
            if (!rhs().eval_as_predicate(sample,curr_ctx() MAYBE_INCR).as_bool())
            {
                return false;
            }
        }
    }
    else if (if_obj)
    {
        for (auto const& kv: *if_obj)
        {
            if (!rhs().eval_as_predicate({kv.key(), kv.value()},curr_ctx() MAYBE_INCR).as_bool())
            {
                return false;
            }
        }
    }

    return true;
}


ZMBT_DEFINE_EVALUATE_IMPL(Map)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument")
    auto const& samples = *if_arr;
    auto const& F = rhs();
    boost::json::array ret {};
    if (samples.empty())
    {
        return ret;
    }

    ret.reserve(samples.size());

    for (auto const& el: samples)
    {
        ret.push_back(F.eval(el,curr_ctx() MAYBE_INCR));
    }

    return ret;
}


ZMBT_DEFINE_EVALUATE_IMPL(Filter)
{
    auto const if_arr = lhs().if_array();
    ASSERT(if_arr, "invalid argument")
    auto const& samples = *if_arr;
    boost::json::array ret {};
    auto const& F = rhs();

    if (samples.empty())
    {
        return ret;
    }
    for (auto const& el: samples)
    {
        if (F.eval_as_predicate(el,curr_ctx() MAYBE_INCR).as_bool())
        {
            ret.push_back(el);
        }
    }
    return ret;
}

} // namespace lang
} // namespace zmbt

