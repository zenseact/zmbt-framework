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


ZMBT_DEFINE_EVALUATE_IMPL(Fn)
{
    auto const enc = self().encoding_view();
    ASSERT(enc.arity() == 2, "invalid encoding");

    ExpressionView link (enc.child(0));
    auto const if_str_link = link.if_string();
    ASSERT(if_str_link, "Symbolic reference shall be a string")

    auto const link_pos = curr_ctx().expr_links->find(*if_str_link);
    ASSERT(link_pos == curr_ctx().expr_links->cend(), "key refers to existing link")
    for (auto const& scope_captures: *curr_ctx().capture_links)
    {
        ASSERT(not scope_captures.contains(*if_str_link), "key refers to existing capture")
    }

    ExpressionView referent(enc.child(1));
    curr_ctx().expr_links->emplace_hint(link_pos, *if_str_link, enc.child(1));

    return referent.eval_e(lhs(), curr_ctx());
}


ZMBT_DEFINE_EVALUATE_IMPL(Get)
{
    auto const if_str_key = rhs().if_string();
    ASSERT(if_str_key, "reference not a string");

    boost::json::value const* capture_found{};

    auto capture_links_it = std::make_reverse_iterator(curr_ctx().capture_links->cend());
    auto capture_links_start = capture_links_it;
    auto const capture_links_end = std::make_reverse_iterator(curr_ctx().capture_links->cbegin());
    while (!capture_found && (capture_links_it != capture_links_end))
    {
        capture_found = capture_links_it->if_contains(*if_str_key);
        capture_links_it++;
    }

    // load upvalue into local scope to prevent shadowing
    if (capture_found && (capture_links_start != capture_links_it))
    {
        curr_ctx().capture_links->back().insert_or_assign(*if_str_key, *capture_found);
    }

    return capture_found ? *capture_found : nullptr;

}


ZMBT_DEFINE_EVALUATE_IMPL(Link)
{
    auto if_str_key = self().if_string();
    if (!if_str_key) if_str_key = rhs().if_string();
    ASSERT(if_str_key, "reference not a string");

    ExpressionView const* symlink_found{};
    {
        auto const link_pos = curr_ctx().expr_links->find(*if_str_key);
        if(link_pos != curr_ctx().expr_links->cend())
        {
            symlink_found = &link_pos->second;
        }
    }


    auto const capture_found = curr_ctx().capture_links->back().if_contains(*if_str_key);

    ASSERT(not (symlink_found && capture_found), "corrupted context")

    if(capture_found && lhs().is(Keyword::LazyToken))
    {
        return *capture_found;
    }
    else if (symlink_found)
    {
        curr_ctx().capture_links->push_back({});
        auto const result = symlink_found->eval_e(lhs(), curr_ctx());
        curr_ctx().capture_links->pop_back();
        return result;
    }
    else
    {
        curr_ctx().capture_links->back().insert_or_assign(*if_str_key, lhs().data());
        return lhs();
    }
}



} // namespace lang
} // namespace zmbt

