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
    Expression lhs_maybe_owned_{nullptr};
    Expression rhs_maybe_owned_{nullptr};
    std::reference_wrapper<Expression const> lhs_{lhs_maybe_owned_};
    std::reference_wrapper<Expression const> rhs_{rhs_maybe_owned_};
    std::reference_wrapper<Expression const> self_;
    std::reference_wrapper<Expression const> x_;
    EvalContext ctx_;


    bool has_default_rhs() const;

  public:

    EvalParams(Expression const& e, Expression const& x, EvalContext ctx);

    Expression const& self() const { return self_; }
    Expression const& x()    const { return x_; }
    Expression const& lhs()  const { return lhs_; }
    Expression const& rhs()  const { return rhs_; }
    EvalContext context()  const { return ctx_; }

  protected:

};

template <Keyword K>
struct FixedEvalParams : public EvalParams, public KeywordInfo<K>
{
    using EvalParams::EvalParams;
};


}  // namespace lang
}  // namespace zmbt

#endif
