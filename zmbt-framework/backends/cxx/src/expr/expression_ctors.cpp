/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
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

namespace
{
zmbt::lang::EncodingView lazy_token_view()
{
    static zmbt::lang::Encoding const lazy_token = []{
        zmbt::lang::Encoding enc;
        enc.push_back(zmbt::lang::Keyword::LazyToken, 0, {});
        return enc;
    }();
    return zmbt::lang::EncodingView(lazy_token);
}
} // namespace


namespace zmbt {
namespace lang {

ExpressionView::ExpressionView()
    : ExpressionView(lazy_token_view())
{
}

Expression::Expression(Encoding&& encoding)
    : encoding_{std::move(encoding)}
{
    encoding_view_ = EncodingView(encoding_);
}

Expression::Expression(Encoding const& encoding)
    : encoding_{encoding}
{
    encoding_view_ = EncodingView(encoding_);
}

Expression::Expression(Keyword const& keyword)
    : Expression(encodeNested(keyword, {}))
{
}

Expression::Expression(ExpressionView const& view)
    : Expression(view.encoding_view().freeze())
{

}


Expression::Expression(boost::json::value const& expr)
    : Expression(Encoding(expr))
{
}

Expression::Expression(boost::json::value&& expr)
    : Expression(Encoding(std::move(expr)))
{
}


Expression::Expression()
    : Expression(nullptr)
{
}


Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(boost::json::value(items))
{
}

Expression::Expression(Expression const& other)
    : Expression(other.encoding_)
{
}

Expression::Expression(Expression&& other)
    : Expression(std::move(other.encoding_))
{
}

Expression& Expression::operator=(Expression const& other)
{
    encoding_ = other.encoding_;
    encoding_view_ = EncodingView(encoding_);
    return *this;
}

Expression& Expression::operator=(Expression&& other)
{
    encoding_ = std::move(other.encoding_);
    encoding_view_ = EncodingView(encoding_);
    return *this;
}

}  // namespace lang
}  // namespace zmbt
