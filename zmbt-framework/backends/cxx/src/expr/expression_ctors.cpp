/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <cstdint>
#include <ostream>
#include <sstream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/attributes.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/api.hpp"


namespace zmbt {
namespace lang {


Expression::Expression() : Expression(nullptr)
{
}

Expression::Expression(Encoding&& encoding)
    : encoding_{std::move(encoding)}
{
}

Expression::Expression(Encoding const& encoding)
    : encoding_{encoding}
{
}

Expression::Expression(Keyword const& keyword)
    : Expression(encodeNested(keyword, {}))
{
    if (keyword == Keyword::Literal || keyword == Keyword::PreProc)
    {
        throw zmbt::expression_error("expression requires a value");
    }
}

Expression::Expression(boost::json::value const& expr)
    : encoding_{expr}
{
}

Expression::Expression(boost::json::value&& expr)
    : encoding_{std::move(expr)}
{
}

Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(boost::json::value(items))
{
}

Expression::Expression(Expression const& other) : Expression(other.encoding_)
{
}

Expression::Expression(Expression&& other) : Expression(std::move(other.encoding_))
{
}

Expression& Expression::operator=(Expression const& other)
{
    encoding_ = other.encoding_;
    return *this;
}

Expression& Expression::operator=(Expression&& other)
{
    encoding_ = std::move(other.encoding_);
    return *this;
}

}  // namespace lang
}  // namespace zmbt
