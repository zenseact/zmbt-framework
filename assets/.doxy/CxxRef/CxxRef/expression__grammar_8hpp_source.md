

# File expression\_grammar.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**expression\_grammar.hpp**](expression__grammar_8hpp.md)

[Go to the documentation of this file](expression__grammar_8hpp.md)


```C++

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
```


