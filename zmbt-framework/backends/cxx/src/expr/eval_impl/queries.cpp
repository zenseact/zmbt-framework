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

using ExpressionView = zmbt::lang::ExpressionView;

boost::json::value query_at_impl(boost::json::value const& value, boost::json::value const& at)
{

    bool const at_is_number = at.is_number();
    auto const value_as_array = value.if_array();
    auto const value_as_object = value.if_object();
    auto const value_as_string = value.if_string();

    auto const at_as_array = at.if_array();
    auto const at_as_object = at.if_object();
    auto const at_as_string = at.if_string();

    boost::json::value result {};

    if (at_is_number && (value_as_array || value_as_object || value_as_string))
    {
        auto const N
            = value_as_array  ? value_as_array->size()
            : value_as_object ? value_as_object->size()
            : value_as_string ? value_as_string->size()
            : 0;

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

        if (idx >= N)
        {
            return result;
        }

        try
        {
            if (value_as_array)
            {
                result = value_as_array->at(idx);
            }
            else if (value_as_object)
            {
                auto const kvpair = *(value_as_object->cbegin() + idx);
                result = boost::json::value_from(kvpair);
            }
            else if (value_as_string)
            {
                result = value_as_string->at(idx);
            }
        }
        catch(std::exception const&)
        {
            // ignore and return default
        }
    }
    else if (at_as_array)
    {
        result.emplace_array();
        auto& result_as_array = result.get_array();

        for (auto const& jp: *at_as_array)
        {
            result_as_array.push_back(query_at_impl(value, jp));
        }
    }
    else if (at_as_object)
    {
        result.emplace_object();
        auto& result_as_object = result.get_object();
        for (auto const& kv: *at_as_object)
        {
            if (kv.key().starts_with("$")) // dynamic key as ptr
            {
                try
                {
                    auto dynamic_key = query_at_impl(value, kv.key().substr(1));
                    if (dynamic_key.is_number())
                    {
                        dynamic_key = zmbt::format("%s", dynamic_key);
                    }
                    result_as_object.emplace(dynamic_key.as_string(), query_at_impl(value, kv.value()));
                }
                catch(std::exception const&)
                {
                    // ignore and return default
                }
            }
            else
            {
                result_as_object.emplace(kv.key(), query_at_impl(value, kv.value()));
            }
        }
    }
    else if (at_as_string)
    {
        auto const& token = *at_as_string;
        if (token.starts_with("/") or token.empty())
        {
            boost::json::error_code ec;
            if (boost::json::value const* ptr = value.find_pointer(at.get_string(), ec))
            {
                result = *ptr;
            }
        }
        else if (value_as_array)
        {
            auto const slice_idx = zmbt::detail::str_to_slice_idx(token);
            result = zmbt::slice(*value_as_array, slice_idx.at(0),slice_idx.at(1),slice_idx.at(2));
        }
        else if (value_as_string)
        {
            auto const slice_idx = zmbt::detail::str_to_slice_idx(token);
            result = zmbt::slice(*value_as_string, slice_idx.at(0),slice_idx.at(1),slice_idx.at(2));
        }
        else if (value_as_object)
        {
            if (value_as_object->contains(token))
            {
                result = value_as_object->at(token);
            }
        }
    }
    return result;
}


boost::json::value query_at(ExpressionView const& value, ExpressionView const& query, zmbt::lang::EvalContext ctx)
{
    boost::json::value tmp_value, tmp_query;

    boost::json::value const& value_ref = value.is_literal() ? value.data() : (tmp_value = value.eval({}, ctx));
    boost::json::value const& query_ref = query.is_literal() ? query.data() : (tmp_query = query.eval({}, ctx));

    return query_at_impl(value_ref, query_ref);
}
} // namespace


namespace zmbt {
namespace lang {

ZMBT_DEFINE_EVALUATE_IMPL(At)
{
    return query_at(lhs(), rhs(), curr_ctx());
}

ZMBT_DEFINE_EVALUATE_IMPL(Lookup)
{
    return query_at(rhs(), lhs(), curr_ctx());
}

} // namespace lang
} // namespace zmbt

