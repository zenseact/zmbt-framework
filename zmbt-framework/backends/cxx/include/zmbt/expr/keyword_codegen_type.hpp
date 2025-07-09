/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_
#define ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_

#include <boost/json.hpp>
#include <tuple>

#include "keyword.hpp"

namespace zmbt {
namespace lang {
namespace detail {

bool isConst(Keyword const& k);
bool isHiOrd(Keyword const& k);
bool isVariadic(Keyword const& k);
bool isTernary(Keyword const& k);

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
} // namespace lang
} // namespace zmbt



#endif // ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_
