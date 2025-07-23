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

ZMBT_DEFINE_EVALUATE_IMPL(Link)     { return expr::Err("not implemented"); }
ZMBT_DEFINE_EVALUATE_IMPL(Refer)    { return expr::Err("not implemented"); }

ZMBT_DEFINE_EVALUATE_IMPL(Capture)
{
    if (not curr_ctx().captures)
    {
        return rhs();
    }
    auto const key = self().data();
    ASSERT(key.is_string(), "invalid reference");
    auto const referent = curr_ctx().captures->if_contains(key.get_string());
    if (referent)
    {
        return *referent;
    }
    else
    {
        curr_ctx().captures->emplace(key.get_string(), lhs().data());
        return lhs();
    }
}



} // namespace lang
} // namespace zmbt

