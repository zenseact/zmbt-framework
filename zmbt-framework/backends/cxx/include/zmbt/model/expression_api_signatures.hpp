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
    Expression operator()(Expression const& param) const
    {
        return Expression(K, param);
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
    Expression operator()(Expression const& expr, Expression const& param) const
    {
        return Expression(K, boost::json::array{expr, param});
    }
};


/// Expression with variadic expression parameters
template <Keyword K>
struct SignatureVariadic : public SignatureBase<K>
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
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7, zmbt::json_from(rest)...});
    }
};



} // namespace expr
} // namespace zmbt

#endif // ZMBT_MODEL_EXPRESSION_API_SIGNATURES_HPP_
