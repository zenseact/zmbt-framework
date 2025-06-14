/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include "zmbt/model.hpp"

using namespace zmbt::dsl;


std::vector<boost::json::array> const OrderTestData
{
    {1,2,3},
    {"a","b","c"},
    {{1,2}, {3,4}, {5,6}},
};


BOOST_DATA_TEST_CASE(GenericOrder, OrderTestData)
{
    constexpr auto le = &Operator::generic_less;

    auto const A = sample.at(0);
    auto const B = sample.at(1);
    auto const C = sample.at(2);

    // Irreflexivity
    BOOST_CHECK(not le(A, A));
    // Assymetry
    BOOST_CHECK(le(A, B) ? !le(B, A) : true);
    // Transitivity
    BOOST_CHECK(le(A, B) && le(B, C) ? le(A, C) : true);

    // Equivalence Transitivity
    auto equiv = [](auto const& a, auto const& b) {
        return !le(a, b) && !le(b, a);
    };
    BOOST_CHECK((equiv(A, B) && equiv(B, C)) ? equiv(A, C) : true);
}
