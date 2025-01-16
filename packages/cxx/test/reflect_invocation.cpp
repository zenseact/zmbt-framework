/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/reflect.hpp"

struct O {
    double x;
    int operator()(int y) const& { return y; }
    void set_x(double newx) {
        x = newx;
    }
};

int freefun(int x) { return x; }

BOOST_AUTO_TEST_SUITE(InvocationMetafn)

BOOST_AUTO_TEST_CASE(ApplyFreeFun)
{
    using reflection = zmbt::reflect::invocation<decltype(&freefun)>;
    auto ret = reflection::apply(nullptr, freefun, {42});
    BOOST_TEST(ret == 42);
}

BOOST_AUTO_TEST_CASE(ApplyFunctor)
{
    using reflection = zmbt::reflect::invocation<O*>;
    O obj;

    auto ret = reflection::apply(nullptr, &obj, {42});
    BOOST_TEST(ret == 42);
}

BOOST_AUTO_TEST_CASE(ApplyMemfun)
{
    using reflection = zmbt::reflect::invocation<decltype(&O::set_x)>;
    O obj;

    reflection::apply(obj, &O::set_x, {42});
    BOOST_TEST(obj.x == 42);
}

BOOST_AUTO_TEST_SUITE_END()
