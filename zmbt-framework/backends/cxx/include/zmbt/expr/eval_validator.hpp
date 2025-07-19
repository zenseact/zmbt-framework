/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_EVAL_VALIDATOR_HPP_
#define ZMBT_EXPR_EVAL_VALIDATOR_HPP_

#include "expression.hpp"
#include "keyword.hpp"


namespace zmbt {
namespace lang {

template <Keyword K>
struct EvalValidator
{

    EvalValidator(Expression const& lhs, Expression const& rhs) {}

    bool is_invalid() const
    {
        return false;
    }

    Expression status() const
    {
        return {nullptr};
    }
};

}  // namespace lang
}  // namespace zmbt

#endif
