/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/logging.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"
#include "zmbt/model/exceptions.hpp"
#include "zmbt/model/keyword_classifier.hpp"


#include <boost/regex.hpp>

#define ASSERT(E)      if (!(E)) { throw zmbt::expression_error("%s#%d - " #E, __FILE__, __LINE__);}

namespace
{

using V = boost::json::value;
using O = zmbt::SignalOperatorHandler;
using E = zmbt::Expression;
using Keyword = zmbt::expr::Keyword;
}

namespace zmbt {

boost::json::value Expression::eval(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    if(is(Keyword::Literal))
    {
        return Expression(Keyword::Eq, params()).eval(x, op);
    }

    using expr::detail::Classifier;
    using expr::detail::getKeywordClassifier;
    Classifier const classifier = getKeywordClassifier(keyword());

    switch (classifier)
    {
    case Classifier::Const:         return eval_Const(x, op);
    case Classifier::UnaryOp:       return eval_UnaryOp(x, op);
    case Classifier::UnaryMathFn:   return eval_CodegenFn(x, op);
    case Classifier::BinaryOp:      return eval_BinaryOp(x, op);
    case Classifier::Special:
    default:
        return eval_Special(x, op);
    }
}

} // namespace zmbt

