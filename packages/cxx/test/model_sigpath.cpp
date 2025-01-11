/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"

using namespace zmbt;

using namespace boost::json;

BOOST_AUTO_TEST_SUITE(SignalPathTest)

BOOST_AUTO_TEST_CASE(Test)
{
    DeferredFormat sp1 {"/lol"};
    BOOST_CHECK_EQUAL(DeferredFormat{"/lol"}.to_string(), "/lol");
    BOOST_CHECK_EQUAL((DeferredFormat{"/lol-%s-%s", Param{"kek"}, Param{42}}.to_string()), "/lol-${kek}-${42}");
}

BOOST_AUTO_TEST_CASE(Test2)
{
    DeferredFormat sp1 {{"/%s", "${1}"}};
    BOOST_CHECK_EQUAL(sp1.to_string(), "/${1}");
}


BOOST_AUTO_TEST_SUITE_END()

