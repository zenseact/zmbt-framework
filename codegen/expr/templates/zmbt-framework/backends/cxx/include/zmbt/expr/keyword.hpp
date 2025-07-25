@require(data)
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * app: @appname
 * template: @template
 *
 * How to update:
 * 1. Update the template
 * 2. Run `python -m codegen` in the project root
 * 3. Commit changes
 */

#ifndef ZMBT_EXPR_KEYWORD_HPP_
#define ZMBT_EXPR_KEYWORD_HPP_

#include <boost/json.hpp>

#include "zmbt/reflect.hpp"


namespace zmbt {
namespace lang {


/// ZMBT Expression keyword
enum class Keyword
{
    Undefined,
@for keyword in data.Keywords:

    /// @keyword.Name
    @keyword.Name,
@end

    _count
};

namespace detail
{
bool isBinary(Keyword const& k);
bool isBoolean(Keyword const& k);
} // namespace detail


} // namespace lang
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_HPP_
