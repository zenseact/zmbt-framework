
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/api.hpp"

namespace
{
using zmbt::lang::Expression;
using zmbt::lang::Keyword;

Expression unfold_left_assoc(Keyword const keyword, Expression const& lhs, Expression const& rhs)
{
    if (lhs.encoding().keywords.front() == keyword)
    {
        auto newenc = lhs.encoding();

        newenc.append_to_root(rhs.encoding());
        return Expression(std::move(newenc));
    }
    else
    {
        return Expression(Expression::encodeNested(keyword, std::vector<Expression>{lhs, rhs}));
    }
}

} // namespace


namespace zmbt {
namespace lang {

Expression operator|(Expression const& lhs, Expression const& rhs)
{
    return unfold_left_assoc(Keyword::Pipe, lhs, rhs);
}

Expression operator+(Expression const& lhs, Expression const& rhs)
{
    return unfold_left_assoc(Keyword::Pack, lhs, rhs);
}

Expression operator&(Expression const& lhs, Expression const& rhs)
{
    return unfold_left_assoc(Keyword::Fork, lhs, rhs);
}

Expression operator<<(Expression const&, Expression const&)
{
    // TODO: binding
    return {};
}

std::ostream& operator<<(std::ostream& os, Expression const& expr)
{
    return expr.prettify_to(os);
}

boost::json::value operator*(Expression const& lhs, Expression const& rhs)
{
    return lhs.eval(rhs);
}

boost::json::value operator*(Expression const& expr)
{
    return expr.eval();
}

} // namespace lang
} // namespace zmbt

