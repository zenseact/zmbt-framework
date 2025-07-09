/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/expression_grammar.hpp"
#include "zmbt/expr/keyword_grammar.hpp"


namespace zmbt {
namespace lang {

std::string Expression::prettify() const
{
    std::string out;
    std::back_insert_iterator<std::string> sink(out);

    ExpressionGrammar<std::back_insert_iterator<std::string>> gen;
    boost::spirit::karma::generate(sink, gen, *this);

    return out;
}


} // namespace lang
} // namespace zmbt
