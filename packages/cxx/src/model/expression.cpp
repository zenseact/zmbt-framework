/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"



namespace
{
using Keyword = zmbt::ExpressionKeyword;


boost::json::value const* ptrToParams(boost::json::value const& underlying)
{
    return underlying.is_object() ? &(underlying.get_object().cbegin()->value()) : nullptr;
}

} // namespace


namespace zmbt {
namespace v2 {

Expression::Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying)
    : keyword_{keyword}
    , underlying_{std::move(underlying)}
    , params_ptr_{ptrToParams(underlying_)}
{
}


Expression::Expression(Keyword const& keyword, boost::json::value const& params)
    : Expression(internal_tag{}, keyword, boost::json::object{{reflect::json_from(keyword).get_string(), params}})
{
}


Expression::Expression(Keyword const& keyword)
    : Expression(internal_tag{}, keyword, reflect::json_from(keyword))
{
}



struct Expression::json_ctor_params
{
    boost::json::value underlying;
    Keyword keyword;


    json_ctor_params(boost::json::value&& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = reflect::dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = reflect::dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = std::move(expr);
        }
        else
        {
            keyword = Keyword::Eq;
            underlying = boost::json::object{{reflect::json_from(Keyword::Eq).get_string(), std::move(expr)}};
        }
    }

    json_ctor_params(boost::json::value const& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = reflect::dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = reflect::dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = expr;
        }
        else
        {
            keyword = Keyword::Eq;
            underlying = boost::json::object{{reflect::json_from(Keyword::Eq).get_string(), expr}};
        }
    }
};

Expression::Expression(json_ctor_params&& helper)
    : Expression(internal_tag{}, helper.keyword, std::move(helper.underlying))
{
}


Expression::Expression()
    : Expression(internal_tag{}, Keyword::Undefined, nullptr)
{
}


Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(Keyword::Eq, boost::json::value(items))
{
}


Expression::Expression(boost::json::value const& expr)
    : Expression(json_ctor_params{std::move(expr)})
{
}


boost::json::array Expression::toArray(std::initializer_list<Expression> const& list) {
    boost::json::array arr(list.size());
    std::transform(list.begin(), list.end(), arr.begin(), [](Expression const& e) {return e.underlying_;});
    return arr;
}

}  // namespace v2
}  // namespace zmbt
