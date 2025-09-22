/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_JSON_NODE_HPP_
#define ZMBT_CORE_JSON_NODE_HPP_


#include <boost/json.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

#include "format_string.hpp"

namespace zmbt {



/// \brief Mutator/accessor wrapper over shared JSON value
/// \details
///
/// \b
/// Provides read/write access to the referenced value internal structure via JSON Pointer.
/// Each instance owns a shared pointer to the root value, so subnode proxies may prolong original object lifetime.
///
/// \b
/// Additions to the standard JSON Pointer syntax:
/// At-the-end token: '@'. Points to the last element of an array.
/// Queries with at-the-end token always results in creating a new node on empty array.
/// Past-the-end token: '+'. Points to the new element of an array.
/// Queries with past-the-end token always results in creating a new node.
///
/// \b
/// The rules for the additional tokens are:
///
/// \b
/// For each @ token
///  - if the node is null, the token substituted with 0; otherwise
///  - if the node is an empty array, the token substituted with 0; otherwise
///  - if the node is a non-empty array, the token substituted with node size - 1; otherwise
///  - an error is produced.
///
/// \b
/// For each + token
///  - if the node is null, the token substituted with 0; otherwise
///  - if the node is an array, the token substituted with node size; otherwise
///  - an error is produced.
///
class JsonNode
{
    std::shared_ptr<boost::json::value> root_;
    boost::json::string node_ptr_;

    boost::json::value& get_or_create(boost::json::string_view json_ptr);

    boost::json::string_view validate_jptr(boost::json::string_view query) const;

    boost::json::value& get_or_create_as(boost::json::kind request_kind, boost::json::string_view json_ptr);

    JsonNode(boost::json::string_view node_ptr, std::shared_ptr<boost::json::value> root);


public:

    ~JsonNode() noexcept = default;

    explicit JsonNode(std::shared_ptr<boost::json::value> root)
        : JsonNode("", root)
    {
    }

    JsonNode()
        : JsonNode(std::make_shared<boost::json::value>())
    {
    }

    explicit JsonNode(boost::json::storage_ptr sp)
        : JsonNode(std::make_shared<boost::json::value>(sp))
    {
    }

    JsonNode(JsonNode const& o)
        : JsonNode(o.node())
    {
    }

    JsonNode(JsonNode & o)
        : JsonNode(o.node_ptr(), o.root())
    {
    }

    JsonNode(JsonNode && o)
        : JsonNode(std::move(o.node_ptr_), std::move(o.root_))
    {
    }


    JsonNode(boost::json::value const& v) : JsonNode(std::make_shared<boost::json::value>(v)) {}
    JsonNode(boost::json::value&& v) : JsonNode(std::make_shared<boost::json::value>(std::move(v))) {}

    std::shared_ptr<boost::json::value> const& root() const { return root_; }

    JsonNode root_node() const { return JsonNode{root()}; }

    boost::json::string_view node_ptr() const { return node_ptr_; }

    JsonNode copy() const
    {
        return JsonNode{node()};
    }

    boost::json::value const* node(boost::json::error_code &ec) const
    {
        return root()->find_pointer(node_ptr(), ec);
    }

    boost::json::value* node(boost::json::error_code &ec)
    {
        JsonNode const& self = *this;
        return const_cast<boost::json::value*>(self.node(ec));
    }

    boost::json::value const& node() const {
        return root()->at_pointer(node_ptr());
    }

    boost::json::value& node() {
        JsonNode const& self = *this;
        return const_cast<boost::json::value&>(self.node());
    }


    boost::json::array const& as_array() const {
        return node().as_array();
    }

    boost::json::array& as_array() {
        JsonNode const& self = *this;
        return const_cast<boost::json::array&>(self.as_array());
    }


    boost::json::object const& as_object() const {
        return node().as_object();
    }

    boost::json::object& as_object() {
        JsonNode const& self = *this;
        return const_cast<boost::json::object&>(self.as_object());
    }

    JsonNode& operator=(JsonNode const&) = default;
    JsonNode& operator=(JsonNode &&) = default;


    boost::json::value const& at(boost::json::string_view json_ptr) const;

    template <class T, class... A>
    boost::json::value const& at(boost::json::string_view fmtstr, T&& first, A&&... rest) const
    {
        return at(format(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }

    boost::json::value const* find_pointer(boost::json::string_view json_ptr) const
    {
        boost::json::error_code ignored;
        return node().find_pointer(validate_jptr(json_ptr), ignored);
    }


    template <class T, class... A>
    boost::json::value const* find_pointer(boost::json::string_view fmtstr, T&& first, A&&... rest) const
    {
        return find_pointer(format(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }

    template <class T, class... A>
    boost::json::value* find_pointer(boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        JsonNode const& self = *this;
        return const_cast<boost::json::value*>(self.find_pointer(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }

    bool contains(boost::json::string_view json_ptr) const
    {
        return find_pointer(json_ptr);
    }

    template <class T, class... A>
    bool contains(boost::json::string_view fmtstr, T&& first, A&&... rest) const
    {
        return contains(format(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }


    boost::json::value& operator()(boost::json::string_view json_ptr = "")
    {
        return this->get_or_create(validate_jptr(json_ptr));
    }

    template <class T, class... A>
    boost::json::value& operator()(boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        return this->operator()(format(fmtstr, first, rest...));
    }


    boost::json::value const& operator()(boost::json::string_view json_ptr = "") const
    {
        return at(json_ptr);
    }

    template <class T, class... A>
    boost::json::value const& operator()(boost::json::string_view fmtstr, T&& first, A&&... rest) const
    {
        return at(format(fmtstr, first, rest...));
    }

    boost::json::value get_or_default(boost::json::string_view json_ptr, boost::json::value const& dflt) const
    {
        boost::json::value const* ptr;
        return (ptr = find_pointer(json_ptr)) ? *ptr : dflt;
    }

    boost::json::array& get_or_create_array(boost::json::string_view json_ptr = "")
    {
        return get_or_create_as(boost::json::kind::array, json_ptr).as_array();
    }
    template <class T, class... A>
    boost::json::array& get_or_create_array(boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        return get_or_create_array(format(fmtstr, first, rest...));
    }

    boost::json::object& get_or_create_object(boost::json::string_view json_ptr = "")
    {
        return get_or_create_as(boost::json::kind::object, json_ptr).as_object();
    }


    template <class T, class... A>
    boost::json::object& get_or_create_object(boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        return get_or_create_object(format(fmtstr, first, rest...));
    }


    friend std::ostream& operator<<(std::ostream& os, JsonNode const& view)
    {
        os << view.node();
        return os;
    }

    /// create JsonNode proxy over subnode
    JsonNode branch(boost::json::kind kind, boost::json::string_view subnode_ptr);

    JsonNode branch(boost::json::string_view subnode_ptr)
    {
        return branch(boost::json::kind::null, subnode_ptr);
    }

    template <class T, class... A>
    JsonNode branch(boost::json::kind kind, boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        return branch(kind, format(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }

    template <class T, class... A>
    JsonNode branch(boost::json::string_view fmtstr, T&& first, A&&... rest)
    {
        return branch(format(fmtstr, std::forward<T>(first), std::forward<A>(rest)...));
    }

    bool is_null() const
    {
        return node().is_null();
    }

    std::size_t size() const
    {
        switch (node().kind())
        {
        case boost::json::kind::array:
            return as_array().size();
            break;
        case boost::json::kind::object:
            return as_object().size();
            break;
        default:
            return 0;
            break;
        }
    }
};

}

#endif