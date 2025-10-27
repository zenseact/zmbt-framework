

# File keyword\_grammar.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**keyword\_grammar.hpp**](keyword__grammar_8hpp.md)

[Go to the documentation of this file](keyword__grammar_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_GRAMMAR_HPP_
#define ZMBT_EXPR_KEYWORD_GRAMMAR_HPP_

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

#include "keyword.hpp"


namespace zmbt {
namespace lang {

struct KeywordSymbol : public boost::spirit::qi::symbols<char, Keyword>
{
    KeywordSymbol();
};

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
```


