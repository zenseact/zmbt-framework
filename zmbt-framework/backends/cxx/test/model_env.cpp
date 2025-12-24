/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"

using namespace zmbt;


BOOST_AUTO_TEST_CASE(TypedVariables)
{
    Environment env {};


    BOOST_CHECK_THROW(env.GetVar<int>("number"), zmbt::environment_error);

    env.SetVar("number", 42);
    BOOST_CHECK(42 == env.GetVar<int>("number"));

    BOOST_CHECK(42 == env.GetVarOrUpdate<int>("unset", 42));
    BOOST_CHECK(42 == env.GetVar<int>("unset"));

    env.SetVar("lol", "kek");
    BOOST_CHECK("kek" == env.GetVar<std::string>("lol"));
}

BOOST_AUTO_TEST_CASE(JsonVariables)
{
    Environment env {};

    BOOST_CHECK(env.GetVarOrDefault("test 1").is_null());

    BOOST_CHECK("kek" == env.GetVarOrUpdate("test 2", { {"lol", "kek"} }).as_object()["lol"]);
}


BOOST_AUTO_TEST_CASE(SharedData)
{
    Environment env {};

    auto shared_data = std::make_shared<int>(13);
    env.SetShared("key", shared_data);
    auto retrieved_data = env.GetShared<int>("key");

    BOOST_ASSERT(retrieved_data != nullptr);
    BOOST_CHECK(*shared_data == *retrieved_data);

    // retrieving the data with incorrect type
    BOOST_CHECK_THROW(env.GetShared<float>("key"), std::runtime_error);

    BOOST_CHECK(nullptr == env.GetShared<int>("unset"));
}

namespace
{
    double test_function(int, std::string) { return 0; }

    void empty() { }
    void empty_void(void) { }
}


BOOST_AUTO_TEST_CASE(SetGetReturn)
{
    Environment env {};
    InterfaceRecord(test_function).InjectReturn(13.5);
    BOOST_CHECK_EQUAL(13.5, InterfaceRecord(test_function).YieldInjectionReturn());
}

BOOST_AUTO_TEST_CASE(SetGetArgs)
{
    Environment env {};

    #define A(...) (boost::json::array{__VA_ARGS__})

    // args as tuple
    InterfaceRecord(test_function).InjectArgs({13, "kek"});
    BOOST_CHECK_EQUAL(A(13, "kek"), InterfaceRecord(test_function).YieldInjectionArgs());

    // args as parameter pack
    InterfaceRecord(test_function).InjectArgs({42, "lol"});
    BOOST_CHECK_EQUAL(A(42, "lol"), InterfaceRecord(test_function).YieldInjectionArgs());


    InterfaceRecord(empty).InjectArgs(boost::json::array{});
    BOOST_CHECK_EQUAL(0, InterfaceRecord(empty).YieldInjectionArgs().as_array().size());

    InterfaceRecord(empty_void).InjectArgs(boost::json::array{});
    BOOST_CHECK_EQUAL(0, InterfaceRecord(empty_void).YieldInjectionArgs().as_array().size());
}


BOOST_AUTO_TEST_CASE(Lifetime)
{
    {
        zmbt::Environment env {};
        env.SetVar("lol", 42);
        BOOST_CHECK_EQUAL(42, env.GetVar("lol"));
    }
    {
        zmbt::Environment env2 {};
        BOOST_CHECK_THROW(env2.GetVar("lol"), zmbt::environment_error);
    }

    zmbt::Environment env  {};

    {
        env.SetVar("lol", 13);
        BOOST_CHECK_EQUAL(13, env.GetVar("lol"));

        {
            zmbt::Environment env2  {};
            BOOST_CHECK_EQUAL(13, env2.GetVar("lol"));
        }
    }
}
