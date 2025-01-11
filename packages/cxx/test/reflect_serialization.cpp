/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/reflect.hpp"

struct Dummy
{
    int x;
};

BOOST_DESCRIBE_STRUCT(Dummy, (), (x))


BOOST_AUTO_TEST_CASE(SerializeStaticCArray)
{
    Dummy const carr_in[3] {{1},{2},{3}};
    Dummy carr_out[3] {{}, {}, {}};

    boost::json::array serialized = zmbt::reflect::json_from_array(carr_in).as_array();
    zmbt::reflect::dejsonize_array(serialized, carr_out);
    BOOST_CHECK_EQUAL(carr_in[0].x, carr_out[0].x);
    BOOST_CHECK_EQUAL(carr_in[1].x, carr_out[1].x);
    BOOST_CHECK_EQUAL(carr_in[2].x, carr_out[2].x);
}


struct JsonConvertible
{
    JsonConvertible() {}
    JsonConvertible(boost::json::value const&) {}
    operator boost::json::value() const { return {}; }
};

ZMBT_INJECT_JSON_TAG_INVOKE

BOOST_AUTO_TEST_CASE(SerializeByConversionOp)
{

    BOOST_CHECK((std::is_convertible<JsonConvertible, boost::json::value>::value));
    BOOST_CHECK((std::is_convertible<boost::json::value, JsonConvertible>::value));

    JsonConvertible test{};
    BOOST_CHECK_EQUAL(zmbt::reflect::json_from(test), boost::json::value{});
    BOOST_CHECK_NO_THROW(zmbt::reflect::dejsonize<JsonConvertible>(boost::json::value{}));
}
