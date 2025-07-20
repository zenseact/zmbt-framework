/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/eval_params.hpp"

#include "zmbt/expr/api.hpp"

namespace zmbt {
namespace lang {

EvalParams::EvalParams(Expression const& e, Expression const& x) : self_{e}, x_{x}
{
    namespace attr = zmbt::lang::attr;
    auto const a = attributes(self().keyword());
    bool const is_binary = a & attr::is_binary;
    bool const is_variadic = a & attr::is_variadic;

    lhs_ = x;

    auto const if_arr = x.if_array();
    if (is_variadic && self().has_subexpr())
    {
        // self().parameter_list() is used directly to avoid reserialization
    }
    else if (!is_binary || self().has_subexpr())
    {
        rhs_ = rhs_maybe_owned_ = self().subexpr();
    }
    else if (has_default_rhs())
    {
        rhs_ = expr::Id;
    }
    else if (x.is_literal() && if_arr && if_arr->size() == 2)
    {
        lhs_ = lhs_maybe_owned_ = if_arr->front();
        rhs_ = rhs_maybe_owned_ = if_arr->back();
    }
    else
    {
        // keep rhs nullptr
    }
}

bool EvalParams::has_default_rhs() const
{
    switch (self().keyword()) {
        case Keyword::Sort:
        case Keyword::Min:
        case Keyword::Max:
        case Keyword::Argmin:
        case Keyword::Argmax: {
            return true;
        }
        default: {
            return false;
        }
    }
}

}  // namespace lang
}  // namespace zmbt
