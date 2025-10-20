/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/reflect.hpp"


using namespace zmbt;


BOOST_AUTO_TEST_CASE(ObjectIdSerialization)
{
    BOOST_CHECK_NO_THROW(zmbt::json_from(object_id{nullptr}));
    BOOST_CHECK_NO_THROW(zmbt::dejsonize<object_id>(boost::json::array{"addr",0U}));
}

BOOST_AUTO_TEST_CASE(InterfaceIdSerialization)
{
    BOOST_CHECK_NO_THROW(zmbt::json_from(interface_id{}));
    BOOST_CHECK_NO_THROW(zmbt::dejsonize<interface_id>(boost::json::array{"addr",0U}));
}
