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

std::list<std::pair<std::string, std::string>> Expression::preprocessing_parameters() const
{
    std::list<std::pair<std::string, std::string>> pp;

    if (is_preproc()) // root-level is serialized as string literal
    {
        pp.emplace_back(to_json().as_string().c_str(), "");
        return pp;
    }

    for(auto const item: encoding_view())
    {
        if ((Keyword::PreProc == item.keyword) && item.data)
        {
            pp.emplace_back(item.data->as_string().c_str(), zmbt::format("/data/%d", item.index));
        }
    }

    return pp;
}

boost::json::value const& Expression::data() const
{
    auto const child = encoding_view().child(0);
    auto const a = attributes(child.head());
    if (!child.empty() && ((a & attr::is_literal) || (a & attr::is_preproc)))
    {
        return *child.front().data;
    }
    return *encoding_view().front().data;
}


std::size_t Expression::infix_size() const
{
    std::size_t n {0};
    for (auto const item: encoding_view())
    {
        if (item.depth == 1) ++n;
    }
    return n;
}

boost::json::value Expression::to_json() const
{
    return (is_literal() || is_preproc()) ? (*encoding_view().at(0).data) : boost::json::value_from(encoding());
}

bool Expression::is_const() const
{
    return encoding_view().is_const();
}

boost::json::string_view Expression::keyword_to_str() const
{
    return ::zmbt::lang::keyword_to_str(keyword());
}

bool Expression::is_boolean() const
{
    return encoding_view().is_boolean();
}


std::list<Expression> Expression::subexpressions_list() const
{
    std::list<Expression> result;

    auto subtrees = encoding_view().children();

    for (auto const& s: subtrees)
    {
        result.emplace_back(s.freeze());
    }
    return result;
}




}  // namespace lang
}  // namespace zmbt
