/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_KEYWORD_INFO_HPP_
#define ZMBT_EXPR_KEYWORD_INFO_HPP_

#include "keyword.hpp"
#include "attributes.hpp"


namespace zmbt {
namespace lang {

template <Keyword k>
struct KeywordInfo
{
    constexpr Keyword keyword() { return k; }
    constexpr std::uint32_t attributes() { constexpr std::uint32_t a = ::zmbt::lang::attributes(k); return a; }
    static boost::json::string_view keyword_to_str() { return ::zmbt::lang::keyword_to_str(k); }
};

}  // namespace lang
}  // namespace zmbt

#endif
