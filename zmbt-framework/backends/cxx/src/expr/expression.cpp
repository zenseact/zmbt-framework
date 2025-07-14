/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <cstdint>
#include <ostream>
#include <sstream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"


namespace zmbt {
namespace lang {


Expression::Expression() : Expression(Keyword::Void)
{
}

Expression::Expression(Keyword const& keyword)
    : Expression(encodeTerminal(keyword, {}))
{
    if (keyword == Keyword::Literal || keyword == Keyword::PreProc)
    {
        throw zmbt::expression_error("expression requires a value");
    }
}

Expression::Expression(boost::json::value const& expr)
    : encoding_{encode(expr)}
{
}

Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(encodeTerminal(Keyword::Literal, boost::json::value(items)))
{
}

Expression Expression::asPredicate(Expression const& expr)
{
    if (!expr.is_noop() && expr.is_const())
    {
        return Expression(encodeNested(Keyword::Eq, {expr}));
    }
    return expr;
}

boost::json::value Expression::to_json() const
{
    return (is_literal() || is_preproc()) ? data() : boost::json::value_from(encoding());
}

bool Expression::is_const() const
{
    // TODO: test it and clarify param evaluation
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;
    if (is_nonempty_composition())
    {
        auto const pl = parameter_list();
        auto it = pl.cbegin();
        if (it->is_literal() || it->is_const())
        {
            return true;
        }
        ++it;
        while (it != pl.cend())
        {
            if (it->is_const() && not it->is_literal()) return true;
            ++it;
        }
        return false;
    }
    if (is_nonempty_fork())
    {
        auto const paramlist = parameter_list();
        for (auto const& e: paramlist)
        {
            if (not e.is_const()) return false;
        }
        return true;
    }
    else if (is(Keyword::Op) && has_params())
    {
        auto const pl = parameter_list();
        for (auto const& p: pl)
        {
            if (p.is_const()) return true;
        }
        return false;
    }
    return is(Keyword::Literal) || is(Keyword::Q) || is(Keyword::Err) || (CodegenType::Const == getCodegenType(keyword()));
}

boost::json::string_view Expression::keyword_to_str() const
{
    return ::zmbt::lang::keyword_to_str(keyword());
}

bool Expression::is_boolean() const
{
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;
    using lang::detail::isBoolean;

    if(is_literal())
    {
        return encoding().data.front().is_bool();
    }
    if (isBoolean(keyword()))
    {
        return true;
    }
    if (is(Keyword::Op) && has_params())
    {
        return parameter_list().front().is_boolean();
    }
    if (is_nonempty_composition())
    {
        return parameter_list().back().is_boolean();
    }
    return false;
}

bool Expression::is_hiord() const
{
    return lang::detail::isHiOrd(keyword());
}

std::list<Expression> Expression::parameter_list() const
{
    std::list<Expression> result;


    auto const& self = encoding();
    if (self.keywords.size() == 1)
    {
        return result;
    }

    auto it_keyword = self.keywords.cbegin() + 1;
    auto it_depth = self.depth.cbegin() + 1;
    auto it_data = self.data.cbegin() + 1;
    auto it_binding = self.bindings.cbegin() + 1;

    Encoding enc {};

    while(it_keyword != self.keywords.cend())
    {

        if (!enc.depth.empty() && 1 == *it_depth)
        {
            result.emplace_back(enc);
            enc = {};
        }

        enc.keywords.push_back(*it_keyword);
        enc.depth   .push_back(*it_depth-1);
        enc.bindings.push_back(*it_binding);
        enc.data    .push_back(*it_data);

        ++it_keyword;
        ++it_depth;
        ++it_data;
        ++it_binding;
    }
    if (!enc.depth.empty())
    {
        result.emplace_back(enc);
    }

    for (auto& d: enc.depth)
    {
        --d;
    }

    return result;
}


bool Expression::match(boost::json::value const& observed, Operator const& op) const
{
    auto result = eval(observed, {op, {}, 0});
    if (!result.is_bool())
    {
        return false;
    }
    return result.get_bool();
}

}  // namespace lang
}  // namespace zmbt
