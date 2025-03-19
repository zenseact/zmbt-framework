/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_EXPRESSION_V2_HPP_
#define ZMBT_MODEL_EXPRESSION_V2_HPP_

#include <ostream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "signal_operator_handler.hpp"
#include "keyword.hpp"
#include "keyword_grammar.hpp"
#include "keyword_codegen_type.hpp"
#include "exceptions.hpp"


namespace zmbt {

/// Expression DSL implementation class.
/// \details \see \ref expression-dsl "Expression DSL" documentation.
class Expression
{
public:
    using V = boost::json::value;
    using Keyword = dsl::Keyword;

    /// Expression evaluation log
    struct EvalLog
    {
        mutable std::shared_ptr<boost::json::array> stack;

        /// Default instance with null log stack
        EvalLog() = default;

        /// Stringify log
        std::string str() const;

        /// Push record to log stack
        void push(boost::json::value const& expr, boost::json::value const& x, boost::json::value const& result, int const depth) const;

        friend std::ostream& operator<<(std::ostream& os, EvalLog const& log);

        /// Make non-empty EvalLog
        static EvalLog make();
    };

    /// Expression evaluation config
    struct EvalConfig
    {
        /// Operator
        SignalOperatorHandler op;
        /// Evaluation log
        EvalLog log;
        /// Evaluation stack depth
        int const depth;

        /// Copy config with depth increment
        EvalConfig operator++(int) const;
    };

private:
    Keyword keyword_;
    boost::json::value underlying_;
    boost::json::value const* params_ptr_;
    boost::json::value const dummy_params_{};

    struct internal_tag{};
    Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying);

    struct json_ctor_params;
    Expression(json_ctor_params&&);

    void handle_binary_args(V const& x, V const*& lhs, V const*& rhs) const;

    boost::json::value eval_Const(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_UnaryOp(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_CodegenFn(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_BinaryOp(boost::json::value const&, SignalOperatorHandler const&) const;
    boost::json::value eval_Special(boost::json::value const&, EvalConfig const&) const;
public:

    static boost::json::array toArray(std::initializer_list<Expression> const& list);

    Expression(std::initializer_list<boost::json::value_ref> items);
    Expression(boost::json::value const& expr);
    Expression(Keyword const& keyword, boost::json::value const& params);
    explicit Expression(Keyword const& keyword);

    Expression();

    template <class T>
    Expression(T const& sample) : Expression(json_from(sample)) {}

    Expression(Expression const& o);
    Expression(Expression && o);
    Expression& operator=(Expression const& o);
    Expression& operator=(Expression && o);

    /// \brief Compose expressions left-to-right
    /// \details Pipe functional expressions in composition,
    /// s.t. `a | b` is equivalent to `Compose(b, a)`. \see zmbt::expr::Compose
    friend Expression operator|(Expression const& lhs, Expression const& rhs);

    /// \brief Concatenate expressions. \see zmbt::expr::Concat.
    friend Expression operator+(Expression const& lhs, Expression const& rhs);

    /// \brief Pack expression results into an array. \see zmbt::expr::Pack.
    friend Expression operator&(Expression const& lhs, Expression const& rhs);


    /// \brief Apply x to lhs expression.
    /// \details Equivalent to Apply(expr, x).
    /// Note that operator <<= precedence is lower than pipe operator,
    /// so `a <<= b` should be wrapped in parentheses when followed by |. \see zmbt::expr::Apply
    friend Expression operator<<=(Expression const& expr, Expression const& x);




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

    boost::json::value eval(boost::json::value const& x = nullptr, EvalConfig const& options = {}) const;
    bool match(boost::json::value const& observed, SignalOperatorHandler const& op = {}) const;

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
