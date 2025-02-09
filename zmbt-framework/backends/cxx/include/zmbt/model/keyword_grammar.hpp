/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MODEL_EXPRESSION_GRAMMAR_HPP_
#define ZMBT_MODEL_EXPRESSION_GRAMMAR_HPP_

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

#include "keyword.hpp"


namespace zmbt {
namespace expr {

/// Boost.Sirit.Qi Symbol for zmbt::expr::Keyword parsing
struct KeywordSymbol : public boost::spirit::qi::symbols<char, Keyword>
{
    KeywordSymbol();
};

/// Boost.Sirit.Qi Grammar for zmbt::expr::Keyword parsing
struct KeywordGrammar : public boost::spirit::qi::grammar<boost::json::string::const_iterator, Keyword()>
{
    KeywordGrammar();

    KeywordSymbol keyword_symbol;
    boost::spirit::qi::rule<boost::json::string::const_iterator, Keyword()> start;
};

} // namespace expr
} // namespace zmbt


#endif // ZMBT_MAPPING_EXPRESSION_HPP_
