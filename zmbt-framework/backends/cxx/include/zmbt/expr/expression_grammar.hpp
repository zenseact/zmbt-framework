/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_EXPRESSION_GRAMMAR_HPP_
#define ZMBT_EXPR_EXPRESSION_GRAMMAR_HPP_

#include <boost/spirit/include/karma.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/statement.hpp>
#include <boost/phoenix/core.hpp>

#include "expression.hpp"
#include "keyword_grammar.hpp"


namespace zmbt {
namespace lang {



// template<typename OutputIterator>
struct ExpressionGrammar : boost::spirit::karma::grammar<std::back_insert_iterator<std::string>, Expression()>
{
    using OutputIterator = std::back_insert_iterator<std::string>;

    ExpressionGrammar();

    boost::spirit::karma::rule<OutputIterator, Expression()> start;
    struct Rules;
    std::unique_ptr<Rules> rules;
};

} // namespace lang
} // namespace zmbt


#endif // ZMBT_MAPPING_EXPRESSION_HPP_
