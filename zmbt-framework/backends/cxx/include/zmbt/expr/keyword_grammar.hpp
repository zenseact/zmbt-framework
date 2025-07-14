/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_KEYWORD_GRAMMAR_HPP_
#define ZMBT_EXPR_KEYWORD_GRAMMAR_HPP_

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

#include "keyword.hpp"


namespace zmbt {
namespace lang {

/// Boost.Sirit.Qi Symbol for zmbt::lang::Keyword parsing
struct KeywordSymbol : public boost::spirit::qi::symbols<char, Keyword>
{
    KeywordSymbol();
};

/// Boost.Sirit.Qi Grammar for zmbt::lang::Keyword parsing
struct KeywordGrammar : public boost::spirit::qi::grammar<boost::json::string::const_iterator, Keyword()>
{
    KeywordGrammar();

    KeywordSymbol keyword_symbol;
    boost::spirit::qi::rule<boost::json::string::const_iterator, Keyword()> start;
};

boost::json::string_view keyword_to_str(Keyword const&);


// Boost JSON conversion from Keyword
void tag_invoke(boost::json::value_from_tag const&, boost::json::value&, Keyword const& t);

// Boost JSON conversion to Keyword
Keyword tag_invoke(boost::json::value_to_tag<Keyword> const&, boost::json::value const&);


} // namespace lang
} // namespace zmbt


#endif // ZMBT_MAPPING_EXPRESSION_HPP_
