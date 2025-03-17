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
};

BOOST_FIXTURE_TEST_CASE(GenericTest, ModelTestFixture)
{
    SignalMapping("Ducktyping example: use enum nominal values")
        .OnTrigger (DuckQuack)
        .InjectTo  (DuckQuack)
        .ObserveOn (DuckQuack)
    .Test
        ( ducks::Grey ::Quack , ducks::Grey ::Poop   )
        ( ducks::Grey ::Poop  , ducks::Grey ::Quack  )
        ( ducks::White::Quack , ducks::White::Poop   )
        ( ducks::White::Poop  , ducks::White::Quack  )
        (              "Quack",              "Poop"  )
        (              "Poop" ,              "Quack" )
    ;

}


template <class Duck>
struct Decorator
{
    using decorated_type = Duck;
    decorated_type value_;

    // decorated type transformations

    explicit Decorator(decorated_type const& v) : value_{v} {}

    operator decorated_type() const {
        return value_;
    }

    // serialization

    explicit Decorator(boost::json::value const& v)
    : value_{v.is_string()
        ? zmbt::dejsonize<decorated_type>(v)
        : decorated_type(boost::json::value_to<int>(v))
    } {}

    operator boost::json::value() const {
        return zmbt::json_from(value_);
    }
};

BOOST_FIXTURE_TEST_CASE(DecoratorTest, ModelTestFixture)
{
    SignalMapping("Ducktyping example: decorator for testing underlying value")
    .OnTrigger(DuckQuack)
        .InjectTo  (DuckQuack).As(type<Decorator<ducks::Grey>>)
        .ObserveOn (DuckQuack).As(type<Decorator<ducks::Grey>>)
    .Test
        (0,   1)
        (1,   0)
        (2, 255)
    ;
}
