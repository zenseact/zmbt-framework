/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/expression_grammar.hpp"
#include "zmbt/expr/keyword_grammar.hpp"


namespace zmbt {
namespace lang {

struct ExpressionGrammar::Rules
{
    boost::spirit::karma::rule<OutputIterator, Expression()> literal, keyword;

    boost::spirit::karma::rule<OutputIterator, std::vector<Expression>()> fork, composition;
};

ExpressionGrammar::ExpressionGrammar()
    : ExpressionGrammar::base_type(start)
    , rules{std::make_unique<Rules>()}
{
    namespace karma = boost::spirit::karma;
    namespace phoenix = boost::phoenix;

    struct Funcs
    {
        struct IsLiteral {
            bool operator()(Expression const& e) const {
                return e.is_literal();
            }
        };
        struct IsComposition {
            bool operator()(Expression const& e) const {
                return e.is(Keyword::Compose);
            }
        };

        struct IsFork {
            bool operator()(Expression const& e) const {
                return e.is(Keyword::Fork);
            }
        };
        struct HasParams {
            bool operator()(Expression const& e) const {
                return e.has_params();
            }
        };
        struct JsonDump {
            std::string operator()(Expression const& e) const {
                return boost::json::serialize(e.underlying());
            }
        };
        struct Pascal {
            std::string operator()(Expression const& e) const {
                auto const k = zmbt::json_from(e.keyword()).as_string().c_str();
                auto const w =  boost::json::string_view(k);
                return w.substr(std::strlen(ZMBT_KEYWORD_PREFIX)).data();
            }
        };
        struct Subexpr {
            Expression operator()(Expression const& e) const {
                return e.subexpr();
            }
        };

        struct Composition {
            using result_type = std::vector<Expression>;

            result_type operator()(const Expression& e) const {
                result_type result;
                for (auto const& v : e.params().as_array())
                    result.emplace_back(Expression(v));
                std::reverse(result.begin(), result.end());
                return result;
            }
        };

        struct Fork {
            using result_type = std::vector<Expression>;

            result_type operator()(const Expression& e) const {
                result_type result;
                for (auto const& v : e.params().as_array())
                    result.emplace_back(Expression(v));
                return result;
            }
        };
        boost::phoenix::function<IsLiteral> is_literal;
        boost::phoenix::function<IsComposition> is_composition;
        boost::phoenix::function<IsFork> is_fork;
        boost::phoenix::function<HasParams> has_params;
        boost::phoenix::function<JsonDump> json_dump;
        boost::phoenix::function<Pascal> pascal;
        boost::phoenix::function<Subexpr> subexpr;
        boost::phoenix::function<Composition> composition;
        boost::phoenix::function<Fork> fork;
    } f;

    using karma::string;
    using karma::eps;
    using karma::_val;
    using karma::lit;
    using karma::_1;

    auto& literal = rules->literal;
    auto& keyword = rules->keyword;
    auto& composition = rules->composition;
    auto& fork = rules->fork;

    start
        = eps(f.is_literal(_val)) << literal
        | eps(f.is_composition(_val)) << composition[_1 = f.composition(_val)]
        | eps(f.is_fork(_val)) << fork[_1 = f.fork(_val)]
        | keyword ;

    literal = string[_1 = f.json_dump(_val)];

    composition = lit('(') << start % lit(" | ") << ')';
    fork = lit('(') << start % lit(" & ") << ')';

    keyword = string[_1 = f.pascal(_val)]
        << -(eps(f.has_params(_val)) << lit('(') << start[_1 = f.subexpr(_val)] << ')');
}


std::string Expression::prettify() const
{
    std::string out;
    std::back_insert_iterator<std::string> sink(out);

    ExpressionGrammar
    // <std::back_insert_iterator<std::string>>
    gen;
    boost::spirit::karma::generate(sink, gen, *this);

    return out;
}


} // namespace lang
} // namespace zmbt
