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
    bool const is_binary = attributes(self().keyword()) & attr::is_binary;

    lhs_ = x;

    if (!is_binary | self().has_subexpr())
    {
        rhs_maybe_owned_ = self().subexpr();
    }
    else if (has_default_rhs())
    {
        rhs_ = expr::Id;
    }
    else if (x.is_fork() && (x.encoding_view().arity() == 2))
    {
        lhs_maybe_owned_ = Expression(x.encoding_view().child(0).freeze());
        rhs_maybe_owned_ = Expression(x.encoding_view().child(1).freeze());
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
