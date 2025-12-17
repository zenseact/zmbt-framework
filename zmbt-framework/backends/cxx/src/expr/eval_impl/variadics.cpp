/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include <limits>
#include <boost/json.hpp>
#include <boost/format.hpp>


#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"


namespace zmbt {
namespace lang {

using E = Expression;
using V = boost::json::value;

ZMBT_DEFINE_EVALUATE_IMPL(Tuple)
{
    return self().subexpressions_list();
}

ZMBT_DEFINE_EVALUATE_IMPL(Pipe)
{
    auto const subexpressions = self().subexpressions_list();
    ASSERT(subexpressions.size() > 0, "invalid parameter (empty array)");

    auto fn = subexpressions.cbegin();
    // first el eval as is
    E ret = (*fn++).eval_e(lhs(), curr_ctx());
    while (fn != subexpressions.cend())
    {
        // any consequent literals eval as eq
        ret = (*fn++).eval_maybe_predicate(ret,curr_ctx());
    }
    return ret;
}

ZMBT_DEFINE_EVALUATE_IMPL(Fork)
{
    auto const fork_terms = self().subexpressions_list();

    boost::json::array out {};
    out.reserve(fork_terms.size());
    for (auto const& fn: fork_terms)
    {
        out.push_back(fn.eval_e(lhs(), curr_ctx()).to_json());
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Fmt)
{
    auto const subexpressions = self().subexpressions_list();

    auto const& x = lhs().data();

    V fmtstr;
    boost::format fmt;
    boost::json::array args;

    auto const if_arr = x.if_array();
    auto const if_str = x.if_string();

    if (if_str)
    {
        fmt = boost::format{if_str->c_str()};
        for (auto const& term: subexpressions)
        {
            auto e = term.eval_e({}, curr_ctx());
            if (!e.is_literal())
            {
                args.emplace_back(e.prettify());
            }
            else
            {
                args.emplace_back(e.data());
            }
        }
    }
    else if (if_arr && subexpressions.empty())
    {
        ASSERT(if_arr->size() == 2, "invalid arity");

        auto const if_lhs_str = if_arr->front().if_string();
        auto const if_rhs_args = if_arr->back().if_array();

        ASSERT(if_lhs_str && if_rhs_args, "invalid operands");

        fmt = boost::format{if_lhs_str->c_str()};
        for (auto const& arg: *if_rhs_args)
        {
            args.push_back(E(arg).eval({}, curr_ctx()));
        }
    }
    else
    {
        ASSERT(false, "invalid parameters");
    }

    // ASSERT(x.is_string(), "invalid argument");
    if (args.empty()) return x;

    auto const N = static_cast<std::size_t>(fmt.expected_args());
    ASSERT(args.size() == N, "invalid formatting");

    for (auto const& item: args)
    {
        fmt = item.is_string() ? (fmt % item.get_string().c_str()) : (fmt % item);
    }
    return {fmt.str()};
}

ZMBT_DEFINE_EVALUATE_IMPL(All)
{
    auto const subexpressions = self().subexpressions_list();
    E err_sts(nullptr);

    for (auto const& e: subexpressions)
    {
        if (not e.eval_as_predicate(lhs(),err_sts,curr_ctx()) && !err_sts.is_error())
        {
            return false;
        }
        else if(err_sts.is_error())
        {
            return err_sts;
        }
    }
    return true;
}


ZMBT_DEFINE_EVALUATE_IMPL(Any)
{
    auto const subexpressions = self().subexpressions_list();
    E err_sts(nullptr);

    for (auto const& e: subexpressions)
    {
        if (e.eval_as_predicate(lhs(), err_sts, curr_ctx()) && !err_sts.is_error())
        {
            return true;
        }
        else if(err_sts.is_error())
        {
            return err_sts;
        }
    }
    return false;
}

ZMBT_DEFINE_EVALUATE_IMPL(Saturate)
{
    auto const subexpressions = self().subexpressions_list();
    auto const& x = lhs().data();
    ASSERT(x.is_array(), "invalid argument");

    auto const& samples = x.as_array();

    auto it = subexpressions.cbegin();
    E err_sts(nullptr);

    for (auto const& sample: samples)
    {
        if (it == subexpressions.cend())
        {
            break;
        }
        if ((*it).eval_as_predicate(sample,err_sts,curr_ctx()) && !err_sts.is_error())
        {
            it++;
        }
        else if(err_sts.is_error())
        {
            return err_sts;
        }
    }
    return it == subexpressions.cend();
}



} // namespace lang
} // namespace zmbt

