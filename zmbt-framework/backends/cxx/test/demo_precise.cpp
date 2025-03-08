/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;

using zmbt::decor::precise;
using zmbt::decor::precision_loss_error;

BOOST_AUTO_TEST_CASE(Constructor)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    BOOST_CHECK_THROW(precise<float>("")   ,  std::invalid_argument);
    BOOST_CHECK_THROW(precise<float>("--1"),  std::invalid_argument);
    BOOST_CHECK_THROW(precise<float>("lol"),  std::invalid_argument);

    BOOST_CHECK_EQUAL(precise<float>(  1.f    ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>(   .5    ).value()  ,   .5    );
    BOOST_CHECK_EQUAL(precise<float>( - .125  ).value()  , - .125  );
    BOOST_CHECK_EQUAL(precise<float>(  1.2e-6f).value()  ,  1.2e-6f);
    BOOST_CHECK_EQUAL(precise<float>(  1.     ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>(  1      ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>( -1.f    ).value()  , -1      );
    BOOST_CHECK_EQUAL(precise<float>( -1.     ).value()  , -1      );
    BOOST_CHECK_EQUAL(precise<float>( -1      ).value()  , -1      );
    BOOST_CHECK_EQUAL(precise<float>(  1U     ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>(  1UL    ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>("1.f"    ).value()  ,  1      );
    BOOST_CHECK_EQUAL(precise<float>(".5"     ).value()  ,   .5    );
    BOOST_CHECK_EQUAL(precise<float>("-.125"  ).value()  , - .125  );
    BOOST_CHECK_EQUAL(precise<float>("1.2e-6f").value()  ,  1.2e-6f);

    BOOST_CHECK_EQUAL(precise<float>("0x1.19999ap+0"      ).stringify(), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL(precise<float>("0x1.19999ap+0f"     ).stringify(), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL(precise<float>("0x1.19999a000000p+0").stringify(), "0x1.19999ap+0");

    BOOST_CHECK_EQUAL(precise<float>("NAN"      ).stringify(),  "nan");
    BOOST_CHECK_EQUAL(precise<float>("INF"      ).stringify(),  "inf");
    BOOST_CHECK_EQUAL(precise<float>("INFINITY" ).stringify(),  "inf");
    BOOST_CHECK_EQUAL(precise<float>("-INF"     ).stringify(), "-inf");
    BOOST_CHECK_EQUAL(precise<float>("-INFINITY").stringify(), "-inf");

    BOOST_CHECK_EQUAL(precise<float>(precise<double>("0x1.19999ap+0")).stringify(), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL(precise<double>(precise<float>("0x1.19999ap+0")).stringify(), "0x1.19999ap+0");
}

BOOST_AUTO_TEST_CASE(Assignment)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    precise<float> a;

    BOOST_CHECK_THROW((a = ""   ),  std::invalid_argument);
    BOOST_CHECK_THROW((a = "--1"),  std::invalid_argument);
    BOOST_CHECK_THROW((a = "lol"),  std::invalid_argument);

    BOOST_CHECK_EQUAL((a =  1.f     , a.value()),  1      );
    BOOST_CHECK_EQUAL((a =   .5     , a.value()),   .5    );
    BOOST_CHECK_EQUAL((a = - .125   , a.value()), - .125  );
    BOOST_CHECK_EQUAL((a =  1.2e-6f , a.value()),  1.2e-6f);
    BOOST_CHECK_EQUAL((a =  1.      , a.value()),  1      );
    BOOST_CHECK_EQUAL((a =  1       , a.value()),  1      );
    BOOST_CHECK_EQUAL((a = -1.f     , a.value()), -1      );
    BOOST_CHECK_EQUAL((a = -1.      , a.value()), -1      );
    BOOST_CHECK_EQUAL((a = -1       , a.value()), -1      );
    BOOST_CHECK_EQUAL((a =  1U      , a.value()),  1      );
    BOOST_CHECK_EQUAL((a =  1UL     , a.value()),  1      );
    BOOST_CHECK_EQUAL((a = "1.f"    , a.value()),  1      );
    BOOST_CHECK_EQUAL((a = ".5"     , a.value()),   .5    );
    BOOST_CHECK_EQUAL((a = "-.125"  , a.value()), - .125  );
    BOOST_CHECK_EQUAL((a = "1.2e-6f", a.value()),  1.2e-6f);

    BOOST_CHECK_EQUAL((a = "0x1.19999ap+0"      , a.stringify()), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL((a = "0x1.19999ap+0f"     , a.stringify()), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL((a = "0x1.19999a000000p+0", a.stringify()), "0x1.19999ap+0");

    BOOST_CHECK_EQUAL((a = "NAN"      , a.stringify()),  "nan");
    BOOST_CHECK_EQUAL((a = "INF"      , a.stringify()),  "inf");
    BOOST_CHECK_EQUAL((a = "INFINITY" , a.stringify()),  "inf");
    BOOST_CHECK_EQUAL((a = "-INF"     , a.stringify()), "-inf");
    BOOST_CHECK_EQUAL((a = "-INFINITY", a.stringify()), "-inf");

    BOOST_CHECK_EQUAL((a = precise<double>("0x1.19999ap+0"), a.stringify()), "0x1.19999ap+0");
    BOOST_CHECK_EQUAL((a = precise<float>("0x1.19999ap+0") , a.stringify()), "0x1.19999ap+0");
}

BOOST_AUTO_TEST_CASE(PrecisionLossError)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    // trivial integers
    BOOST_CHECK_NO_THROW(precise<float>("0" ));
    BOOST_CHECK_NO_THROW(precise<float>("1" ));
    BOOST_CHECK_NO_THROW(precise<float>("-1"));

    BOOST_CHECK_NO_THROW(precise<double>("0"));
    BOOST_CHECK_NO_THROW(precise<double>("1"));
    BOOST_CHECK_NO_THROW(precise<double>("-1"));


    // powers of 2: no precision loss
    BOOST_CHECK_NO_THROW(precise<float>(".125"));
    BOOST_CHECK_NO_THROW(precise<float>("-.25"));
    BOOST_CHECK_NO_THROW(precise<float>(".125d"));
    BOOST_CHECK_NO_THROW(precise<float>("-.25D"));

    BOOST_CHECK_NO_THROW(precise<float>(.125));
    BOOST_CHECK_NO_THROW(precise<float>(-.25));
    BOOST_CHECK_NO_THROW(precise<float>(.125));
    BOOST_CHECK_NO_THROW(precise<float>(-.25));

    BOOST_CHECK_NO_THROW(precise<double>(".125"));
    BOOST_CHECK_NO_THROW(precise<double>("-.25"));
    BOOST_CHECK_NO_THROW(precise<double>(".125D"));
    BOOST_CHECK_NO_THROW(precise<double>("-.25d"));
    BOOST_CHECK_NO_THROW(precise<double>(".125f"));
    BOOST_CHECK_NO_THROW(precise<double>("-.25F"));

    BOOST_CHECK_NO_THROW(precise<double>(.125));
    BOOST_CHECK_NO_THROW(precise<double>(-.25));
    BOOST_CHECK_NO_THROW(precise<double>(.125));
    BOOST_CHECK_NO_THROW(precise<double>(-.25));
    BOOST_CHECK_NO_THROW(precise<double>(.125f));
    BOOST_CHECK_NO_THROW(precise<double>(-.25F));


    BOOST_CHECK_NO_THROW(precise<float>(".1f"));
    BOOST_CHECK_NO_THROW(precise<float>(".1F"));
    BOOST_CHECK_NO_THROW(precise<float>(.1f));
    BOOST_CHECK_NO_THROW(precise<double>(".1d"));
    BOOST_CHECK_NO_THROW(precise<double>(".1D"));
    BOOST_CHECK_NO_THROW(precise<double>(.1));

    // rounding error with suffix-type mismatch, .1f != .1d
    BOOST_CHECK_THROW(precise<float>(".1d"),  precision_loss_error);
    BOOST_CHECK_THROW(precise<float>(".1D"),  precision_loss_error);
    BOOST_CHECK_THROW(precise<float>(".1" ),  precision_loss_error);
    BOOST_CHECK_THROW(precise<float>(".1" ),  precision_loss_error);
    BOOST_CHECK_THROW(precise<double>(".1f"), precision_loss_error);
    BOOST_CHECK_THROW(precise<double>(".1F"), precision_loss_error);

    // precision loss, initial value != value
    BOOST_CHECK_THROW(precise<float>(.1), precision_loss_error);
    // won't catch this error as we can't know the decimal form of the initial value
    BOOST_CHECK_NO_THROW(precise<double>(.1f));

    // hexadecimal floats
    BOOST_CHECK_NO_THROW(precise<float>("0x1.19999ap+0"));
    BOOST_CHECK_NO_THROW(precise<float>("0x1.19999a0000000p+0"));
    BOOST_CHECK_THROW(precise<float>("0x1.199999999999ap+0")   , precision_loss_error);

    BOOST_CHECK_NO_THROW(precise<double>("0x1.19999ap+0"));
    BOOST_CHECK_NO_THROW(precise<double>("0x1.19999ap+0f"));
    BOOST_CHECK_NO_THROW(precise<double>("0x1.199999999999ap+0"));
}

namespace {
    struct Point { double x, y; };
    BOOST_DESCRIBE_STRUCT(Point, (void), (x, y))
}


BOOST_AUTO_TEST_CASE(FloatToFloat)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    auto const IdentityTest = [](float x) { return x; };

    SignalMapping("Test comparison assertions on IdentityFunctor")
    .OnTrigger(IdentityTest)
        .InjectTo  (IdentityTest) .As(type<precise<float>>)
        .ObserveOn (IdentityTest) .As(type<precise<float>>)
    .Test
        (  0.f , 0.f   )
        (  1.f , 1.f   )
        (  1.1f, 1.1f  )
        (  0         , 0          )
        (  1         , 1          )
        (  "0"       , "0"        )
        (  "1"       , "1"        )
        ( "-1"       , "-1"       )
        (  "0.25"    , "0.25"     )
        (  "0x1.4p3" , "0x1.4p3"  )
    ;
}

BOOST_AUTO_TEST_CASE(FloatToDouble)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    auto const FloatToDouble = [](float x) -> double { return x; };

    SignalMapping("Test comparison assertions on FloatToDouble")
    .OnTrigger(FloatToDouble)
        .InjectTo  (FloatToDouble) .As(type<precise<float>>)
        .ObserveOn (FloatToDouble) .As(type<precise<double>>)
    // .Description("Literal vectors")
    .Test
        // will fail: "1.1f" is parsed py strtod, not strtof
        // ("1.1f"          , Expect::EQ("1.1f")     )
        // correct: hexadecimal notation is safe regardless of type
        (1.1f            , "0x1.19999ap+0"   )

        (0.f             , 0.f               )
        (1.f             , 1.f               )
        (1.f             , "1.f"             )
        ("0x1.19999ap+0" , "0x1.19999ap+0"   ) // 1.1f
        ("0x1.555556p-2" , "0x1.555556p-2"   ) // 1.f / 3

        (1.f           , Eq(1.f )            )
        (1.f           , Le(1.5f)            )
        (1.f           , Ge( .5f)            )

        (1.f           , Near({1.1f, 0.2})   )
        (1             , In({1.1f, 0.2, 1E0}))
        (1             , In({1.1f, 0.2, 1})  )

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
        (1.f           , Near({precise<double> ("1.f"), 0.1}) )
        (1.5f          , Near({precise<double> ("1.f"), 0.5}) )
        (1.5f          , Near({precise<double> ("2.f"), 0.5}) )
    ;
}

BOOST_AUTO_TEST_CASE(DoubleToFloat)
{
    BOOST_TEST_INFO("Tags: U-SMD U-PRC");

    auto const DoubleToFloat = [](double x) -> float { return x; };

    SignalMapping("Test comparison assertions on FloatToDouble")
    .OnTrigger(DoubleToFloat)
        .InjectTo  (DoubleToFloat) .As(type<precise<double>>)
        .ObserveOn (DoubleToFloat) .As(type<precise<float>>)
    // .Description("Literal vectors")
    .Test
        (0                     , 0               )
        (1                     , 1               )
        ("1.f"                 , "1.f"           )
        ("0x1.19999ap+0"       , "0x1.19999ap+0" ) // 1.1f
        ("0x1.555556p-2"       , "0x1.555556p-2" ) // 1.f / 3
        (1.1                   , "0x1.19999ap+0" )
        ("0x1.199999999999ap+0", "0x1.19999ap+0" )
    ;
}
