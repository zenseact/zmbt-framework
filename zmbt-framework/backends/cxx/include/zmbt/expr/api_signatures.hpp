/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_
#define ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_

#include <cstddef>
#include <initializer_list>
#include <boost/json.hpp>
#include "zmbt/reflect/serialization.hpp"
#include "keyword.hpp"
#include "expression.hpp"

namespace zmbt {
namespace lang {
namespace detail {
    boost::json::value handle_list_init(std::initializer_list<Expression> set);
}

template <Keyword K>
struct SignatureBase : public Expression
{
    SignatureBase() : Expression(K)
    {}
};

/// @brief Const expression
/// @tparam K keyword
/// \anchor const-syntactic-forms
/// \paragraph par-const-syntactic-forms Syntactic forms
/// Syntatic form over constant C: \f$E \mapsto (x \mapsto C)\f$
template <Keyword K>
struct SignatureConst : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

/// @brief Unary expression
/// @tparam K keyword
/// \anchor unary-syntactic-forms
/// \paragraph par-unary-syntactic-forms Syntactic forms
/// Syntatic form over function f: \f$E \mapsto (x \mapsto f(x))\f$
template <Keyword K>
struct SignatureUnary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

/// @brief Binary expression
/// @tparam K keyword
/// \anchor binary-syntactic-forms
/// \paragraph par-binary-syntactic-forms Syntactic forms
/// Syntatic forms over operator `*`:
/// 1. \f$E    \mapsto ([x, y] \mapsto x * y      )\f$
/// 2. \f$E    \mapsto (x      \mapsto x * default)\f$
/// 3. \f$E(y) \mapsto (x      \mapsto x * y      )\f$
///
/// The second form is used in place of first for expressions that have default value defined,
/// see documentation for specific expression.
///
/// In the third form the expression parameter is used as the right-hand side operand.
template <Keyword K>
struct SignatureBinary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;

    /// \brief Make parametrized expression
    Expression operator()(Expression const& param) const
    {
        return Expression(K, param);
    }

    /// \brief Make parametrized expression with initializer list
    /// \details Interpret {x} as single-element array instead of using default boost::json::value ctor
    Expression operator()(std::initializer_list<Expression> param) const
    {
        return Expression(K, detail::handle_list_init(param));
    }
};

/// @brief Ternary expression
/// @tparam K keyword
/// \anchor ternary-syntactic-forms
/// \paragraph par-ternary-syntactic-forms Syntactic forms
/// Syntatic form over function f: \f$E(a, b) \mapsto (x \mapsto f(a, b)(x))\f$
template <Keyword K>
struct SignatureTernary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;

    Expression operator()(Expression const& expr, Expression const& param) const
    {
        return Expression(K, boost::json::array{expr, param});
    }
};


/// @brief Variadic expression
/// @tparam K keyword
/// \anchor variadic-syntactic-forms
/// \paragraph par-variadic-syntactic-forms Syntactic forms
/// Syntatic form over function f: \f$E(a,b,c,...) \mapsto (x \mapsto f(a,b,c,...)(x))\f$
template <Keyword K>
struct SignatureVariadic : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    using E = Expression;
    Expression operator()() const {
        return Expression(K, boost::json::array{});
    }
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




struct SignatureOverload : public SignatureTernary<Keyword::Overload>
{
    using SignatureTernary<Keyword::Overload>::SignatureTernary;
    using SignatureTernary<Keyword::Overload>::operator();

    template <class T>
    Expression operator()(Expression const& expr, type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Keyword::Overload, {expr, op.annotation()});
    }
};

struct SignatureDecorate : public SignatureBinary<Keyword::Decorate>
{
    using SignatureBinary<Keyword::Decorate>::SignatureBinary;
    using SignatureBinary<Keyword::Decorate>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Keyword::Decorate, op.annotation());
    }
};

struct SignatureUndecorate : public SignatureBinary<Keyword::Undecorate>
{
    using SignatureBinary<Keyword::Undecorate>::SignatureBinary;
    using SignatureBinary<Keyword::Undecorate>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Keyword::Undecorate, op.annotation());
    }
};

} // namespace lang
} // namespace zmbt

#endif // ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_
