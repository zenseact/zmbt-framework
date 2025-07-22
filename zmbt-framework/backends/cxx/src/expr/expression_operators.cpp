
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/api.hpp"


namespace zmbt {
namespace lang {


Expression Expression::unfold_left_assoc(Keyword const keyword, Expression&& lhs, Expression&& rhs)
{
    if (lhs.encoding_.keywords.front() == keyword)
    {
        lhs.encoding_.append_to_root(std::move(rhs.encoding_));
        return lhs;
    }
    else
    {
        return Expression(Expression::encodeNested(keyword, {std::move(lhs), std::move(rhs)}));
    }
}


Expression operator|(Expression lhs, Expression rhs)
{
    return Expression::unfold_left_assoc(Keyword::Pipe, std::move(lhs), std::move(rhs));
}

Expression operator+(Expression lhs, Expression rhs)
{
    return Expression::unfold_left_assoc(Keyword::Tuple, std::move(lhs), std::move(rhs));
}

Expression operator&(Expression lhs, Expression rhs)
{
    return Expression(Expression::encodeNested(Keyword::Fork, {lhs + rhs}));

}

Expression operator<<(Expression link, Expression referent)
{
    // TODO: binding
    // TODO: handle righ-assoc infix unfold
    static_cast<void>(link);
    static_cast<void>(referent);
    return expr::Err("symbolic references are not implemented");
}

std::ostream& operator<<(std::ostream& os, Expression const& expr)
{
    return expr.prettify_to(os);
}

zmbt::Logger& operator<<(zmbt::Logger& logger, Expression const& expr)
{
    logger << static_cast<boost::json::value>(expr.prettify());
    return logger;
}

boost::json::value operator*(Expression lhs, Expression const& rhs)
{
    return lhs.eval(rhs);
}

boost::json::value operator*(Expression expr)
{
    return expr.eval();
}

} // namespace lang
} // namespace zmbt

