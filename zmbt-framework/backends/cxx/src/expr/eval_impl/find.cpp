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

ZMBT_DEFINE_EVALUATE_IMPL(Find)     { return expr::Err("not implemented"); }
ZMBT_DEFINE_EVALUATE_IMPL(FindPtr)  { return expr::Err("not implemented"); }
ZMBT_DEFINE_EVALUATE_IMPL(FindIdx)  { return expr::Err("not implemented"); }

} // namespace lang
} // namespace zmbt

