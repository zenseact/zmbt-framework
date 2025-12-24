/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/core.hpp"

using namespace zmbt;
using namespace boost::json;


BOOST_AUTO_TEST_CASE(JsonNodeCtors)
{
    BOOST_CHECK_NO_THROW(( [](){ (void) JsonNode();                    }() ));
    BOOST_CHECK_NO_THROW(( [](){ (void) JsonNode{{"hello", "kitty"}};    }() ));
    BOOST_CHECK_NO_THROW(( [](){ (void) JsonNode{{{"hello", "kitty"}}};  }() ));

    BOOST_CHECK_NO_THROW(( [](boost::json::value &&     v){ (void) JsonNode{std::move(v)}; }({"hello", "kitty"}) ));
    BOOST_CHECK_NO_THROW(( [](boost::json::value const& v){ (void) JsonNode{v};            }({"hello", "kitty"}) ));
}


BOOST_AUTO_TEST_CASE(JsonNodePointerManipulation)
{
    auto node = JsonNode{{{"hello", "kitty"}}};
    BOOST_CHECK_EQUAL(node.at("/hello"), "kitty");
    node("/hello") = "bad kitty";
    BOOST_CHECK_EQUAL(node.at("/hello"), "bad kitty");

    BOOST_CHECK_THROW(node.at("/object"), std::exception);
    node("/object") = {{"lol", 13}, {"kek", 42}};
    BOOST_CHECK(node.at("/object").is_object());
    BOOST_CHECK_EQUAL(node.at("/object/lol"), 13);
    BOOST_CHECK_EQUAL(node.at("/object/kek"), 42);
}


BOOST_AUTO_TEST_CASE(JsonNodeFormattedPointerManipulation)
{
    auto node = JsonNode{};
    node("/object") = {{"lol", 13}, {"kek", 42}};
    node("/object/%s", "lol") = node.at("/object/%s", "kek");
    BOOST_CHECK_EQUAL(node.at("/object/lol"), 42);
}


BOOST_AUTO_TEST_CASE(JsonNodeSharingData)
{
    auto init_node = JsonNode{{{"hello", "kitty"}}};
    auto copy_node = JsonNode(init_node.node());
    auto root_proxy = JsonNode(init_node);
    auto subnode_proxy = init_node.branch("/hello");

    BOOST_CHECK_EQUAL(init_node.node(), copy_node.node());

    BOOST_CHECK_EQUAL(subnode_proxy.node().as_string(), "kitty");

    subnode_proxy.node() = "bad kitty";
    BOOST_CHECK_EQUAL(init_node.at("/hello"), "bad kitty");
    BOOST_CHECK_EQUAL(root_proxy.at("/hello"), "bad kitty");
    BOOST_CHECK_EQUAL(copy_node.at("/hello"), "kitty");
}


BOOST_AUTO_TEST_CASE(JsonNodeOperatorSubscript)
{
    auto op = JsonNode{{{"hello", "kitty"}}};
    BOOST_CHECK_EQUAL(op("/hello"), "kitty");
    BOOST_CHECK(op("/lol/kek").is_null());
    op("/lol/kek") = 42;
    BOOST_CHECK_EQUAL(op("/lol/kek"), 42);
}


BOOST_AUTO_TEST_CASE(JsonNodeGetOrCreate)
{
    auto op = JsonNode{{{"hello", "kitty"}}};
    BOOST_CHECK_THROW(op.get_or_create_array("/hello"), std::exception);
    BOOST_CHECK(op("/array").is_null());
    BOOST_CHECK_NO_THROW(op.get_or_create_array("/array"));
    BOOST_CHECK(op.at("/array").is_array());
    BOOST_CHECK_EQUAL(op.get_or_create_array("/array").size(), 0);

    BOOST_CHECK_THROW(op.get_or_create_object("/hello"), std::exception);
    BOOST_CHECK(op("/object").is_null());
    BOOST_CHECK_NO_THROW(op.get_or_create_object("/object"));
    BOOST_CHECK(op.at("/object").is_object());
    BOOST_CHECK_EQUAL(op.get_or_create_object("/object").size(), 0);
}


BOOST_AUTO_TEST_CASE(JsonNodeGetOrCreateOnTemporary)
{
    auto op = JsonNode{{{"hello", "kitty"}}};
    boost::json::array& arr = JsonNode(op).get_or_create_array("/array");
    BOOST_CHECK_EQUAL(arr.size(), 0);
    BOOST_CHECK(op.at("/array").is_array());
}
