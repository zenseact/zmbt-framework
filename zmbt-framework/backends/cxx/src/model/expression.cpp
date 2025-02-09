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
using Keyword = zmbt::expr::Keyword;


boost::json::value const* ptrToParams(Keyword const& keyword, boost::json::value const& underlying)
{
    if (keyword == Keyword::Literal)
    {
        return &underlying;
    }
    return underlying.is_object() ? &(underlying.get_object().cbegin()->value()) : nullptr;
}

boost::json::value get_underlying(Keyword const& keyword, boost::json::value const& params)
{
    return keyword == Keyword::Literal ? params : boost::json::value{{zmbt::json_from(keyword).get_string(), params}};
}

} // namespace


namespace zmbt {

Expression::Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying)
    : keyword_{keyword}
    , underlying_{std::move(underlying)}
    , params_ptr_{ptrToParams(keyword, underlying_)}
{
}


Expression::Expression(Keyword const& keyword, boost::json::value const& params)
    : Expression(internal_tag{}, keyword, get_underlying(keyword, params))
{
}


Expression::Expression(Keyword const& keyword)
    : Expression(internal_tag{}, keyword, json_from(keyword))
{
    if (keyword == Keyword::Literal)
    {
        throw zmbt::model_error("Lit expression requires a value");
    }
}



struct Expression::json_ctor_params
{
    boost::json::value underlying;
    Keyword keyword;


    json_ctor_params(boost::json::value&& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = std::move(expr);
        }
        else
        {
            keyword = Keyword::Literal;
            underlying = expr;
        }
    }

    json_ctor_params(boost::json::value const& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = expr;
        }
        else
        {
            keyword = Keyword::Literal;
            underlying = expr;
        }
    }
};

Expression::Expression(json_ctor_params&& params)
    : Expression(internal_tag{}, params.keyword, std::move(params.underlying))
{
}


Expression::Expression()
    : Expression(internal_tag{}, Keyword::Undefined, nullptr)
{
}

Expression::Expression(boost::json::value const& expr)
    : Expression(json_ctor_params{std::move(expr)})
{
}

Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(Keyword::Literal, boost::json::value(items))
{
}

boost::json::array Expression::toArray(std::initializer_list<Expression> const& list) {
    boost::json::array arr(list.size());
    std::transform(list.begin(), list.end(), arr.begin(), [](Expression const& e) {return e.underlying_;});
    return arr;
}

bool Expression::match(boost::json::value const& observed, SignalOperatorHandler const& op) const
{
    auto result = eval(observed, op);
    if (!result.is_bool())
    {
        throw zmbt::model_error("expr is not a predicate: `%s`", underlying_);
    }
    return result.get_bool();
}

}  // namespace zmbt
