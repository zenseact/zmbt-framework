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

boost::json::value Expression::eval_UnaryOp(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    return op.apply(keyword(), nullptr, x);
}

void Expression::handle_terminal_binary_args(V const& x, V const*& lhs, V const*& rhs) const
{
    if (has_params())
    {
        lhs = &x;
        rhs = params_ptr_;
    }
    else // treat x as argument pair
    {
        if (x.is_array() && x.get_array().size() == 2)
        {
            lhs = x.get_array().cbegin();
            rhs = lhs + 1;
        }
        else
        {
            throw expression_error("got invalid params for binary operator, should be pair");
        }
    }
}

boost::json::value Expression::eval_BinaryOp(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    V const* lhs {nullptr}; // binary LHS or functor params
    V const* rhs {nullptr}; // binary RHS or functor arg
    handle_terminal_binary_args(x, lhs, rhs);
    ASSERT(lhs)
    ASSERT(rhs)
    return op.apply(keyword(), *lhs, *rhs);
}


} // namespace zmbt

