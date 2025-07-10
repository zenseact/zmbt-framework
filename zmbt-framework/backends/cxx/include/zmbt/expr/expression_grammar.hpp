/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_EXPRESSION_GRAMMAR_HPP_
#define ZMBT_EXPR_EXPRESSION_GRAMMAR_HPP_

#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/karma.hpp>

#include "expression.hpp"
#include "keyword_grammar.hpp"

namespace zmbt {
namespace lang {


template <typename OutputIterator>
struct ExpressionGrammar : boost::spirit::karma::grammar<std::back_insert_iterator<std::string>, Expression()> {
    ExpressionGrammar() : ExpressionGrammar::base_type(start)
    {
        namespace karma = boost::spirit::karma;
        namespace phoenix = boost::phoenix;

        using karma::_1;
        using karma::_val;
        using karma::eps;
        using karma::lit;
        using karma::string;

        struct CompositionList {
            using result_type = std::list<Expression>;
            result_type operator()(const Expression& e) const
            {
                auto list = e.parameter_list();
                list.reverse();
                return list;
            }
        };

        auto const is_literal     = boost::phoenix::bind(&Expression::is_literal, _val);
        auto const has_params     = boost::phoenix::bind(&Expression::has_params, _val);
        auto const serialize      = boost::phoenix::bind(&Expression::serialize, _val);
        auto const keyword_to_str = boost::phoenix::bind(&Expression::keyword_to_str, _val);
        auto const is_nonempty_composition  = boost::phoenix::bind(&Expression::is_nonempty_composition, _val);
        auto const is_nonempty_fork         = boost::phoenix::bind(&Expression::is_nonempty_fork, _val);
        auto const parameter_list           = boost::phoenix::bind(&Expression::parameter_list, _val);
        auto const composition_list         = boost::phoenix::function<CompositionList>()(_val);


        start
            = eps(is_literal) << literal
            | eps(is_nonempty_composition) << composition[_1 = composition_list]
            | eps(is_nonempty_fork) << fork[_1 = parameter_list]
            | keyword;

        literal = string[_1 = serialize];
        composition = start % lit(" | ");
        parameters = start % lit(", ");
        fork = lit('(') << start % lit(" & ") << ')';

        keyword = string[_1 = keyword_to_str] << -(eps(has_params) << lit('(') << parameters[_1 = parameter_list] << ')');
    }

    boost::spirit::karma::rule<OutputIterator, Expression()> start, literal, keyword;
    boost::spirit::karma::rule<OutputIterator, std::list<Expression>()> fork, composition, parameters;
};

}  // namespace lang
}  // namespace zmbt

#endif  // ZMBT_MAPPING_EXPRESSION_HPP_
