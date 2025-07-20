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

#define UNUSED static_cast<void>(context);


namespace zmbt {
namespace lang {

using E = Expression;
using V = boost::json::value;

ZMBT_DEFINE_EVALUATE_IMPL(Pack)
{
    UNUSED; return self().parameter_list();
}

ZMBT_DEFINE_EVALUATE_IMPL(Pipe)
{
    auto const subexpressions = self().parameter_list();
    ASSERT(subexpressions.size() > 0, "invalid parameter (empty array)");

    auto fn = subexpressions.cbegin();
    // first el eval as is
    E ret = (*fn++).eval(lhs(), context++);
    while (fn != subexpressions.cend())
    {
        // any consequent literals eval as eq
        ret = (*fn++).eval_as_predicate(ret,context++);
    }
    return ret;
}

ZMBT_DEFINE_EVALUATE_IMPL(Fork)
{
    auto const subexpressions = self().parameter_list();

    boost::json::array out {};
    out.reserve(subexpressions.size());
    for (auto const& fn: subexpressions)
    {
        out.push_back(fn.eval(lhs(), context++));
    }
    return out;
}

ZMBT_DEFINE_EVALUATE_IMPL(Fmt)
{
    auto const subexpressions = self().parameter_list();

    auto const& x = lhs().data();

    static_cast<void>(context);
    V fmtstr;
    boost::format fmt;
    zmbt::remove_cvref_t<decltype(subexpressions)> args;

    auto const if_arr = x.if_array();
    auto const if_str = x.if_string();

    if (if_str)
    {
       fmt = boost::format{if_str->c_str()};
       args = subexpressions;
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
            args.push_back(E(arg));
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

    for (auto const& fn: args)
    {
        auto const item = fn.eval({}, context++);
        fmt = item.is_string() ? (fmt % item.get_string().c_str()) : (fmt % item);
    }
    return {fmt.str()};
}

ZMBT_DEFINE_EVALUATE_IMPL(All)
{
    auto const subexpressions = self().parameter_list();

    for (auto const& e: subexpressions)
    {
        if (not e.eval_as_predicate(lhs(),context++).as_bool())
        {
            return false;
        }
    }
    return true;
}


ZMBT_DEFINE_EVALUATE_IMPL(Any)
{
    auto const subexpressions = self().parameter_list();

    for (auto const& e: subexpressions)
    {
        if (e.eval_as_predicate(lhs(),context++).as_bool())
        {
            return true;
        }
    }
    return false;
}

ZMBT_DEFINE_EVALUATE_IMPL(Saturate)
{
    auto const subexpressions = self().parameter_list();
    auto const& x = lhs().data();
    ASSERT(x.is_array(), "invalid argument");

    auto const& samples = x.as_array();

    auto it = subexpressions.cbegin();
    for (auto const& sample: samples)
    {
        if (it == subexpressions.cend())
        {
            break;
        }
        if ((*it).eval_as_predicate(sample,context++).as_bool())
        {
            it++;
        }
    }
    return it == subexpressions.cend();
}



} // namespace lang
} // namespace zmbt

