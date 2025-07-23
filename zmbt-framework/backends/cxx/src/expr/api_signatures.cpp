/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/api_signatures.hpp"
#include "zmbt/expr/api.hpp"


namespace
{
    bool maybe_kv_pair(boost::json::value const& val)
    {
        if (!val.is_array())
        {
            return false;
        }
        auto const& pair = val.get_array();
        if (pair.size() != 2)
        {
            return false;
        }
        return pair.at(0).is_string();
    }

    bool maybe_object(boost::json::array const& value_list)
    {
        return value_list.cend() == std::find_if_not(value_list.cbegin(), value_list.cend(), maybe_kv_pair);
    }
} // namespace


namespace zmbt {
namespace lang {
namespace detail {
boost::json::value handle_list_init(std::initializer_list<Expression> set)
{
    auto const value_list = boost::json::value_from(set).as_array();

    if (maybe_object(value_list))
    {
        boost::json::object out{};
        out.reserve(value_list.size());
        for (auto const& pair: value_list)
        {
            auto const& kv = pair.get_array();
            out.insert_or_assign(kv.at(0).as_string(), kv.at(1));
        }
        return out;
    }
    else
    {
        return value_list;
    }
}
} // namespace detail



Expression SignatureErr::make_error(boost::json::string_view type, boost::json::string_view msg, boost::json::string_view ctx) const
{
    boost::json::object err{};
    if (not type.empty())
    {
        err["type"] = type;
    }
    if (not msg.empty())
    {
        err["message"] = msg;
    }
    if (not ctx.empty())
    {
        err["context"] = ctx;
    }
    return Expression(Expression::encodeNested(Keyword::Err, {err}));
}

} // namespace lang
} // namespace zmbt
