/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt::expr;

#if defined(__clang__)
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
    #pragma clang diagnostic ignored "-Wc++17-extensions"
#endif


namespace ducks {
    enum class Grey  {Quack = 0, Poop = 1};
    enum class White {Quack = 1, Poop = 2};

    BOOST_DESCRIBE_ENUM(Grey , Quack, Poop)
    BOOST_DESCRIBE_ENUM(White, Quack, Poop)

    ZMBT_INJECT_JSON_TAG_INVOKE
}


BOOST_AUTO_TEST_CASE(EnumNominalMatch)
{

    auto expr = Eq(ducks::Grey::Quack);
    BOOST_CHECK(expr.match(zmbt::json_from(ducks::Grey::Quack)));
    BOOST_CHECK(expr.match(zmbt::json_from(ducks::White::Quack)));

    expr = Ne(ducks::White::Quack);
    BOOST_CHECK(!expr.match(zmbt::json_from(ducks::Grey::Quack)));
    BOOST_CHECK(!expr.match(zmbt::json_from(ducks::White::Quack)));
}


ducks::Grey DuckQuack(ducks::Grey what)
{
    switch (what)
    {
    case ducks::Grey::Quack:
        return ducks::Grey::Poop;
    case ducks::Grey::Poop:
        return ducks::Grey::Quack;
    default:
        return ducks::Grey(255);
    }
}

BOOST_FIXTURE_TEST_CASE(GenericTest, ModelTestFixture)
{
    SignalMapping("Ducktyping example: use enum nominal values")
    .OnTrigger(DuckQuack)
        .At(DuckQuack).Inject()
        .At(DuckQuack).Expect()
    .Test
        ( ducks::Grey ::Quack , ducks::Grey ::Poop   )
        ( ducks::Grey ::Poop  , ducks::Grey ::Quack  )
        ( ducks::White::Quack , ducks::White::Poop   )
        ( ducks::White::Poop  , ducks::White::Quack  )
        (              "Quack",              "Poop"  )
        (              "Poop" ,              "Quack" )
    ;

}
