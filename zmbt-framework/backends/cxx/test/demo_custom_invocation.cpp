/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt::expr;
using namespace zmbt;

struct Foo
{
    std::string str;

    Foo() = default;
    Foo (std::string s) : str{s} {}

    void bar(std::string& x) const
    {
        x = str;
    }
};


template <>
struct zmbt::reflect::custom_invocation<decltype(&Foo::bar)>
{
    using type     = decltype(&Foo::bar);
    using args_t   = std::tuple<int&>;
    using host_t  = typename zmbt::reflect::default_invocation<type>::host_t;
    using return_t  = typename zmbt::reflect::default_invocation<type>::return_t;


    static return_t apply(host_t object, type ifc, args_t args)
    {
        using orig_t = std::tuple<host_t, std::string&>;

        std::string str_out;
        boost::mp11::tuple_apply(std::mem_fn(ifc), orig_t{object, str_out});
        std::get<0>(args) = boost::json::value_to<int>(boost::json::parse(str_out));
    }

    static return_t apply(std::shared_ptr<std::remove_reference_t<host_t>> object, type ifc, args_t args)
    {
        return apply(*object.get(), ifc, args);
    }

    static return_t apply(std::shared_ptr<remove_cvref_t<host_t>> object, type ifc, args_t args)
    {
        return apply(*object.get(), ifc, args);
    }
};

BOOST_AUTO_TEST_CASE(CustomInvocationApply)
{
    using invocation = zmbt::reflect::invocation<decltype(&Foo::bar)>;
    {
        Foo const foo {"42"};
        int x;
        invocation::apply(foo, &Foo::bar, invocation::args_t{x});
        BOOST_TEST(x == 42);
    }

    {
        auto const foo = std::make_shared<Foo>("42");
        int x;
        invocation::apply(foo, &Foo::bar, invocation::args_t{x});
        BOOST_TEST(x == 42);
    }
}

BOOST_AUTO_TEST_CASE(CustomInvocationInModel)
{
    Foo const foo {"42"};

    SignalMapping("SignalMapping test")
    .OnTrigger(foo, &Foo::bar)
        .ObserveOn (&Foo::bar).Args(0).As(type<int>)
    .Test(42)
    ;
}

