/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include <limits>
#include <boost/math/constants/constants.hpp>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"

namespace
{

using V = boost::json::value;
using L = boost::json::array;
using E = zmbt::lang::Expression;
using K = zmbt::lang::Keyword;
using O = zmbt::lang::Operator;

void deleteval_impl(boost::json::value const& at, boost::json::value& x)
{
    if(at == "")
    {
        x.emplace_null();
        return;
    }

    auto x_as_array = x.if_array();
    auto x_as_object = x.if_object();

    auto const N = x_as_array ? x_as_array->size() : x_as_object ? x_as_object->size() : 0;


    if (auto const delete_range = at.if_array())
    {
        boost::json::array sorted_delete_range;
        auto not_number = std::find_if(delete_range->cbegin(), delete_range->cend(), [](V const& v){ return not v.is_number(); });
        if (not_number == delete_range->cend())
        {
            sorted_delete_range = *delete_range;
        }
        else
        {
            sorted_delete_range.reserve(delete_range->size());

            for (auto const& el: *delete_range)
            {
                if (el.is_string()) { sorted_delete_range.push_back(el); }
                else if (el.is_number()) sorted_delete_range.push_back(zmbt::format("/%d", el).c_str());
            }
        }

        // reverse alphanumeric sort to handle nested JSON ptrs (array items are deleted from tail to head)
        std::stable_sort(
            sorted_delete_range.rbegin(),
            sorted_delete_range.rend(),
            [](auto const& a, auto const& b){
                return zmbt::lang::Operator::generic_less(a, b).as_bool();
            }
        );

        for (auto const& el: sorted_delete_range)
        {
            deleteval_impl(el, x);
        }
    }

    else if (not x.is_structured())
    {
        return;
    }
    else if (at.is_number())
    {
        std::size_t idx{};
        switch (at.kind())
        {
        case boost::json::kind::int64:
        case boost::json::kind::double_:
            {
                std::int64_t at_idx = boost::json::value_to<std::int64_t>(at);
                idx = (at_idx >= 0) ? at_idx : (at_idx + N); // TODO: handle overflow
            }
            break;
        case boost::json::kind::uint64:
            idx = at.get_uint64();
        default:
            break;
        }
        if (idx >= N) return;

        if (x_as_array)
        {
            auto pos = x_as_array->cbegin() + idx;
            x_as_array->erase(pos, pos + 1);
        }
        else if (x_as_object)
        {
            auto pos = x_as_object->cbegin() + idx;
            x_as_object->erase(pos);
        }
    }
    else if (auto const delete_at_string = at.if_string())
    {

        if (delete_at_string->starts_with("/") or delete_at_string->empty())
        {
            auto const last_token_idx = delete_at_string->find_last_of("/");
            auto const node_jp = delete_at_string->subview(0, last_token_idx);
            auto const element_jp = delete_at_string->subview(last_token_idx);

            if (last_token_idx == 0)
            {
                deleteval_impl(boost::json::parse(element_jp.substr(1)), x);
            }
            else // nested JSON pointer
            {
                boost::json::error_code ec;
                if (auto node_ptr = x.find_pointer(node_jp, ec))
                {
                    deleteval_impl(element_jp, *node_ptr);
                }
            }
        }
        // TODO: handle slice (or not?)
        else if (x_as_object)
        {
            x_as_object->erase(*delete_at_string);
        }
    }
}

} // namespace


namespace zmbt {
namespace lang {


ZMBT_DEFINE_EVALUATE_IMPL(Del)
{
    UNUSED_CTX;
    ASSERT(not rhs().is_null(), "invalid parameter")
    boost::json::value const at = rhs().eval();
    boost::json::value result = lhs().eval();

    deleteval_impl(at, result);
    return result;
}

} // namespace lang
} // namespace zmbt

