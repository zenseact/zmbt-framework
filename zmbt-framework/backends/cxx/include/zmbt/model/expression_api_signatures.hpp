/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MODEL_EXPRESSION_API_SIGNATURES_HPP_
#define ZMBT_MODEL_EXPRESSION_API_SIGNATURES_HPP_

#include <cstddef>
#include <initializer_list>
#include <boost/json.hpp>
#include "zmbt/model/keyword.hpp"
#include "zmbt/reflect/serialization.hpp"
#include "expression.hpp"

namespace zmbt {
namespace expr {
namespace detail {
    boost::json::value as_set(std::initializer_list<boost::json::value_ref> set);
}

template <Keyword K>
struct SignatureBase : public Expression
{
    SignatureBase() : Expression(K)
    {}
};

template <Keyword K>
struct SignatureConst : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

template <Keyword K>
struct SignatureUnary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

template <Keyword K>
struct SignatureBinary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    Expression operator()(boost::json::value const& param) const
    {
        return Expression(K, param);
    }
    template <class T>
    Expression operator()(T&& param) const
    {
        return Expression(K, json_from(std::forward<T>(param)));
    }
};

template <Keyword K>
struct SignatureUnaryParam : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    Expression operator()(boost::json::value const& param) const
    {
        return Expression(K, param);
    }
    template <class T>
    Expression operator()(T&& param) const
    {
        return Expression(K, json_from(std::forward<T>(param)));
    }
};

/// Expression with variadic parameters
template <Keyword K>
struct SignatureVariadic : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    using V = boost::json::value;
    Expression operator()(V const& p0) const {
        return Expression(K, boost::json::array{p0});
    }
    Expression operator()(V const& p0, V const& p1) const {
        return Expression(K, {p0, p1});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2) const {
        return Expression(K, {p0, p1, p2});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3) const {
        return Expression(K, {p0, p1, p2, p3});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4) const {
        return Expression(K, {p0, p1, p2, p3, p4});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6});
    }
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6, V const& p7) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7});
    }
    template <class... T>
    Expression operator()(V const& p0, V const& p1, V const& p2, V const& p3, V const& p4, V const& p5, V const& p6, V const& p7, T&&... rest) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7, boost::json::value_from(rest)...});
    }
};

template <Keyword K>
struct SignatureHiOrd : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    Expression operator()(Expression const& expr) const
    {
        return Expression(K, expr);
    }
};

template <Keyword K>
struct SignatureHiOrdParam : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    Expression operator()(Expression const& expr, boost::json::value const& param) const
    {
        return Expression(K, boost::json::array{expr, param});
    }
    template <class T>
    Expression operator()(Expression const& expr, T&& param) const
    {
        return Expression(K, boost::json::array{expr, json_from(std::forward<T>(param))});
    }
};


template <Keyword K>
struct SignatureHiOrdParamOpt : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    Expression operator()(Expression const& expr) const
    {
        return Expression(K, boost::json::array{expr});
    }
    Expression operator()(Expression const& expr, boost::json::value const& param) const
    {
        return Expression(K, boost::json::array{expr, param});
    }
    template <class T>
    Expression operator()(Expression const& expr, T&& param) const
    {
        return Expression(K, boost::json::array{expr, json_from(std::forward<T>(param))});
    }
};


/// Expression with variadic expression parameters
template <Keyword K>
struct SignatureHiOrdVariadic : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    using E = Expression;
    Expression operator()(E const& p0) const {
        return Expression(K, boost::json::array{p0});
    }
    Expression operator()(E const& p0, E const& p1) const {
        return Expression(K, {p0, p1});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2) const {
        return Expression(K, {p0, p1, p2});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3) const {
        return Expression(K, {p0, p1, p2, p3});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4) const {
        return Expression(K, {p0, p1, p2, p3, p4});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7});
    }
    template <class... T>
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7, T&&... rest) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7, boost::json::value_from(rest)...});
    }
};

template <Keyword Kw>
struct SignatureBinarySetRhs : public SignatureBase<Kw>
{
    using SignatureBase<Kw>::SignatureBase;
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

struct SignatureApprox : public SignatureBase<Keyword::Approx>
{
    using SignatureBase<Keyword::Approx>::SignatureBase;

    Expression operator()(double reference, double rtol, double atol = std::numeric_limits<double>::epsilon()) const
    {
        return Expression(Keyword::Approx, boost::json::array {reference, rtol, atol});
    }
};

} // namespace expr
} // namespace zmbt

#endif // ZMBT_MODEL_EXPRESSION_API_SIGNATURES_HPP_
