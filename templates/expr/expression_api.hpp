@require(keyword_groups)
/**
 * \file
 * \copyright (c) Copyright 2024 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/expression_api.hpp`
 * 2. Update the keyword data at `templates/expr/data.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:expression_api_hpp > packages/cxx/include/zmbt/model/expression_api.hpp`
 * 4. Commit changes
 */


#ifndef ZMBT_MODEL_EXPRESSION_API_HPP_
#define ZMBT_MODEL_EXPRESSION_API_HPP_

#include "expression_api_templates.hpp"


namespace zmbt {

@(
    def define_api_constant(group, keyword):
        Key = keyword.get('enum', keyword['name'].capitalize())
        BaseTemplate = group['api-template']
        return f"static {BaseTemplate}<ExpressionKeyword::{Key}> const {Key}{{}};"
)

namespace expr
{

@for group in filter(lambda g: 'api-template' in g, keyword_groups):
@for keyword in group['keywords']:

    @if comment := keyword.get('comment', None):
    @for line in comment.split('\n'):
    /// @line
    @end
    @end
    @define_api_constant(group, keyword)
@end
@end



/// Floating point approximate equal
/// Based on numpy.isclose: abs(a - b) <= (atol + rtol * abs(ref))
static struct Approx_ : public Expression
{
    Approx_() : Expression(Keyword::Approx)
    {}

    Expression operator()(double reference, double rtol, double atol = std::numeric_limits<double>::epsilon()) const
    {
        return Expression(Expression::Keyword::Approx, boost::json::array {reference, rtol, atol});
    }
} Approx;


// Aliases

static decltype(Noop) const _ = Noop;
static decltype(Ni) const Contains = Ni;
static decltype(Approx) const Near = Approx;
static decltype(Re) const Regex = Re;

}

} // namespace zmbt

#endif // ZMBT_MAPPING_EXPRESSION_HPP_
