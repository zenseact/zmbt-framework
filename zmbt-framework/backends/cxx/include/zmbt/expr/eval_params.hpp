/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EVAL_PARAMS_HPP_
#define ZMBT_EXPR_EVAL_PARAMS_HPP_

#include "expression.hpp"
#include "keyword_info.hpp"
#include "eval_context.hpp"


namespace zmbt {
namespace lang {

class EvalParams
{
    Expression lhs_maybe_owned_;
    Expression rhs_maybe_owned_;
    ExpressionView lhs_;
    ExpressionView rhs_;
    ExpressionView const& self_;
    ExpressionView const& x_;
    EvalContext ctx_;

    bool has_default_rhs() const;

  public:

    EvalParams(ExpressionView const& e, ExpressionView const& x, EvalContext ctx);

    ExpressionView const& self() const { return self_; }
    ExpressionView const& x()    const { return x_; }
    ExpressionView const& lhs()  const { return lhs_; }
    ExpressionView const& rhs()  const { return rhs_; }
    EvalContext context()  const { return ctx_; }

};

template <Keyword K>
struct FixedEvalParams : public EvalParams, public KeywordInfo<K>
{
    using EvalParams::EvalParams;
};


}  // namespace lang
}  // namespace zmbt

#endif
