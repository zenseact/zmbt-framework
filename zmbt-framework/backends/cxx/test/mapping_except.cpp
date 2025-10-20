/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/application.hpp"
#include "zmbt/mapping.hpp"
#include "zmbt/model.hpp"


using namespace zmbt::mapping;
using namespace zmbt::reflect;

using namespace zmbt;
using namespace zmbt::expr;
using L = boost::json::array;


BOOST_AUTO_TEST_CASE(CaptureTriggerException)
{
    auto const sut = []{
        throw std::out_of_range("lol");
    };

    SignalMapping("Capture exception from trigger")
    .OnTrigger(sut).Repeat(42)
        .At(sut).Exception().Take(Map(At("/type"))).Expect(Each("std::out_of_range"))
        .At(sut).Exception().Take(Map(At("/what"))).Expect(Each("lol"));
}


BOOST_AUTO_TEST_CASE(CaptureTriggerBadException)
{
    auto const sut = []{
        throw "lol";
    };

    SignalMapping("Capture exception from trigger")
    .OnTrigger(sut)
        .At(sut).Exception().Take(At("/type")).Expect("unknown")
        .At(sut).Exception().Take(At("/what")).Expect("unknown");
}


BOOST_AUTO_TEST_CASE(CaptureUnhandledException)
{
    boost::json::value error(nullptr);
    zmbt::Config()
        .SetFailureHandler([&error](boost::json::value const& sts){ error = sts; });

    auto const sut = []{
        throw "lol";
    };

    SignalMapping("Fail on unexpected exception")
    .OnTrigger(sut)
        .At(sut).Expect(Noop);

    BOOST_CHECK_NE(error, nullptr);
    BOOST_TEST_MESSAGE(error);
    BOOST_CHECK_EQUAL(error.at("verdict"), "ERROR") ;
}


BOOST_AUTO_TEST_CASE(InjectMockException)
{
    struct Mock
    {
        void foo()
        {
            return zmbt::InterfaceRecord(&Mock::foo).Hook();
        }
    };

    auto const sut = []{
        Mock().foo();
    };

    SignalMapping("Mapping execution test: type default")
    .OnTrigger(sut)
        .At(&Mock::foo).Exception().Inject(expr::Error(type<std::out_of_range>,"lol"))
        .At(sut).Exception().Expect(At("type") | "std::out_of_range")
        .At(sut).Exception().Expect(At("what") | "lol");
}
