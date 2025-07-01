/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/decor.hpp"

using namespace zmbt::decor;

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

