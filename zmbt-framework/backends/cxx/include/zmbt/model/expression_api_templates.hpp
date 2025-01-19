/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MODEL_EXPRESSION_API_TEMPLATES_HPP_
#define ZMBT_MODEL_EXPRESSION_API_TEMPLATES_HPP_

#include <cstddef>
#include <initializer_list>
#include <boost/json.hpp>
#include "zmbt/model/keyword.hpp"
#include "zmbt/reflect/serialization.hpp"
#include "expression.hpp"

namespace zmbt {


namespace detail
{
    boost::json::value as_set(std::initializer_list<boost::json::value_ref> set);
}

/// Invariant Expression
template <Keyword k>
struct ExprNoParams : public Expression
{
    ExprNoParams()
        : Expression(k)
    {}
};

/// Expression with one parameter
template <Keyword k>
struct ExprOneParam : public Expression
{
    ExprOneParam()
        : Expression(k)
    {}

    Expression operator()(boost::json::value const& param) const
    {
        return Expression(k, param);
    }

    template <class T>
    Expression operator()(T&& param) const
    {
        return Expression(k, json_from(std::forward<T>(param)));
    }
};

/// Expression with one or two parameters
template <Keyword k>
struct ExprOneOrTwoParam : public Expression
{
    ExprOneOrTwoParam()
        : Expression(k)
    {}

    Expression operator()(V const& p0) const
    {
        return Expression(k, boost::json::array{p0});
    }
    Expression operator()(V const& p0, V const& p1) const
    {
        return Expression(k, {p0, p1});
    }
};

/// Expression with two parameters
template <Keyword k>
struct ExprTwoParam : public Expression
{
    ExprTwoParam()
        : Expression(k)
    {}

    Expression operator()(V const& p0, V const& p1) const
    {
        return Expression(k, {p0, p1});
    }
};

/// Expression with variadic parameters
template <Keyword k>
struct ExprVariadicParam : public Expression
{
    ExprVariadicParam()
        : Expression(k)
    {}

    Expression operator()(V const& p0) const {
        return Expression(k, boost::json::array{p0});
    }
    Expression operator()(V const& p0, V const& p1) const {
        return Expression(k, {p0, p1});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2) const {
        return Expression(k, {p0, p1, p2});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3) const {
        return Expression(k, {p0, p1, p2, p3});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4) const {
        return Expression(k, {p0, p1, p2, p3, p4});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5) const {
        return Expression(k, {p0, p1, p2, p3, p4, p5});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6) const {
        return Expression(k, {p0, p1, p2, p3, p4, p5, p6});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6, V const& p7) const {
        return Expression(k, {p0, p1, p2, p3, p4, p5, p6, p7});
    }
    template <class... T>
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6, V const& p7, T&&... rest) const {
        return Expression(k, {p0, p1, p2, p3, p4, p5, p6, p7, boost::json::value_from(rest)...});
    }
};

/// Expression with set-like parameter
template <Keyword Kw>
struct ExprSetParam : public Expression
{
    ExprSetParam()
        : Expression(Kw)
    {}

    Expression operator()(std::initializer_list<boost::json::value_ref> set) const
    {
        return Expression(Kw, detail::as_set(set));
    }

    Expression operator()(boost::json::array const& set) const
    {
        return Expression(Kw, set);
    }
    Expression operator()(boost::json::object const& set) const
    {
        return Expression(Kw, set);
    }
};


} // namespace zmbt

#endif // ZMBT_MAPPING_EXPRESSION_API_TEMPLATES_HPP_
