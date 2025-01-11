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
    BOOST_CHECK(Environment::IfcRec(nullptr,  return_int)    .interface() == interface_id(return_int));
    BOOST_CHECK(Environment::IfcRec(nullptr,  increment_args).interface() == interface_id(increment_args));

    BOOST_CHECK(Environment::IfcRec(nullptr, &return_int).interface()     == interface_id(&return_int)    );
    BOOST_CHECK(Environment::IfcRec(nullptr, &increment_args).interface() == interface_id(&increment_args));
}

BOOST_AUTO_TEST_CASE(Functor)
{
    O fn {};
    BOOST_CHECK(Environment::IfcRec(nullptr, fn) .interface() == interface_id(fn));
    BOOST_CHECK(Environment::IfcRec(nullptr, &fn).interface() == interface_id(&fn));
}

BOOST_AUTO_TEST_CASE(MemberFunctionInterfaceId)
{
    O obj {};
    BOOST_CHECK(Environment::IfcRec(&obj, &O::set_x).interface()    == interface_id(&O::set_x));
    BOOST_CHECK(Environment::IfcRec(&obj, &O::set_x).interface()    == interface_id(&O::set_x));
    BOOST_CHECK(Environment::IfcRec(nullptr, &O::set_x).interface() == interface_id(&O::set_x));
}

BOOST_AUTO_TEST_CASE(RefobjNullptr)
{
    BOOST_CHECK(Environment::IfcRec(nullptr, &O::set_x).refobj() == object_id(nullptr));
}

BOOST_AUTO_TEST_CASE(RefobjRefVsPtr)
{
    O obj {};
    BOOST_CHECK(Environment::IfcRec(obj, &O::set_x).refobj() == Environment::IfcRec(&obj, &O::set_x).refobj());
}

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE(InterfaceRecordTests);



BOOST_AUTO_TEST_CASE(IRFunction)
{
    BOOST_CHECK(InterfaceRecord(nullptr,  return_int)    .interface() == interface_id(return_int));
    BOOST_CHECK(InterfaceRecord(nullptr,  increment_args).interface() == interface_id(increment_args));
    BOOST_CHECK(InterfaceRecord(nullptr, &return_int)    .interface() == interface_id(&return_int));
    BOOST_CHECK(InterfaceRecord(nullptr, &increment_args).interface() == interface_id(&increment_args));
}

BOOST_AUTO_TEST_CASE(IRFunctor)
{
    O fn {};
    BOOST_CHECK(InterfaceRecord(nullptr, fn) .interface() == interface_id(fn));
    BOOST_CHECK(InterfaceRecord(nullptr, &fn).interface() == interface_id(&fn));
}


BOOST_AUTO_TEST_CASE(IRMemberFunctionInterfaceId)
{
    O obj {};
    BOOST_CHECK(InterfaceRecord(obj, &O::set_x)    .interface() == interface_id(&O::set_x));
    BOOST_CHECK(InterfaceRecord(&obj, &O::set_x)   .interface() == interface_id(&O::set_x));
    BOOST_CHECK(InterfaceRecord(nullptr, &O::set_x).interface() == interface_id(&O::set_x));
}


BOOST_AUTO_TEST_CASE(IRRefobjNullptr)
{
    auto lambda = [](){};
    BOOST_CHECK_EQUAL(InterfaceRecord(lambda).refobj().annotation(), type_name<nullptr_t>());
    BOOST_CHECK_EQUAL(InterfaceRecord(nullptr, lambda).refobj().annotation(), type_name<nullptr_t>());

    BOOST_CHECK_EQUAL(InterfaceRecord(&O::set_x).refobj().annotation(), type_name<O>());
    BOOST_CHECK_EQUAL(InterfaceRecord(nullptr, &O::set_x).refobj().annotation(), type_name<nullptr_t>());
}

BOOST_AUTO_TEST_CASE(IRRefobjRefVsPtr)
{
    O obj {};
    BOOST_CHECK(InterfaceRecord(obj, &O::set_x).refobj() == InterfaceRecord(&obj, &O::set_x).refobj());
}

BOOST_AUTO_TEST_SUITE_END();
