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
#include "zmbt/expr/attributes.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/api.hpp"


namespace
{

using V = boost::json::value;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;


}


namespace zmbt {
namespace lang {

Encoding Expression::encodeLiteral(boost::json::value const& params)
{
    Encoding enc {};
    enc.push_back(Keyword::Literal, 0, params, nullptr);
    return enc;
}

Encoding Expression::encodePreProc(boost::json::value const& params)
{
    Encoding enc {};
    enc.push_back(Keyword::PreProc, 0, params, nullptr);
    return enc;
}

template <class T>
Encoding Expression::encodeNested(Keyword const& keyword, std::move_iterator<T> begin, std::move_iterator<T> const end)
{
    Encoding enc {};
    enc.push_back(keyword, 0, nullptr, nullptr);

    while(begin != end)
    {
        enc.append_to_root(std::move((*begin++).encoding_));
    }
    return enc;
}

Encoding Expression::encodeNested(Keyword const& keyword, std::initializer_list<Expression> subexpressions)
{
    return encodeNested(keyword, std::make_move_iterator(subexpressions.begin()), std::make_move_iterator(subexpressions.end()));
}

Encoding Expression::encodeNested(Keyword const& keyword, std::vector<Expression>&& subexpressions)
{
    return encodeNested(keyword, std::make_move_iterator(subexpressions.begin()), std::make_move_iterator(subexpressions.end()));
}


bool Expression::to_predicate_if_const(Expression& e)
{
    if (!e.is_noop() && e.is_const())
    {
        e = Expression(encodeNested(Keyword::Eq, {e}));
        return true;
    }
    return false;
}

}  // namespace lang
}  // namespace zmbt
