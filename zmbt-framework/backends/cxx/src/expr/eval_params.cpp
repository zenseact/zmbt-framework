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

EvalParams::EvalParams(ExpressionView const& e, ExpressionView const& x, EvalContext ctx)
    : lhs_maybe_owned_(Keyword::LazyToken)
    , rhs_maybe_owned_(nullptr)
    , lhs_{lhs_maybe_owned_.encoding_view()}
    , rhs_{rhs_maybe_owned_.encoding_view()}
    , self_{e}
    , x_{x}
    , ctx_{ctx}
{
    namespace attr = zmbt::lang::attr;
    auto const a = attributes(self().keyword());
    bool const is_binary = a & attr::is_binary;
    bool const is_variadic = a & attr::is_variadic;
    bool const is_single_form = a & attr::is_single_form;

    lhs_ = x;

    auto const if_arr = x.if_array();
    if (is_variadic && self().has_subexpr())
    {
        // self().subexpressions_list() is used directly to avoid reserialization
    }
    else if (!is_binary || self().has_subexpr())
    {
        auto const child = self().encoding_view().child(0);

        if (!child.empty())
        {
            rhs_ = ExpressionView(child);
        }
    }
    else if (has_default_rhs())
    {
        rhs_ = expr::Id;
    }
    else if(is_single_form)
    {
        // keep null
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
