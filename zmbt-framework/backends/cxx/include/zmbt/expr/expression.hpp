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
#include "keyword_codegen_type.hpp"
#include "exceptions.hpp"


namespace zmbt {
namespace lang {

struct Encoding
{
    using K = Keyword;
    using V = boost::json::value;
    std::vector<K> keywords;
    std::vector<std::size_t> depth;
    std::vector<V> data;
    std::vector<V> bindings;
};

BOOST_DESCRIBE_STRUCT(Encoding, (void), (keywords, depth, bindings, data))


/// Expression Language implementation class.
/// \details \see <A HREF="/user-guide/expressions/">Expression Language documentation</A>.
class Expression
{
public:
    using V = boost::json::value;
    using Keyword = lang::Keyword;

    /// Expression evaluation log
    struct EvalLog
    {
        mutable std::shared_ptr<boost::json::array> stack;

        /// Default instance with null log stack
        EvalLog() = default;

        /// Stringify log
        boost::json::string str(int const indent = 0) const;

        /// Push record to log stack
        void push(Expression const& expr, boost::json::value const& x, boost::json::value const& result, std::uint64_t const depth) const;


        static void format(std::ostream& os, boost::json::array const& log, int const indent = 0);

        friend std::ostream& operator<<(std::ostream& os, EvalLog const& log);

        /// Make non-empty EvalLog
        static EvalLog make();
    };

    /// Expression evaluation context
    struct EvalContext
    {
        /// Operator
        Operator op;
        /// Evaluation log
        EvalLog log;
        /// Evaluation stack depth
        std::uint64_t const depth;

        /// Copy context with depth increment
        EvalContext operator++(int) const;
    };

private:
    Encoding encoding_;

    struct internal_tag{};
    Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying);

    struct json_ctor_params;
    Expression(json_ctor_params&&);

    void handle_binary_args(V const& x, V &lhs, V &rhs) const;

    boost::json::value eval_Const(boost::json::value const&) const;
    boost::json::value eval_UnaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_BinaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_CodegenFn(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_HiOrd(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_Special(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_Variadic(boost::json::value const&, EvalContext const&) const;

    Expression(internal_tag, Operator const& op);

    static void preprocess(Encoding& origin)
    {
        auto const keywords_end = origin.keywords.cend();
        auto it_keywords = origin.keywords.cbegin();

        if (std::find(it_keywords, keywords_end, Keyword::PreProc) == keywords_end)
        {
            return;
        }

        Encoding enc;
        enc.keywords.reserve(origin.keywords.size());
        enc.depth   .reserve(origin.depth   .size());
        enc.data    .reserve(origin.data    .size());
        enc.bindings.reserve(origin.bindings.size());

        auto it_depth = origin.depth.cbegin();
        auto it_data = origin.data.cbegin();
        auto it_bindings = origin.bindings.cbegin();

        while(it_keywords != keywords_end)
        {
            // no preproc recursion
            bool const is_preproc = *it_keywords == Keyword::PreProc;
            bool const is_unevaluated = it_data->is_string() && it_data->get_string().starts_with("$[") && it_data->get_string().ends_with("]");
            if (is_preproc && !is_unevaluated)
            {
                auto subenc = Expression(*it_data).encoding();
                for (auto& d: subenc.depth)
                {
                    d += *it_depth;
                }

                enc.keywords.reserve(subenc.keywords.size());
                enc.depth   .reserve(subenc.depth   .size());
                enc.data    .reserve(subenc.data    .size());
                enc.bindings.reserve(subenc.bindings.size());
                enc.keywords.insert(enc.keywords.end(), std::make_move_iterator(subenc.keywords.begin()), std::make_move_iterator(subenc.keywords.end()));
                enc.depth   .insert(enc.depth   .end(), std::make_move_iterator(subenc.depth   .begin()), std::make_move_iterator(subenc.depth   .end()));
                enc.data    .insert(enc.data    .end(), std::make_move_iterator(subenc.data    .begin()), std::make_move_iterator(subenc.data    .end()));
                enc.bindings.insert(enc.bindings.end(), std::make_move_iterator(subenc.bindings.begin()), std::make_move_iterator(subenc.bindings.end()));
            }
            else
            {
                enc.keywords.push_back(*it_keywords);
                enc.depth   .push_back(*it_depth);
                enc.data    .push_back(*it_data);
                enc.bindings.push_back(*it_bindings);
            }

            ++it_keywords;
            ++it_depth;
            ++it_data;
            ++it_bindings;
        }

        origin = enc;
    }

    static Encoding encode(boost::json::value const& value)
    {
        if (auto const if_obj = value.if_object())
        {
            if (if_obj->contains("keywords")
                && if_obj->contains("depth")
                && if_obj->contains("data")
                && if_obj->contains("bindings")
            )
            {
                auto enc = boost::json::value_to<Encoding>(value);
                preprocess(enc);
                return enc;
            }
        }
        auto k = Keyword::Literal;
        if (auto const if_str = value.if_string())
        {
            if(if_str->starts_with("$[") && if_str->ends_with("]"))
            {
                k = Keyword::PreProc;
            }
        }

        Encoding enc{};
        enc.keywords  .push_back(k);
        enc.depth     .push_back(0);
        enc.data      .push_back(value);
        enc.bindings  .push_back(nullptr);

        return enc;
    }

  public:

    Encoding const& encoding() const
    {
        return encoding_;
    }

    explicit Expression(Encoding && encoding) : encoding_{std::move(encoding)} {}

    // Terminal expression with literal parameters
    static Encoding encodeTerminal(Keyword const& keyword, boost::json::value const& params)
    {
        Encoding enc {};
        enc.keywords.push_back(keyword);
        enc.depth   .push_back(0);
        enc.data    .push_back(params);
        enc.bindings.push_back(nullptr);
        return enc;
    }

    // Non-terminal expression
    static Encoding encodeNested(Keyword const& keyword, std::vector<Expression> const& subexpressions)
    {
        Encoding enc {};
        enc.keywords.push_back(keyword);
        enc.depth   .push_back(0);
        enc.data    .push_back(nullptr);
        enc.bindings.push_back(nullptr);

        for (auto const& subexpr: subexpressions)
        {
            enc.keywords.insert(enc.keywords.end(), subexpr.encoding().keywords.begin(), subexpr.encoding().keywords.end());
            enc.data    .insert(enc.data    .end(), subexpr.encoding().data    .begin(), subexpr.encoding().data    .end());
            enc.bindings.insert(enc.bindings.end(), subexpr.encoding().bindings.begin(), subexpr.encoding().bindings.end());
            for (auto d: subexpr.encoding_.depth)
            {
                enc.depth.push_back(++d);
            }
        }
        return enc;
    }

    Expression(Keyword const& keyword, boost::json::value const& params)
        : encoding_{encodeTerminal(keyword, params)}
    {
    }

    // Deserialize JSON
    Expression(boost::json::value const& expr);

    /// Return const expressions as Eq(expr), except for Noop,
    /// otherwise return expr unchanged
    static Expression asPredicate(Expression const& expr);

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

    Expression(Expression const& o) = default;
    Expression(Expression && o) = default;
    Expression& operator=(Expression const& o) = default;
    Expression& operator=(Expression && o) = default;

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

        auto data_it = encoding().data.cbegin();
        for (size_t i = 0; i < encoding().keywords.size(); i++)
        {
            auto const& k = encoding().keywords.at(i);
            if (Keyword::PreProc == k)
            {
                pp.emplace_back(encoding().data.at(i).as_string().c_str(), zmbt::format("/data/%d", i));
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
        return encoding().keywords == o.encoding().keywords
        && encoding().depth == o.encoding().depth
        && encoding().bindings == o.encoding().bindings
        && encoding().data == o.encoding().data;
    }

    bool operator!=(Expression const& o) const
    {
        return !operator==(o);
    }

    Keyword keyword() const
    {
        return encoding().keywords.at(0);
    }

    boost::json::value const& data() const
    {
        if (encoding().keywords.size() > 1 && encoding().keywords.at(1) == Keyword::Literal)
        {
            return encoding().data.at(1);
        }
        return encoding().data.at(0);
    }

    Expression subexpr() const
    {
        auto const pl = parameter_list();
        return pl.empty() ? nullptr : pl.front();
    }


    bool has_params() const
    {
        return has_subexpr() || is_literal(); //!encoding().data.at(0).is_null();
    }

    bool has_subexpr() const
    {
        return encoding().keywords.size() > 1;
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

    bool is_infix_chain() const
    {
        return (is_fork() || is_compose()) && encoding().keywords.size() > 2;
    }

    bool is_variadic() const
    {
        return detail::isVariadic(keyword());
    }

    std::size_t infix_size() const
    {
        std::size_t n {0};
        for (auto const& d: encoding().depth)
        {
            if (d == 1) ++n;
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

    bool is_hiord() const;

    bool is_boolean() const;

    bool is_error() const
    {
        return is(Keyword::Err);
    }

    boost::json::value to_json() const;

    operator boost::json::value() const
    {
        return to_json();
    }

    /// @brief Evaluate expression
    /// @param x run-time argument
    /// @param config evaluation config
    /// @return
    boost::json::value eval(boost::json::value const& x = nullptr, EvalContext const& ctx = {}) const;

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
