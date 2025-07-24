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



namespace zmbt {
namespace lang {

ZMBT_DEFINE_EVALUATE_IMPL(Let)
{
    auto const if_str_key = rhs().if_string();
    ASSERT(if_str_key, "reference not a string");
    curr_ctx().captures->insert_or_assign(*if_str_key, lhs().data());
    return lhs();
}

ZMBT_DEFINE_EVALUATE_IMPL(Refer)
{
    auto const if_str_key = rhs().if_string();
    ASSERT(if_str_key, "reference not a string");
    auto const found = curr_ctx().captures->find(*if_str_key);
    if (found != curr_ctx().captures->cend())
    {
        return found->value();
    }
    else
    {
        return rhs();
    }
}


ZMBT_DEFINE_EVALUATE_IMPL(Link)
{
    auto const enc = self().encoding_view();
    auto const tuple = enc.child(0);
    ASSERT((enc.arity() == 1)
        && (tuple.head() == Keyword::Tuple)
        && (tuple.arity() == 2)
        , "invalid encoding");

    ExpressionView link (tuple.child(0));
    auto const if_str_link = link.if_string();
    ASSERT(if_str_link, "Symbolic reference shall be a string")

    auto const link_pos = curr_ctx().links->find(*if_str_link);
    ASSERT(link_pos == curr_ctx().links->cend(), "key refers to existing link")
    ASSERT(not curr_ctx().captures->contains(*if_str_link), "key refers to existing capture")

    ExpressionView referent(tuple.child(1));
    curr_ctx().links->emplace_hint(link_pos, *if_str_link, tuple.child(1));

    return referent.eval_e(lhs(), curr_ctx());
}

ZMBT_DEFINE_EVALUATE_IMPL(Capture)
{
    auto const if_str_key = self().if_string();
    ASSERT(if_str_key, "reference not a string");

    auto const link_pos = curr_ctx().links->find(*if_str_key);
    auto const capture_pos = curr_ctx().captures->find(*if_str_key);

    auto const link_found    = link_pos     != curr_ctx().links->cend();
    auto const capture_found = capture_pos  != curr_ctx().captures->cend();

    ASSERT(not (link_found && capture_found), "corrupted context")

    if (capture_found)
    {
        return capture_pos->value();
    }
    else if (link_found)
    {
        return link_pos->second.eval_e(lhs(), curr_ctx());
    }
    else
    {
        curr_ctx().captures->emplace(*if_str_key, lhs().data());
        return lhs();
    }
}



} // namespace lang
} // namespace zmbt

