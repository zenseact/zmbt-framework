/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_
#define ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_

#include <boost/json.hpp>
#include <tuple>

#include "zmbt/model/keyword.hpp"

namespace zmbt {
namespace expr {
namespace detail {

/// Keyword classifier, allows to select eval implementation
enum class Classifier
{
    Internal,
    Const,
    UnaryOp,
    UnaryMathFn,
    BinaryOp,
    Special
};

Classifier getKeywordClassifier(Keyword const& k);

} // namespace detail
} // namespace expr
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_CLASSIFIER_HPP_
