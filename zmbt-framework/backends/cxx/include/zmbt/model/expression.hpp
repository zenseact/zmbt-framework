/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_EXPRESSION_V2_HPP_
#define ZMBT_MODEL_EXPRESSION_V2_HPP_

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "signal_operator_handler.hpp"
#include "keyword.hpp"
#include "keyword_grammar.hpp"
#include "keyword_classifier.hpp"
#include "exceptions.hpp"


namespace zmbt {

/// JSON transformation expression base
class Expression
{
public:
    using V = boost::json::value;
    using Keyword = expr::Keyword;

private:
    Keyword keyword_;
    boost::json::value underlying_;
    boost::json::value const* params_ptr_;
    boost::json::value const dummy_params_{};

    struct internal_tag{};
    Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying);

    struct json_ctor_params;
    Expression(json_ctor_params&&);

    void handle_terminal_binary_args(V const& x, V const*& lhs, V const*& rhs) const;

    boost::json::value eval_Const(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_UnaryOp(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_UnaryMathFn(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_BinaryOp(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_Special(boost::json::value const&, SignalOperatorHandler const&) const;

public:

    static boost::json::array toArray(std::initializer_list<Expression> const& list);

    Expression(std::initializer_list<boost::json::value_ref> items);
    Expression(boost::json::value const& expr);
    Expression(Keyword const& keyword, boost::json::value const& params);
    explicit Expression(Keyword const& keyword);

    Expression();

    template <class T>
    Expression(T const& sample) : Expression(json_from(sample)) {}

    Expression(Expression const& o)
    {
        keyword_ = o.keyword_;
        underlying_ = o.underlying_;
        params_ptr_ = o.params_ptr_;
    }

    Expression(Expression && o)
    {
        keyword_ = o.keyword_;
        underlying_ = std::move(o.underlying_);
        params_ptr_ = o.params_ptr_;
    }

    Expression& operator=(Expression const& o)
    {
        keyword_ = o.keyword_;
        underlying_ = o.underlying_;
        params_ptr_ = o.params_ptr_;
        return *this;
    }

    Expression& operator=(Expression && o)
    {
        keyword_ = o.keyword_;
        underlying_ = std::move(o.underlying_);
        params_ptr_ = o.params_ptr_;
        return *this;
    }

    /// \brief Compose expressions left-to-right
    /// \details Pipe functional expressions in composition,
    /// s.t. `a | b` is equivalent to `Compose(b, a)`.
    friend Expression operator|(Expression const& lhs, Expression const& rhs);

    /// \brief Apply x to lhs expression.
    /// \details Equivalent to Apply(expr, x).
    /// Note that operator <<= precedence is lower than pipe operator,
    /// so `a <<= b` should be wrapped in parentheses when followed by |.
    friend Expression operator<<=(Expression const& expr, boost::json::value const& x);

    template <class T>
    friend Expression operator<<=(Expression const& lhs, T const& rhs)
    {
        return lhs <<= json_from(rhs);
    }




    ~Expression() = default;

    boost::json::value const& underlying() const
    {
        return underlying_;
    }

    boost::json::value const& serialize() const
    {
        return underlying();
    }

    bool operator==(Expression const& o) const
    {
        return (keyword_ == o.keyword_) && (underlying_ == o.underlying_);
    }

    bool operator!=(Expression const& o) const
    {
        return !operator==(o);
    }

    Keyword keyword() const
    {
        return keyword_;
    }

    boost::json::value const& params() const
    {
        return has_params() ? *params_ptr_ : dummy_params_;
    }

    boost::json::value const& subexpr() const
    {
        return params();
    }

    bool has_params() const
    {
        return nullptr != params_ptr_;
    }

    bool is(Keyword const kwrd) const
    {
        return kwrd == keyword_;
    }

    operator boost::json::value() const
    {
        return underlying();
    }


    bool match(boost::json::value const& observed, SignalOperatorHandler const& op = {}) const;

    boost::json::value eval(boost::json::value const& x = nullptr, SignalOperatorHandler const& op = {}) const;

};

template<class T>
struct reflect::custom_serialization<T, mp_if<is_base_of<Expression, T>, void>> {

    static boost::json::value
    json_from(T const& t)
    {
        return t.underlying();
    }

    static T
    dejsonize(boost::json::value const& v)
    {
        return static_cast<T>(Expression(v));
    }

};

}  // namespace zmbt

#endif
