@require(keyword_groups)
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/keyword.cpp`
 * 2. Update the keyword data at `templates/expr/keywords.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:keyword_cpp > zmbt-framework/backends/cxx/src/model/keyword.cpp`
 * 4. Commit changes
 */


#include <boost/spirit/include/qi.hpp>
#include <boost/json.hpp>


#include "zmbt/reflect.hpp"
#include "zmbt/model/keyword.hpp"
#include "zmbt/model/keyword_grammar.hpp"


#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {
void tag_invoke(boost::json::value_from_tag const&, boost::json::value& v, Keyword const& kw)
{
    switch(kw)
    {
@for group in keyword_groups.values():
@for keyword in group:
    case Keyword::@keyword.get('enum', keyword['name'].capitalize()): { v = ZMBT_KEYWORD_PREFIX "@keyword['name']"; break; }
@end
@end
        // TODO: throw
        default: v = ":undefined";
    }
}

Keyword
tag_invoke(boost::json::value_to_tag<Keyword> const&, boost::json::value const& v)
{
    if (not v.is_string())
    {
        return Keyword::Undefined;
    }

    static KeywordGrammar const keyword_parser {};

    auto const& str = v.as_string();
    auto iter = str.cbegin();
    auto end = str.cend();
    Keyword keyword_out {Keyword::Undefined};
    static_cast<void>(boost::spirit::qi::parse(iter, end, keyword_parser, keyword_out));
    return keyword_out;
}
} // namespace zmbt
