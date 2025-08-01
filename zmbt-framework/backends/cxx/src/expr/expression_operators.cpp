
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
    return Expression(Expression::encodeNested(Keyword::Fork, {lhs, rhs}));

}

Expression operator<<(Expression link, Expression referent)
{
    // TODO: binding
    // TODO: handle righ-assoc infix unfold
    static_cast<void>(link);
    static_cast<void>(referent);

    auto const if_string = link.if_string();
    if (!if_string)
    {
        return expr::Err("Symbolic reference shall be a string", BOOST_CURRENT_FUNCTION);
    }
    else if(not Encoding::is_link_token(*if_string))
    {
        return expr::Err("Invalid symbolic reference format", BOOST_CURRENT_FUNCTION);
    }
    return Expression(Expression::encodeNested(Keyword::Fn,
        {expr::Tuple(std::move(link), std::move(referent))}
    ));
}

Expression operator~(Expression expr)
{
    return expr::Flip(expr);
}


} // namespace lang
} // namespace zmbt
