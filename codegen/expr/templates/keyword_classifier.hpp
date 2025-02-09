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

#ifndef ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_
#define ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_

#include <boost/json.hpp>
#include <tuple>

#include "zmbt/model/keyword.hpp"

namespace zmbt {
namespace expr {

enum class Category
{
@for cat in {keyword.Category: None for keyword in data.Keywords}:
    @cat,
@end
    Undefined
};


enum class Signature
{
@for sig in {keyword.Signature: None for keyword in data.Keywords}:
    @sig,
@end
    Undefined
};


using KeywordClassifier = std::tuple<Category, Signature>;

KeywordClassifier getKeywordClassifier(Keyword const& k);

} // namespace expr
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_
