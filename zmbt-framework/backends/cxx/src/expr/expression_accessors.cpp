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

std::list<std::pair<std::string, std::string>> ExpressionView::preprocessing_parameters() const
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

boost::json::value const& ExpressionView::data() const
{
    auto const child = encoding_view().child(0);
    auto const a = attributes(child.head());
    if (!child.empty() && ((a & attr::is_literal) || (a & attr::is_preproc)))
    {
        return *child.front().data;
    }
    return encoding_view().empty() ? expr::Noop.data() : *encoding_view().front().data;
}

bool ExpressionView::is_const() const
{
    return encoding_view().is_const();
}

boost::json::string_view ExpressionView::keyword_to_str() const
{
    return ::zmbt::lang::keyword_to_str(keyword());
}

bool ExpressionView::is_boolean() const
{
    return encoding_view().is_boolean();
}


std::list<ExpressionView> ExpressionView::subexpressions_list() const
{
    std::list<ExpressionView> result;

    auto subtrees = encoding_view().children();

    for (auto const& s: subtrees)
    {
        result.emplace_back(s);
    }
    return result;
}

boost::json::value ExpressionView::to_json() const
{
    // TODO: optimize value_from EncodingView
    return (is_literal() || is_preproc() || is_capture()) ? (*encoding_view().at(0).data) : boost::json::value_from(encoding_view().freeze());
}

}  // namespace lang
}  // namespace zmbt
