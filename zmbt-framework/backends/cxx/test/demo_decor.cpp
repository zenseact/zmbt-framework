/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt::expr;

using zmbt::decor::Underlying;
using zmbt::decor::Precise;
using zmbt::decor::precise;




BOOST_AUTO_TEST_CASE(DecorPreciseFloatToFloat)
{

    auto const IdentityTest = [](float x) { return x; };

    SignalMapping("Test comparison assertions on IdentityFunctor")
    .OnTrigger(IdentityTest)
        .At(IdentityTest).As(Precise<float>).Inject()
        .At(IdentityTest).As(Precise<float>).Expect()
    .Test
        (0           , "0x0p+0")
        (  1.f       , 1.f        )

        (  0.f       , 0.f        )
        (  1.f       , 1.f        )
        (  1.1f      , 1.1f       )
        (  0         , 0          )
        (  1         , 1          )
        (  "0"       , "0"        )
        (  "1"       , "1"        )
        ( "-1"       , "-1"       )
        (  "0.25"    , "0.25"     )
        (  "0x1.4p3" , "0x1.4p3"  )
    ;
}

BOOST_AUTO_TEST_CASE(DecorPreciseFloatToDouble)
{

    auto const FloatToDouble = [](float x) -> double { return x; };

    SignalMapping("Test comparison assertions on FloatToDouble")
    .OnTrigger(FloatToDouble)
        .At(FloatToDouble) .As(Precise<float>) .Inject()
        .At(FloatToDouble) .As(Precise<float>) .Expect()
    // .Description("Literal vectors")
    .Test
        // will fail: "1.1f" is parsed py strtod, not strtof
        // ("1.1f"          , Expect::EQ("1.1f")     )
        // correct: hexadecimal notation is safe regardless of type
        (1.1f            , "0x1.19999ap+0"   )

        // (0.f             , 0.f               )
        // (1.f             , 1.f               )
        ("0x1.19999ap+0" , "0x1.19999ap+0"   ) // 1.1f
        ("0x1.555556p-2" , "0x1.555556p-2"   ) // 1.f / 3

        (1.f           , Eq(1.f )            )
        (1.f           , Le(1.5f)            )
        (1.f           , Ge( .5f)            )

        (1.f           , Near({1.1f, 0.2f})   )
        (1             , In({1.1f, 0.2f, 1E0}))
        (1             , In({1.1f, 0.2f, 1})  )

        (  0           , 0                   )
        (  1           , 1                   )
        (  "0"         , "0"                 )
        (  "1"         , "1"                 )
        ( "-1"         , "-1"                )
        (  "0.25"      , "0.25"              )
        (  "0x1.4p3"   , "0x1.4p3"           )

        (1.5f          , Near({1.f, 0.5})    )
        (1.5f          , Near({2.f, 0.5})    )
        (1.5f          , Near({1  , 0.5})    )
        (1.5f          , Near({2  , 0.5})    )
        (1.f           , Near({"1.f", 0.1f}) )
        (1.5f          , Near({"1.f", 0.5}) )
        (1.5f          , Near({"2.f", 0.5}) )
    ;
}

BOOST_AUTO_TEST_CASE(DecorPreciseDoubleToFloat)
{

    auto const DoubleToFloat = [](double x) -> float { return x; };

    SignalMapping("Test comparison assertions on FloatToDouble")
    .OnTrigger(DoubleToFloat)
        .At(DoubleToFloat) .As(Precise<double>) .Inject()
        .At(DoubleToFloat) .As(Precise<float>)  .Expect()
    // .Description("Literal vectors")
    .Test
        (0                     , 0               )
        (1                     , 1               )
        ("1.f"                 , "1.f"           )
        ("0x1.19999ap+0"       , "0x1.19999ap+0" )
        ("0x1.555556p-2"       , "0x1.555556p-2" )
        (1.1                   , "0x1.19999ap+0" )
        ("0x1.199999999999ap+0", "0x1.19999ap+0" )
    ;
}
