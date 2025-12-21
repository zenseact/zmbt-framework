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
        // TODO: optionally data traversing this with cli flag
        else if((Keyword::Literal == item.keyword) && item.data)
        {
            JsonTraverse([&](boost::json::value const& node, std::string const jp) -> bool {
                if (Encoding::is_preproc_token(node))
                {
                    pp.emplace_back(node.as_string().c_str(), zmbt::format("/data/%d%s", item.index, jp));
                }
                return false;
            })(*item.data);
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

bool ExpressionView::is_valid_link() const
{
    auto const self = encoding_view();
    return is(Keyword::Fn) && (self.arity() == 2);
}

bool ExpressionView::is_infix_pipe() const
{
    return is_compose() && (encoding_view().arity() > 1);
}

bool ExpressionView::is_infix_tuple() const
{
    if (not (is_tuple() && (encoding_view().arity() > 1)))
    {
        return false;
    }
    return encoding_view().child(0).front().keyword != Keyword::Tuple;
}


bool ExpressionView::is_infix_fork() const
{
    return is_fork() && encoding_view().arity() == 2;
}


std::vector<ExpressionView> ExpressionView::link_parameters() const
{
    std::vector<ExpressionView> result;

    auto tuple = encoding_view();
    if (tuple.head() != Keyword::Fn) return result;

    for (auto const& s: tuple.children())
    {
        result.emplace_back(s);
    }
    return result;
}


std::vector<ExpressionView> ExpressionView::subexpressions_list() const
{
    auto subtrees = encoding_view().children();
    std::vector<ExpressionView> result;
    result.reserve(subtrees.size());
    std::transform(subtrees.cbegin(), subtrees.cend(),
               std::back_inserter(result),
               [](EncodingView const& v) { return ExpressionView{v}; });
    return result;
}

boost::json::value ExpressionView::to_json() const
{
    if (is(Keyword::LazyToken))
    {
        return nullptr;
    }
    else if (is_literal() || is_preproc() || is_link())
    {
        return *encoding_view().at(0).data;
    }
    else
    {
        return encoding_view().to_json();
    }
}


std::string ExpressionView::error_id() const
{
    if (!is_error()) return "";

    auto const& payload = data().as_object();
    auto const type_name = payload.at("type").as_string();
    auto const msg       = payload.at("message").as_string();
    auto const ctx       = payload.contains("context") ? payload.at("context").as_string() : "";
    auto const hash = std::hash<boost::json::string>();
    return format("$(%s-%s-%s)", type_name, hash(msg), hash(ctx));
}

}  // namespace lang
}  // namespace zmbt
