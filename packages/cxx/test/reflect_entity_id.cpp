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
    BOOST_CHECK_NO_THROW(zmbt::reflect::json_from(object_id{nullptr}));
    BOOST_CHECK_NO_THROW(zmbt::reflect::dejsonize<object_id>("[addr](type)"));
}

BOOST_AUTO_TEST_CASE(InterfaceIdSerialization)
{
    BOOST_CHECK_NO_THROW(zmbt::reflect::json_from(interface_id{}));
    BOOST_CHECK_NO_THROW(zmbt::reflect::dejsonize<interface_id>("[addr](type)"));
}
