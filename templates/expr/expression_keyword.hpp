@require(keyword_groups)
/**
 * \file
 * \copyright (c) Copyright 2024 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/expression_keyword.hpp`
 * 2. Update the keyword data at `templates/expr/data.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:expression_keyword_hpp > packages/cxx/include/zmbt/model/expression_keyword.hpp`
 * 4. Commit changes
 */

#ifndef ZMBT_MODEL_EXPRESSION_KEYWORD_HPP_
#define ZMBT_MODEL_EXPRESSION_KEYWORD_HPP_

#include "zmbt/reflect.hpp"

#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {


enum class ExpressionKeyword
{
@for group in keyword_groups:
@for keyword in group['keywords']:

    @if comment := keyword.get('comment', None):
    @for line in comment.split('\n'):
    /// @line
    @end
    @end
    @keyword.get('enum', keyword['name'].capitalize()),
@end
@end
    /// Internal utility value
    Undefined
};


template<>
struct reflect::custom_serialization<ExpressionKeyword> {

    static boost::json::value json_from(ExpressionKeyword const& kw);
    static ExpressionKeyword dejsonize(boost::json::value const& v);
};

} // namespace zmbt



#endif // ZMBT_MAPPING_EXPRESSION_KEYWORD_HPP_