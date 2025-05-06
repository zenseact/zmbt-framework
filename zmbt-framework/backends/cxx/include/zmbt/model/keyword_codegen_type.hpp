/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_KEYWORD_CODEGEN_TYPE_HPP_
#define ZMBT_MODEL_KEYWORD_CODEGEN_TYPE_HPP_

#include <boost/json.hpp>
#include <tuple>

#include "zmbt/model/keyword.hpp"

namespace zmbt {
namespace dsl {
namespace detail {

bool isConst(Keyword const& k);
bool isHiOrd(Keyword const& k);
bool isVariadic(Keyword const& k);

/// Keyword classifier, allows to select eval implementation
enum class CodegenType
{
    None,
    Const,
    UnaryOp,
    BinaryOp,
    CodegenFn,
};

CodegenType getCodegenType(Keyword const& k);




} // namespace detail
} // namespace dsl
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_CODEGEN_TYPE_HPP_
