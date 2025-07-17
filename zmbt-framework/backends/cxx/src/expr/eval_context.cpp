/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/eval_context.hpp"


namespace zmbt {
namespace lang {

EvalContext EvalContext::operator++(int) const
{
    return {op, log, depth + 1};
}

}  // namespace lang
}  // namespace zmbt
