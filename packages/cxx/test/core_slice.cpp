/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/core.hpp"

using namespace zmbt;


BOOST_AUTO_TEST_CASE(Slice)
{
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, 2, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, 2   ), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0      ), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}         ), (boost::json::array{0, 1, 2}));

    // overflow in span
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, 4, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -4, 3, 1), (boost::json::array{0, 1, 2}));

    // overflow outside span
    BOOST_CHECK_EQUAL(slice({0,1,2},  4,  4, 1), (boost::json::array{}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -4, -4, 1), (boost::json::array{}));

    // negat stop
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, -1, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, -2, 1), (boost::json::array{0, 1   }));

    // negat start
    BOOST_CHECK_EQUAL(slice({0,1,2}, -3, 2, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -2, 2, 1), (boost::json::array{   1, 2}));


    // reverse
    BOOST_CHECK_EQUAL(slice({0,1,2},  2, 0, -1), (boost::json::array{2,1,0}));
    BOOST_CHECK_EQUAL(slice({0,1,2},  0, 2, -1), (boost::json::array{}));

    // single element forward step yields identity, equivalent to python slice [0:1:1]
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 0, 1), (boost::json::array{42}));
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 0, 2), (boost::json::array{42}));
    // single element reverse step yields empty set, equivalent to python slice [0:1:-1]
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 0, -1), (boost::json::array{}));

    BOOST_CHECK_THROW(slice({42, 43},  0, 0, 0), base_error);
}

