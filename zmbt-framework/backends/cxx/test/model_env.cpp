/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
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


BOOST_AUTO_TEST_CASE(Hook)
{
    auto test_fun = [](int, int&&, int const&, int&){ return 0; };
    auto handle = InterfaceRecord(test_fun);

    auto producing_mock = [&](int v, int&& rv, int const& c, int& lv)
    {
        // Environment().DumpJsonData(std::cerr);
        return handle.Hook(v, rv, c, lv );
    };

    handle.InjectReturn(13);
    handle.InjectArgs({0, 0, 0, 42});

    int arg_out = -1;

    BOOST_CHECK_EQUAL(13, producing_mock(10, 20, 30, arg_out)); // Hook shall return what was set with InjectReturn
    BOOST_CHECK_EQUAL(42, arg_out); // Hook shall update output args
    BOOST_CHECK_EQUAL(1, handle.ObservedCalls());

    handle.InjectArgs({0,0,0,43});
    BOOST_CHECK_EQUAL(13, producing_mock(11, 21, 31, arg_out)); // Hook shall return what was set with InjectReturn
    BOOST_CHECK_EQUAL(43, arg_out); // Hook shall update output args
    BOOST_CHECK_EQUAL(2, handle.ObservedCalls());
    auto observed_args_1_call =  handle.ObservedArgs(-2);
    auto observed_args_2_call =  handle.ObservedArgs();

    BOOST_CHECK_EQUAL(10, observed_args_1_call[0]); // Hook shall capture values
    BOOST_CHECK_EQUAL(20, observed_args_1_call[1]); // Hook shall capture rvalue refs
    BOOST_CHECK_EQUAL(30, observed_args_1_call[2]); // Hook shall capture const refs
    BOOST_CHECK_EQUAL(-1, observed_args_1_call[3]); // Hook shall capture non-const ref inputs
    BOOST_CHECK_EQUAL(11, observed_args_2_call[0]); // Hook shall capture values
    BOOST_CHECK_EQUAL(21, observed_args_2_call[1]); // Hook shall capture rvalue refs
    BOOST_CHECK_EQUAL(31, observed_args_2_call[2]); // Hook shall capture const refs
    BOOST_CHECK_EQUAL(42, observed_args_2_call[3]); // Hook shall capture non-const ref inputs
}



BOOST_AUTO_TEST_CASE(InterfaceHandleLambda)
{
    auto test_fun = [](int, int&&, int const&, int&){ return 0; };
    auto handle = InterfaceRecord(test_fun);

    handle.InjectReturn(42);
    BOOST_CHECK_EQUAL(42, handle.YieldInjectionReturn());

    handle.InjectArgs({1,2,3,4});
    auto args = handle.YieldInjectionArgs().as_array();

    BOOST_CHECK_EQUAL(1, args[0].as_int64());
    BOOST_CHECK_EQUAL(2, args[1].as_int64());
    BOOST_CHECK_EQUAL(3, args[2].as_int64());
    BOOST_CHECK_EQUAL(4, args[3].as_int64());

    handle.InjectArgs({10, 20, 30, 40});
    args = handle.YieldInjectionArgs().as_array();

    BOOST_CHECK_EQUAL(10, args[0].as_int64());
    BOOST_CHECK_EQUAL(20, args[1].as_int64());
    BOOST_CHECK_EQUAL(30, args[2].as_int64());
    BOOST_CHECK_EQUAL(40, args[3].as_int64());
}

BOOST_AUTO_TEST_CASE(InterfaceHandleMemfun)
{

}

BOOST_AUTO_TEST_CASE(InterfaceHandleMemdata)
{

}

BOOST_AUTO_TEST_CASE(InterfaceHandleFreefun)
{

}

BOOST_AUTO_TEST_CASE(InterfaceHandleVoid)
{
    auto test_fun = [](){};
    auto handle = InterfaceRecord(test_fun);

    BOOST_CHECK_NO_THROW(handle.YieldInjectionReturn());
    BOOST_CHECK_NO_THROW(handle.InjectArgs(boost::json::array{}));

    BOOST_CHECK(0 == handle.YieldInjectionArgs().as_array().size());
    BOOST_CHECK_NO_THROW(handle.Hook());
    BOOST_CHECK(1 == handle.ObservedCalls());
}
