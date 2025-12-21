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
struct ExpressionGrammar : boost::spirit::karma::grammar<OutputIterator, ExpressionView()> {
    ExpressionGrammar() : ExpressionGrammar::base_type(start)
    {
        namespace karma = boost::spirit::karma;
        namespace phoenix = boost::phoenix;

        using karma::_1;
        using karma::_val;
        using karma::eps;
        using karma::lit;
        using karma::string;

        auto const is_literal          = boost::phoenix::bind(&ExpressionView::is_literal, _val);
        auto const is_preproc          = boost::phoenix::bind(&ExpressionView::is_preproc, _val);
        auto const is_link          = boost::phoenix::bind(&ExpressionView::is_link, _val);
        auto const is_valid_link       = boost::phoenix::bind(&ExpressionView::is_valid_link, _val);
        auto const has_subexpr         = boost::phoenix::bind(&ExpressionView::has_subexpr, _val);
        auto const serialize           = boost::phoenix::bind(&ExpressionView::serialize, _val);
        auto const keyword_to_str      = boost::phoenix::bind(&ExpressionView::keyword_to_str, _val);
        auto const subexpressions_list = boost::phoenix::bind(&ExpressionView::subexpressions_list, _val);
        auto const tuple_parameters    = boost::phoenix::bind(&ExpressionView::tuple_parameters, _val);
        auto const is_infix_pipe       = boost::phoenix::bind(&ExpressionView::is_infix_pipe, _val);
        auto const is_infix_fork       = boost::phoenix::bind(&ExpressionView::is_infix_fork, _val);
        auto const is_infix_tuple      = boost::phoenix::bind(&ExpressionView::is_infix_tuple, _val);
        auto const is_complete_flip    = boost::phoenix::bind(&ExpressionView::is_complete_flip, _val);

        start
            = eps(is_literal)      << karma::lazy(serialize)
            | eps(is_preproc)      << karma::lazy(serialize)
            | eps(is_link)      << karma::lazy(serialize)
            | eps(is_valid_link)   << link[_1 = tuple_parameters]
            | eps(is_infix_pipe)   << pipe[_1 = subexpressions_list]
            | eps(is_infix_fork)   << fork[_1 = subexpressions_list]
            | eps(is_infix_tuple)  << tuple[_1 = subexpressions_list]
            | eps(is_complete_flip)<< flip[_1 = subexpressions_list]
            | keyword;

        subexpr
            = eps(is_literal)       << karma::lazy(serialize)
            | eps(is_preproc)       << karma::lazy(serialize)
            | eps(is_link)       << karma::lazy(serialize)
            | eps(is_valid_link)    << nested_link[_1 = tuple_parameters]
            | eps(is_infix_pipe)    << nested_pipe[_1 = subexpressions_list]
            | eps(is_infix_fork)    << nested_fork[_1 = subexpressions_list]
            | eps(is_infix_tuple)   << nested_tuple[_1 = subexpressions_list]
            | eps(is_complete_flip) << flip[_1 = subexpressions_list]
            | keyword;

        pipe    = subexpr % lit(" | ");
        fork    = subexpr % lit(" & ");
        tuple   = subexpr % lit(" , ");
        link    = subexpr % lit(" << ");
        flip    = lit('~') << subexpr;
        nested_link = lit('(') << subexpr % lit(" << ") << ')';
        nested_pipe  = lit('(') << subexpr % lit(" | ") << ')';
        nested_fork  = lit('(') << subexpr % lit(" & ") << ')';
        nested_tuple = lit('(') << subexpr % lit(" , ") << ')';
        parameters   = lit('(') << -(start % lit(", "))  << ')';

        keyword = string[_1 = keyword_to_str] << -(eps(has_subexpr)  << parameters[_1 = subexpressions_list]);
    }

    boost::spirit::karma::rule<OutputIterator, ExpressionView()> start, subexpr, keyword;
    boost::spirit::karma::rule<OutputIterator, std::vector<ExpressionView>()> parameters, fork, pipe, tuple, link, flip, nested_pipe, nested_fork, nested_tuple, nested_link;
};

}  // namespace lang
}  // namespace zmbt

#endif  // ZMBT_MAPPING_EXPRESSION_HPP_
