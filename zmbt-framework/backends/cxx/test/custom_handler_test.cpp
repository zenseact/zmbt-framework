/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/application.hpp"
#include "zmbt/mapping.hpp"
#include "zmbt/model.hpp"


using namespace zmbt;
using namespace zmbt::expr;




BOOST_AUTO_TEST_CASE(BadSignalPath)
{
    boost::json::value error{};
    zmbt::Config()
        .SetFailureHandler([&error](boost::json::value const& sts){ error = sts; });

    auto const sut = [](int){return 42;};

    zmbt::mapping::SignalMapping("Invalid signal path")
    .OnTrigger(sut)
        .At(sut).Return("/foo/bar") .Expect(42)
    ;

    BOOST_CHECK_NE(error, nullptr);
    BOOST_CHECK_EQUAL(error.at("verdict"), "ERROR");
}

// Config() can't restore the handler from boost_main, instead it resets to default handler with exception.
// Do not add more tests here :)
