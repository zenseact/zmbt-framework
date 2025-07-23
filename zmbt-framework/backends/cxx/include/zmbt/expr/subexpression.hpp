/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_SUBEXPRESSION_HPP_
#define ZMBT_EXPR_SUBEXPRESSION_HPP_

#include <ostream>

#include "expression.hpp"


namespace zmbt {
namespace lang {


class Expression::Subexpression : Expression
{
    EncodingView ev_;
public:
    Subexpression(EncodingView const& ev)
        : ev_{ev}
    {
    }
};

}  // namespace lang
}  // namespace zmbt

#endif
