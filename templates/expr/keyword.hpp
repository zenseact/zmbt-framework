@require(keyword_groups)
/**
 * \file
 * \copyright (c) Copyright 2024 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/keyword.hpp`
 * 2. Update the keyword data at `templates/expr/data.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:keyword_hpp > zmbt-framework/backends/cxx/include/zmbt/model/keyword.hpp`
 * 4. Commit changes
 */

#ifndef ZMBT_MODEL_keyword.hpp_
#define ZMBT_MODEL_keyword.hpp_

#include <boost/json.hpp>

#include "zmbt/reflect.hpp"

#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {

/// ZMBT Expression keyword
enum class Keyword
{
@for group in keyword_groups:
@for keyword in group['keywords']:

    /// @keyword.get('brief', keyword['name'].capitalize())
    @keyword.get('enum', keyword['name'].capitalize()),
@end
@end
    /// Internal utility value
    Undefined
};

// Boost JSON conversion from Keyword
void tag_invoke(boost::json::value_from_tag const&, boost::json::value&, Keyword const& t);

// Boost JSON conversion to Keyword
Keyword tag_invoke(boost::json::value_to_tag<Keyword> const&, boost::json::value const&);


} // namespace zmbt



#endif // ZMBT_MAPPING_keyword.hpp_
