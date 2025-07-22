/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EXPRESSION_V2_HPP_
#define ZMBT_EXPR_EXPRESSION_V2_HPP_

#include <ostream>

#include "zmbt/core.hpp"
#include "zmbt/logging.hpp"
#include "zmbt/reflect.hpp"
#include "operator.hpp"
#include "keyword.hpp"
#include "keyword_grammar.hpp"
#include "encoding.hpp"
#include "eval_context.hpp"


namespace zmbt {
namespace lang {


/// Expression Language implementation class.
/// \details \see <A HREF="/user-guide/expressions/">Expression Language documentation</A>.
class Expression
{
    static Expression unfold_left_assoc(Keyword const keyword, Expression&& lhs, Expression&& rhs);
    template <class T>
    static Encoding encodeNested(Keyword const& keyword, std::move_iterator<T> begin, std::move_iterator<T> const end);

public:

    using V = boost::json::value;
    using Keyword = lang::Keyword;

    //////////////////
    // STATIC FUNCS
    //////////////////


    // Terminal expression
    static Encoding encodeLiteral(boost::json::value const& params);
    // Terminal expression
    static Encoding encodePreProc(boost::json::value const& params);

    // Non-terminal expression
    static Encoding encodeNested(Keyword const& keyword, std::initializer_list<Expression> subexpressions);
    static Encoding encodeNested(Keyword const& keyword, std::vector<Expression>&& subexpressions);

    static bool to_predicate_if_const(Expression& e);


    //////////////////
    // CTORS
    //////////////////

    Expression();

    explicit Expression(Encoding && encoding);
    explicit Expression(Encoding const& encoding);

    // Deserialize JSON
    Expression(boost::json::value const& expr);
    Expression(boost::json::value && expr);

    // construct Literal from JSON init list
    Expression(std::initializer_list<boost::json::value_ref> items);

    // construct from keyword
    explicit Expression(Keyword const& keyword);

    Expression(Expression const& other);
    Expression(Expression&& other);
    Expression& operator=(Expression const& other);
    Expression& operator=(Expression&& other);
    ~Expression() = default;


    template <class T>
    Expression(T sample) : Expression(json_from(std::move(sample))) {}

    //////////////
    // OPERATORS
    //////////////

    friend std::ostream& operator<<(std::ostream& os, Expression const& expr);
    friend zmbt::Logger& operator<<(zmbt::Logger& logger, Expression const& expr);

    /// \brief Pipe expressions left-to-right
    /// \details Pipe functional expressions in composition,
    /// s.t. `a | b` is equivalent to `Pipe(a, b)`. \see zmbt::expr::Pipe
    friend Expression operator|(Expression lhs, Expression rhs);

    /// \brief Pack expression results into an array. \see zmbt::expr::Fork.
    friend Expression operator&(Expression lhs, Expression rhs);

    /// \brief Pack expression into an array. without evaluation \see zmbt::expr::Tuple.
    friend Expression operator+(Expression lhs, Expression rhs);

    friend Expression operator<=(Expression link, Expression referent);


    /// \brief Evaluate x to lhs expression.
    /// \details Equivalent to expr.eval(x).
    friend V operator*(Expression expr, Expression const& x);

    /// \brief Evaluate expression.
    /// \details Equivalent to expr.eval().
    friend V operator*(Expression expr);

    bool operator==(Expression const& o) const
    {
        return (this == &o) || (encoding_view() == o.encoding_view());
    }

    bool operator!=(Expression const& o) const
    {
        return !operator==(o);
    }

    //////////////////////
    // ENCODING OBSERVERS
    //////////////////////

    Encoding const& encoding() const
    {
        return encoding_;
    }

    virtual EncodingView encoding_view() const
    {
        return {encoding_};
    }


    std::size_t infix_size() const;

    bool has_subexpr() const
    {
        return encoding_view().size() > 1;
    }

    /// Subexpressions
    std::list<Expression> subexpressions_list() const;


    //////////////////
    // DATA OBSERVERS
    //////////////////

    boost::json::value const& data() const;

    boost::json::string const& as_string() const
    {
        return data().as_string();
    }

    boost::json::array const& as_array() const
    {
        return data().as_array();
    }

    boost::json::object const& as_object() const
    {
        return data().as_object();
    }

    bool as_bool() const
    {
        return data().as_bool();
    }

    boost::json::string const* if_string() const
    {
        return data().if_string();
    }

    boost::json::array const* if_array() const
    {
        return data().if_array();
    }

    boost::json::object const* if_object() const
    {
        return data().if_object();
    }

    bool const* if_bool() const
    {
        return data().if_bool();
    }

    bool is_null() const
    {
        return data().is_null();
    }


    std::string serialize() const
    {
        return boost::json::serialize(to_json());
    }

    boost::json::string_view keyword_to_str() const;

    ////////////////////////
    // KEYWORD OBSERVERS
    ////////////////////////


    Keyword keyword() const
    {
        return encoding_view().head();
    }

    bool is(Keyword const kwrd) const
    {
        return kwrd == keyword();
    }

    bool is_compose() const
    {
        return is(Keyword::Pipe);
    }

    bool is_fork() const
    {
        return is(Keyword::Fork);
    }

    bool is_tuple() const
    {
        return is(Keyword::Tuple);
    }

    bool is_literal() const
    {
        return is(Keyword::Literal);
    }

    bool is_preproc() const
    {
        return is(Keyword::PreProc);
    }

    bool is_noop() const
    {
        return is(Keyword::Noop);
    }

    bool is_error() const
    {
        return is(Keyword::Err);
    }


    ///////////////////////////
    // ATTR-BASED OBSERVERS
    ///////////////////////////

    bool is_const() const;

    bool is_boolean() const;

    bool is_infix_pipe() const
    {
        return is_compose() && (infix_size() > 1);
    }

    bool is_infix_fork() const
    {
        return is_fork() && (infix_size() > 1);
    }

    bool is_infix_tuple() const
    {
        return is_tuple() && (infix_size() > 1);
    }

    virtual boost::json::value to_json() const;

    explicit operator boost::json::value() const
    {
        return to_json();
    }

    ///////////////////////////
    // Boost.Spirit Karma generator
    ///////////////////////////

    std::string prettify() const;
    std::ostream& prettify_to(std::ostream& os) const;

    template <std::size_t N>
    void prettify_to(char (&buff)[N]) const
    {
        return prettify_to(buff, N);
    }

    void prettify_to(char* buff, std::size_t n) const;



    ///////////////////
    // EVALUATORS
    ///////////////////

    /// @brief Evaluate expression
    /// @param x run-time argument
    /// @param ctx evaluation context
    /// @return
    Expression eval_e(Expression const& x, EvalContext ctx) const;

    /// @brief Evaluate expression
    /// @param x run-time argument
    /// @param ctx evaluation context
    /// @return
    boost::json::value eval(Expression const& x = {}, EvalContext ctx = {}) const;

    /// Eval const expressions as Eq(expr), except for Noop,
    /// otherwise eval expr
    boost::json::value  eval_as_predicate(Expression const& x, EvalContext ctx) const;

    /// Eval and cast to boolean, return false on error
    bool match(boost::json::value const& observed, Operator const& op = {}) const;


    ////////////////////////
    // PREPROCESSING
    ////////////////////////

    /// List of [param, json ptr]
    std::list<std::pair<std::string, std::string>> preprocessing_parameters() const;


private:
    Encoding encoding_;


};

ZMBT_INJECT_SERIALIZATION
}  // namespace lang

template<class T>
struct reflect::custom_serialization<T, mp_if<is_base_of<lang::Expression, T>, void>> {

    static boost::json::value
    json_from(T const& t)
    {
        return t.to_json();
    }

    static T
    dejsonize(boost::json::value const& v)
    {
        return static_cast<T>(lang::Expression(v));
    }
};

}  // namespace zmbt


#endif
