/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/math/constants/constants.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "zmbt/expr.hpp"

namespace utf = boost::unit_test;
using zmbt::lang::Keyword;

BOOST_AUTO_TEST_CASE(SerializationSpeed, *utf::timeout(1))
{
    boost::json::string const test_kw = zmbt::json_from(Keyword::PSuperset).as_string();
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = zmbt::dejsonize<Keyword>(test_kw);
        if (kw != Keyword::PSuperset)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}

BOOST_AUTO_TEST_CASE(SerializationUndefinedSpeed, *utf::timeout(1))
{
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = zmbt::dejsonize<Keyword>("ArbitraryStringNotPresentInKeywords");
        if (kw != Keyword::Undefined)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}
