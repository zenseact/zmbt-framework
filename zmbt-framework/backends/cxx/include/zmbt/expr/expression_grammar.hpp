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
struct ExpressionGrammar : boost::spirit::karma::grammar<OutputIterator, Expression()> {
    ExpressionGrammar() : ExpressionGrammar::base_type(start)
    {
        namespace karma = boost::spirit::karma;
        namespace phoenix = boost::phoenix;

        using karma::_1;
        using karma::_val;
        using karma::eps;
        using karma::lit;
        using karma::string;

        auto const is_literal          = boost::phoenix::bind(&Expression::is_literal, _val);
        auto const is_preproc          = boost::phoenix::bind(&Expression::is_preproc, _val);
        auto const has_subexpr         = boost::phoenix::bind(&Expression::has_subexpr, _val);
        auto const serialize           = boost::phoenix::bind(&Expression::serialize, _val);
        auto const keyword_to_str      = boost::phoenix::bind(&Expression::keyword_to_str, _val);
        auto const subexpressions_list = boost::phoenix::bind(&Expression::subexpressions_list, _val);
        auto const is_infix_pipe       = boost::phoenix::bind(&Expression::is_infix_pipe, _val);
        auto const is_infix_fork       = boost::phoenix::bind(&Expression::is_infix_fork, _val);
        auto const is_infix_pack       = boost::phoenix::bind(&Expression::is_infix_pack, _val);

        start
            = eps(is_literal)    << karma::lazy(serialize)
            | eps(is_preproc)    << karma::lazy(serialize)
            | eps(is_infix_pipe) << pipe[_1 = subexpressions_list]
            | eps(is_infix_fork) << fork[_1 = subexpressions_list]
            | eps(is_infix_pack) << pack[_1 = subexpressions_list]
            | keyword;

        subexpr
            = eps(is_literal)    << karma::lazy(serialize)
            | eps(is_preproc)    << karma::lazy(serialize)
            | eps(is_infix_pipe) << nested_pipe[_1 = subexpressions_list]
            | eps(is_infix_fork) << nested_fork[_1 = subexpressions_list]
            | eps(is_infix_pack) << nested_pack[_1 = subexpressions_list]
            | keyword;

        pipe    = subexpr % lit(" | ");
        fork    = subexpr % lit(" & ");
        pack    = subexpr % lit(" + ");
        nested_pipe = lit('(') << subexpr % lit(" | ") << ')';
        nested_fork = lit('(') << subexpr % lit(" & ") << ')';
        nested_pack = lit('(') << subexpr % lit(" + ") << ')';
        parameters  = lit('(') << -(start % lit(", "))  << ')';

        keyword = string[_1 = keyword_to_str] << -(eps(has_subexpr)  << parameters[_1 = subexpressions_list]);
    }

    boost::spirit::karma::rule<OutputIterator, Expression()> start, subexpr, keyword;
    boost::spirit::karma::rule<OutputIterator, std::list<Expression>()> fork, pipe, pack, parameters, nested_pipe, nested_fork, nested_pack;
};

}  // namespace lang
}  // namespace zmbt

#endif  // ZMBT_MAPPING_EXPRESSION_HPP_
