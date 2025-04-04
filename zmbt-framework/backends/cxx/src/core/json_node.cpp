/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/core/json_node.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <string>
#include <vector>

#include "zmbt/core/exceptions.hpp"

namespace {

namespace js = boost::json;

template <class T>
boost::json::string join(T const& tokens)
{
    boost::json::string q;
    for (auto const& token: tokens)
    {
        q.append("/").append(token);
    }
    return q;
}



void emplace_as(boost::json::kind kind, boost::json::value& node)
{

    switch (kind)
    {
    case boost::json::kind::object : node.emplace_object() ; break;
    case boost::json::kind::array  : node.emplace_array()  ; break;
    case boost::json::kind::string : node.emplace_string() ; break;
    case boost::json::kind::double_: node.emplace_double() ; break;
    case boost::json::kind::bool_  : node.emplace_bool()   ; break;
    case boost::json::kind::uint64 : node.emplace_uint64() ; break;
    case boost::json::kind::int64  : node.emplace_int64()  ; break;
    default: break;
    }
}


boost::json::string  kind2str(boost::json::kind const& k)
{
    switch (k)
    {
    case boost::json::kind::object : return "object";
    case boost::json::kind::array  : return "array";
    case boost::json::kind::string : return "string";
    case boost::json::kind::double_: return "double";
    case boost::json::kind::bool_  : return "bool";
    case boost::json::kind::uint64 : return "uint64";
    case boost::json::kind::int64  : return "int64";
    case boost::json::kind::null  : return "null";
    default: return "undefined";
    }
}

} // namespace


namespace zmbt {


JsonNode::JsonNode(boost::json::string_view node_ptr, std::shared_ptr<boost::json::value> root)
    : root_{root}
    , node_ptr_{node_ptr}
{
}


boost::json::value& JsonNode::get_or_create(boost::json::string_view json_ptr)
{
    boost::json::error_code errcode;
    boost::json::value* ptr = node().find_pointer(json_ptr, errcode);

    // boost::json::set_pointer_options set_pointer_options {};
    // set_pointer_options.replace_any_scalar = false;
    if (!errcode.failed() && ptr)
    {
        return *ptr;
    }
    else
    {
        auto& rv = node().set_at_pointer(json_ptr, nullptr);
        return rv;
    }
}


boost::json::string JsonNode::resolve_tokens(boost::json::string_view query) const
{
    if (query.empty()) { return ""; }

    std::vector<boost::json::string> tokens;
    boost::split(
        tokens,
        query.starts_with('/') ? query.substr(1) : query,
        boost::is_any_of("/")
    );

    for (std::size_t idx = 0; idx < tokens.size(); idx++)
    {
        auto& token = tokens[idx];
        if ((token != "+") and (token != "@")) { continue; }

        auto subquery = join(tokens | boost::adaptors::sliced(0, idx));

        boost::json::error_code errcode;
        boost::json::value const* subnode = node().find_pointer(subquery, errcode);

        if (errcode.failed() || !subnode || subnode->is_null())
        {
            token = "0";
            continue;
        }

        if (subnode and !subnode->is_array())
        {
            throw base_error(
                "%s token is not applicable at node %s (not an array = %s)", token, subquery, *subnode
            );
            continue;
        }

        auto const& array = subnode->get_array();

        if (array.empty())
        {
            token = "0";
        }
        else if (token == "+")
        {
            token = std::to_string(array.size());
        }
        else if (token == "@")
        {
            token = std::to_string(array.size() - 1);
        }
    }

    return join(tokens);
}


boost::json::value& JsonNode::get_or_create_as(boost::json::kind request_kind, boost::json::string_view json_ptr)
{
    boost::json::value& node = get_or_create(resolve_tokens(json_ptr));
    if (node.is_null()) emplace_as(request_kind, node);

    if (request_kind != node.kind())
    {
        throw base_error("%s node can't be resolved as %s, value: %s"
            , json_ptr
            , kind2str(request_kind)
            , node
        );
    }
    return node;
}


boost::json::value const& JsonNode::at(boost::json::string_view json_ptr) const
{
    if (!contains(json_ptr))
    {
        throw base_error(
            "JsonNode: No \"%s%s\" found"
            , node_ptr()
            , json_ptr
        );
    }
    return node().at_pointer(resolve_tokens(json_ptr));
}


/// create JsonNode proxy over subnode
JsonNode JsonNode::branch(boost::json::kind kind, boost::json::string_view subnode_ptr)
{
    auto new_ptr = boost::json::string(node_ptr()).append(resolve_tokens(subnode_ptr));
    boost::json::value& new_node = get_or_create(new_ptr);

    if (new_node.is_null()) { emplace_as(kind, new_node); }
    if ((kind != boost::json::kind::null) && (new_node.kind() != kind))
    {
        throw base_error("%s node can't be resolved as %s, value: %s"
            , subnode_ptr
            , kind2str(kind)
            , new_node
        );
    }
    return JsonNode(new_ptr, root());
}


}
