/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MODEL_EXPRESSION_GRAMMAR_HPP_
#define ZMBT_MODEL_EXPRESSION_GRAMMAR_HPP_

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

// #include "zmbt/reflect.hpp"
// #include "signal_operator_handler.hpp"
#include "expression_keyword.hpp"


namespace zmbt {


struct ExpressionKeywordSymbol : public boost::spirit::qi::symbols<char, ExpressionKeyword>
{
    ExpressionKeywordSymbol();
};


struct ExpressionKeywordGrammar : public boost::spirit::qi::grammar<boost::json::string::const_iterator, ExpressionKeyword()>
{
    ExpressionKeywordGrammar();

    ExpressionKeywordSymbol keyword_symbol;
    boost::spirit::qi::rule<boost::json::string::const_iterator, ExpressionKeyword()> start;
};

} // namespace zmbt


#endif // ZMBT_MAPPING_EXPRESSION_HPP_
