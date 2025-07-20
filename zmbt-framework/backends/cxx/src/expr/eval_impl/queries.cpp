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
boost::json::value query_at(boost::json::value const& value, boost::json::value const& at)
{
    boost::json::value query {};

    if (at.is_number() && value.is_structured())
    {
        auto x_as_array = value.if_array();
        auto x_as_object = value.if_object();

        auto const N = x_as_array ? x_as_array->size() : x_as_object ? x_as_object->size() : 0;

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
            return query;
        }

        try
        {
            if (x_as_array)
            {
                query = x_as_array->at(idx);
            }
            else if (x_as_object)
            {
                auto const kvpair = *(x_as_object->cbegin() + idx);
                query = boost::json::value_from(kvpair);
            }
        }
        catch(std::exception const&)
        {
            // ignore and return default
        }
    }
    else if (at.is_array())
    {
        query.emplace_array();
        for (auto const& jp: at.get_array())
        {
            query.get_array().push_back(query_at(value, jp));
        }
    }
    else if (at.is_object())
    {
        query.emplace_object();
        auto& as_object = query.get_object();
        for (auto const& kv: at.get_object())
        {
            if (kv.key().starts_with("$")) // dynamic key as ptr
            {
                try
                {
                    auto dynamic_key = query_at(value, kv.key().substr(1));
                    if (dynamic_key.is_number())
                    {
                        dynamic_key = zmbt::format("%s", dynamic_key);
                    }
                    as_object.emplace(dynamic_key.as_string(), query_at(value, kv.value()));
                }
                catch(std::exception const&)
                {
                    // ignore and return default
                }
            }
            else
            {
                as_object.emplace(kv.key(), query_at(value, kv.value()));
            }
        }
    }
    else if (at.is_string())
    {
        auto const& token = at.get_string();
        if (token.starts_with("/") or token.empty())
        {
            boost::json::error_code ec;
            if (boost::json::value const* ptr = value.find_pointer(at.get_string(), ec))
            {
                query = *ptr;
            }
        }
        else if (value.is_array())
        {
            auto const slice_idx = zmbt::detail::str_to_slice_idx(token);
            query = zmbt::slice(value.get_array(), slice_idx.at(0),slice_idx.at(1),slice_idx.at(2));
        }
        else if (auto const as_obj = value.if_object())
        {
            if (as_obj->contains(token))
            {
                query = as_obj->at(token);
            }
        }
    }
    return query;
}


} // namespace


namespace zmbt {
namespace lang {

ZMBT_DEFINE_EVALUATE_IMPL(At)
{
    ASSERT(not rhs().is_null(), "null parameter")
    return query_at(lhs(), rhs());
}

ZMBT_DEFINE_EVALUATE_IMPL(Lookup)
{
    ASSERT(not lhs().is_null(), "null argument")
    return query_at(rhs(), lhs());
}

} // namespace lang
} // namespace zmbt

