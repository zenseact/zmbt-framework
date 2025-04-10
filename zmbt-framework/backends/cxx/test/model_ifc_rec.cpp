/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"


using namespace zmbt;

namespace {

struct O {
    double x;
    int operator()(int y) const& { return y;}
    void set_x(double newx) { x = newx; }
};


int return_int(int) { return 42; }

void increment_args(int& x, int& y) { ++x; ++y; }

}


BOOST_AUTO_TEST_SUITE(EnvironementTests);


BOOST_AUTO_TEST_CASE(Function)
{
    BOOST_CHECK(Environment::InterfaceHandle( return_int    , nullptr)    .interface() == interface_id(return_int));
    BOOST_CHECK(Environment::InterfaceHandle( increment_args, nullptr).interface() == interface_id(increment_args));
    BOOST_CHECK(Environment::InterfaceHandle(&return_int    , nullptr).interface()     == interface_id(&return_int)    );
    BOOST_CHECK(Environment::InterfaceHandle(&increment_args, nullptr).interface() == interface_id(&increment_args));
}

BOOST_AUTO_TEST_CASE(Functor)
{
    O fn {};
    BOOST_CHECK(Environment::InterfaceHandle(fn , nullptr) .interface() == interface_id(fn));
    BOOST_CHECK(Environment::InterfaceHandle(&fn, nullptr).interface() == interface_id(&fn));
}

BOOST_AUTO_TEST_CASE(MemberFunctionInterfaceId)
{
    O obj {};
    BOOST_CHECK(Environment::InterfaceHandle(&O::set_x, &obj).interface()    == interface_id(&O::set_x));
    BOOST_CHECK(Environment::InterfaceHandle(&O::set_x, &obj).interface()    == interface_id(&O::set_x));
    BOOST_CHECK(Environment::InterfaceHandle(&O::set_x, nullptr).interface() == interface_id(&O::set_x));
}

BOOST_AUTO_TEST_CASE(RefobjNullptr)
{
    BOOST_CHECK(Environment::InterfaceHandle(&O::set_x, nullptr).refobj() == object_id(nullptr));
}

BOOST_AUTO_TEST_CASE(RefobjRefVsPtr)
{
    O obj {};
    BOOST_CHECK_EQUAL(Environment::InterfaceHandle(&O::set_x, obj).refobj(), Environment::InterfaceHandle(&O::set_x, &obj).refobj());
}

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE(InterfaceRecordTests);



BOOST_AUTO_TEST_CASE(IRFunction)
{
    BOOST_CHECK(InterfaceRecord( return_int    , nullptr)    .interface() == interface_id(return_int));
    BOOST_CHECK(InterfaceRecord( increment_args, nullptr).interface() == interface_id(increment_args));
    BOOST_CHECK(InterfaceRecord(&return_int    , nullptr)    .interface() == interface_id(&return_int));
    BOOST_CHECK(InterfaceRecord(&increment_args, nullptr).interface() == interface_id(&increment_args));
}

BOOST_AUTO_TEST_CASE(IRFunctor)
{
    O fn {};
    BOOST_CHECK(InterfaceRecord(fn , nullptr) .interface() == interface_id(fn));
    BOOST_CHECK(InterfaceRecord(&fn, nullptr).interface() == interface_id(&fn));
}


BOOST_AUTO_TEST_CASE(IRMemberFunctionInterfaceId)
{
    O obj {};
    BOOST_CHECK(InterfaceRecord(&O::set_x, obj    )    .interface() == interface_id(&O::set_x));
    BOOST_CHECK(InterfaceRecord(&O::set_x, &obj   )   .interface() == interface_id(&O::set_x));
    BOOST_CHECK(InterfaceRecord(&O::set_x, nullptr).interface() == interface_id(&O::set_x));
}


BOOST_AUTO_TEST_CASE(IRRefobjNullptr)
{
    auto lambda = [](){};
    BOOST_CHECK_EQUAL(InterfaceRecord(lambda).refobj().annotation(), type_name<nullptr_t>());
    BOOST_CHECK_EQUAL(InterfaceRecord(lambda, nullptr).refobj().annotation(), type_name<nullptr_t>());

    BOOST_CHECK_EQUAL(InterfaceRecord(&O::set_x).refobj().annotation(), type_name<O>());
    BOOST_CHECK_EQUAL(InterfaceRecord(&O::set_x, nullptr).refobj().annotation(), type_name<nullptr_t>());
}

BOOST_AUTO_TEST_CASE(IRRefobjRefVsPtr)
{
    O obj {};
    BOOST_CHECK_EQUAL(InterfaceRecord(&O::set_x, obj).refobj(), InterfaceRecord(&O::set_x, &obj).refobj());
}

BOOST_AUTO_TEST_SUITE_END();
