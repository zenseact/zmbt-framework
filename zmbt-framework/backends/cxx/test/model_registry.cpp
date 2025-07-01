/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"


using namespace zmbt;



BOOST_AUTO_TEST_CASE(RegAction)
{
    Environment env {};
    int test_var {0};

    env.RegisterAction("increment test_var", [&](){ test_var++; });
    env.RunAction("increment test_var");
    BOOST_CHECK_EQUAL(test_var, 1);


    env
        .RunAction("increment test_var")
        .RunAction("increment test_var")
        .RunAction("increment test_var");

    BOOST_CHECK_EQUAL(test_var, 4);

    BOOST_CHECK_THROW(env.RunAction("none"), environment_error);

    env.RegisterAction("I will throw", [&](){ throw std::runtime_error("lol"); });
    BOOST_CHECK_THROW(env.RunAction("I will throw"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RegTrigger)
{
    Environment env {};
    auto lambda  = [](int x) -> int { return x; };

    BOOST_CHECK_NO_THROW(env.RegisterTrigger("trigger", lambda));
    InterfaceRecord(lambda).InjectArgs({42});
    Environment::InterfaceHandle ifc_rec {"trigger"};
    BOOST_CHECK_NO_THROW(ifc_rec.RunAsTrigger());
    BOOST_CHECK_EQUAL(InterfaceRecord(lambda).ObservedReturn(), 42);

    // registering same entity shall not throw
    BOOST_CHECK_NO_THROW(env.RegisterTrigger("trigger", lambda));
}



BOOST_AUTO_TEST_CASE(RegInterface)
{
    Environment env {};
    auto lambda  = [](int x) -> int { return x; };

    BOOST_CHECK_NO_THROW(env.RegisterInterface("lambda", lambda));

    // registering same entity shall not throw
    BOOST_CHECK_NO_THROW(env.RegisterInterface("lambda", lambda));

    // nullptr obj id registered in the first call implicitely
    BOOST_CHECK_NO_THROW(env.RegisterInterface("lambda", lambda, nullptr));

    // obj+ifc identity already assigned to another key
    BOOST_CHECK_THROW(env.RegisterInterface("other key", lambda, nullptr), environment_error);

    // key already exist
    BOOST_CHECK_THROW(env.RegisterInterface("lambda", lambda, object_id("some obj")), environment_error);
}
