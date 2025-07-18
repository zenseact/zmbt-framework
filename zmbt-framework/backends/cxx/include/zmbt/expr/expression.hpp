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


namespace zmbt {
namespace lang {

class EvalContext;

/// Expression Language implementation class.
/// \details \see <A HREF="/user-guide/expressions/">Expression Language documentation</A>.
class Expression
{
public:
    using V = boost::json::value;
    using Keyword = lang::Keyword;

private:
    Encoding encoding_;

    class Subexpression;

    std::list<Subexpression> subexpressions() const;


    struct json_ctor_params;
    Expression(json_ctor_params&&);

    void handle_binary_args(V const& x, Expression &lhs, Expression &rhs) const;

    boost::json::value eval_Const(boost::json::value const&) const;
    boost::json::value eval_UnaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_BinaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_CodegenFn(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_HiOrd(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_Special(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_Variadic(boost::json::value const&, EvalContext const&) const;


  public:

    Encoding const& encoding() const
    {
        return encoding_;
    }

    virtual EncodingView encoding_view() const
    {
        return {encoding_};
    }

    explicit Expression(Encoding && encoding)
        : encoding_{std::move(encoding)}
    {
    }

    static Encoding encodeLiteral(boost::json::value const& params)
    {
        Encoding enc {};
        enc.push_back(Keyword::Literal, 0, params, nullptr);
        return enc;
    }

    static Encoding encodePreProc(boost::json::value const& params)
    {
        Encoding enc {};
        enc.push_back(Keyword::PreProc, 0, params, nullptr);
        return enc;
    }

    // Non-terminal expression
    static Encoding encodeNested(Keyword const& keyword, std::vector<Expression> const& subexpressions)
    {
        Encoding enc {};
        enc.push_back(keyword, 0, nullptr, nullptr);

        for (auto const& subexpr: subexpressions)
        {
            enc.append_to_root(subexpr.encoding());
        }
        return enc;
    }

    // Deserialize JSON
    Expression(boost::json::value const& expr);

    // construct Literal from JSON init list
    Expression(std::initializer_list<boost::json::value_ref> items);

    // construct from keyword
    explicit Expression(Keyword const& keyword);

    Expression();


    template<Keyword>
    class SignatureBase;

    template<Keyword K>
    explicit Expression(SignatureBase<K> const& other) : Expression(other.encoding()) {}

    template <class T>
    Expression(T const& sample) : Expression(json_from(sample)) {}

    friend std::ostream& operator<<(std::ostream& os, Expression const& expr);


    /// \brief Pipe expressions left-to-right
    /// \details Pipe functional expressions in composition,
    /// s.t. `a | b` is equivalent to `Pipe(a, b)`. \see zmbt::expr::Pipe
    friend Expression operator|(Expression const& lhs, Expression const& rhs);

    /// \brief Pack expression results into an array. \see zmbt::expr::Fork.
    friend Expression operator&(Expression const& lhs, Expression const& rhs);

    /// \brief Evaluate x to lhs expression.
    /// \details Equivalent to expr.eval(x).
    friend V operator*(Expression const& expr, Expression const& x);

    /// \brief Evaluate expression.
    /// \details Equivalent to expr.eval().
    friend V operator*(Expression const& expr);



    std::list<std::pair<std::string, std::string>> preprocessing_parameters() const
    {
        std::list<std::pair<std::string, std::string>> pp;

        if (is_preproc()) // root-level is serialized as string literal
        {
            pp.emplace_back(to_json().as_string().c_str(), "");
            return pp;
        }

        for(auto const item: encoding_view())
        {
            if ((Keyword::PreProc == item.keyword) && item.data)
            {
                pp.emplace_back(item.data->as_string().c_str(), zmbt::format("/data/%d", item.index));
            }
        }

        return pp;
    }

    ~Expression() = default;


    std::string serialize() const
    {
        return boost::json::serialize(this->operator boost::json::value());
    }

    boost::json::string_view keyword_to_str() const;

    bool operator==(Expression const& o) const
    {
        return (this == &o) || (encoding_view() == o.encoding_view());
    }

    bool operator!=(Expression const& o) const
    {
        return !operator==(o);
    }

    Keyword keyword() const
    {
        return encoding_view().head();
    }

    boost::json::value const& data() const
    {
        auto const child = encoding_view().child(0);
        auto const a = attributes(child.head());
        if (!child.empty() && ((a & attr::is_literal) || (a & attr::is_preproc)))
        {
            return *child.front().data;
        }
        return *encoding_view().front().data;
    }

    Expression subexpr() const
    {
        auto const child = encoding_view().child(0);
        return child.empty() ? nullptr : Expression(child.freeze());
    }


    bool has_params() const
    {
        return has_subexpr() || is_literal();
    }

    bool has_subexpr() const
    {
        return encoding_view().size() > 1;
    }

    bool is(Keyword const kwrd) const
    {
        return kwrd == keyword();
    }

    bool is_compose() const
    {
        return is(Keyword::Pipe);
    }

    bool is_nonempty_composition() const
    {
        return is_compose() && has_subexpr();
    }

    bool is_fork() const
    {
        return is(Keyword::Fork);
    }

    std::size_t infix_size() const
    {
        std::size_t n {0};
        for (auto const item: encoding_view())
        {
            if (item.depth == 1) ++n;
        }
        return n;
    }

    bool is_infix_pipe() const
    {
        return is_compose() && (infix_size() > 1);
    }

    bool is_infix_fork() const
    {
        return is_fork() && (infix_size() > 1);
    }

    bool is_nonempty_fork() const
    {
        return is_fork() && has_subexpr();
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

    bool is_const() const;

    bool is_boolean() const;

    bool is_error() const
    {
        return is(Keyword::Err);
    }

    virtual boost::json::value to_json() const;

    operator boost::json::value() const
    {
        return to_json();
    }

    bool check_terms(std::uint32_t const attr_mask) const;

    /// @brief Evaluate expression
    /// @param x run-time argument
    /// @param config evaluation config
    /// @return
    boost::json::value eval(boost::json::value const& x, EvalContext const& ctx) const;
    boost::json::value eval(boost::json::value const& x = nullptr) const;

    /// Eval const expressions as Eq(expr), except for Noop,
    /// otherwise eval expr
    boost::json::value  eval_as_predicate(boost::json::value const& x, EvalContext const& ctx) const;

    static bool to_predicate_if_const(Expression& e);

    bool match(boost::json::value const& observed, Operator const& op = {}) const;

    std::string prettify() const;
    std::ostream& prettify_to(std::ostream& os) const;

    template <std::size_t N>
    void prettify_to(char (&buff)[N]) const
    {
        return prettify_to(buff, N);
    }

    void prettify_to(char* buff, std::size_t n) const;



    std::list<Expression> parameter_list() const;

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
