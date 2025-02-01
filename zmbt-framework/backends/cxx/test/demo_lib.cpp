/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt;


BOOST_AUTO_TEST_CASE(SimpleTest)
{
    auto sum = [](int x, int y){ return x + y; };

    SignalMapping("Simple function test")
    .OnTrigger(sum)
        .InjectTo  (sum) .Args(0)
        .InjectTo  (sum) .Args(1)
        .ObserveOn (sum) .Return()
    .Test
        (2,  2, 4)
        (2, -2, 0)
    .Description("inject x and y and check returning sum")
    ;
}


BOOST_AUTO_TEST_CASE(TestOrderWithMerge)
{
    struct Mock {
        void foo(std::string k) {
            return InterfaceRecord(&Mock::foo).Hook(k);
        }
        void bar(std::string k) {
            return InterfaceRecord(&Mock::bar).Hook(k);
        }
    };

    auto test = [](std::vector<std::string> calls){
        Mock mock{};
        for (auto const& call: calls)
        {
            if (call == "foo") mock.foo(call);
            else if (call == "bar") mock.bar(call);
        }
    };

    SignalMapping("Test order with MergeInSeries channel combo")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).Alias("f")
            .MergeInSeries()
        .ObserveOn (&Mock::bar).Alias("b")

    .Test
        ({"bar", "foo"}, Saturate(At(0)|"b", At(0)|"f"))
        ({"foo", "bar"}, Saturate(At(0)|"f", At(0)|"b"))
    ;
}


BOOST_AUTO_TEST_CASE(TestJoinWithAutoArgs)
{
    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    };

    auto test = [](int x, int y){
        Mock mock{};
        mock.foo(x);
        mock.bar(y);
    };

    SignalMapping("Test join with auto expr match on pair")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).Alias("f")
            .Join()
        .ObserveOn (&Mock::bar).Alias("b")

    .Test
        ({42,13}, {42, 13})
        ({ 2, 1}, Gt      ) ["2 > 1"]
        ({ 1, 2}, Lt      ) ["1 < 2"]
    ;
}


BOOST_AUTO_TEST_CASE(RegexTest)
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("Regex test")
    .OnTrigger(id)
        .InjectTo  (id)
        .ObserveOn (id)
    .Test
        ("123", Re("^.{3}$")   ) ["ECMAScript regex"]
        (42U  , Re("42")       ) ["match serialized value"]
        ({1,2}, Re("\\[1,2\\]")) ["match serialized json array"]
    ;
}


BOOST_AUTO_TEST_CASE(MockExample)
{
    struct Mock {
        bool set_values(int& x, int& y) {
            // ZMBT mock api
            return InterfaceRecord(&Mock::set_values).Hook(x, y);
        }
    };

    struct Sut {
        Mock mock {};
        int operator()() {
            int x, y;
            bool ok = mock.set_values(x, y);
            return ok ? (x + y) : 0;
        }
    } sut {};

    SignalMapping("Test with mock")
    .OnTrigger(sut)
        .InjectTo  (&Mock::set_values) .Return()
        .InjectTo  (&Mock::set_values) .Args(0)
        .InjectTo  (&Mock::set_values) .Args(1)
        .ObserveOn (sut)               .Return()
    .Test
        (true , 2,  2, 4)
        (true , 2, -2, 0)
        (false, 2,  2, 0)["mock returns false, values discarded"]
    ;
}


BOOST_AUTO_TEST_CASE(MultipleMocksExample)
{
    struct Mock {
        void set_value(int& x) const {
            return InterfaceRecord(this, &Mock::set_value).Hook(x);
        }
    };

    struct Sut {
        Mock const& mock_x;
        Mock const& mock_y;

        Sut(Mock const& mx, Mock const& my) : mock_x{mx}, mock_y{my} {}

        int operator()() {
            int x, y;
            mock_x.set_value(x);
            mock_y.set_value(y);
            return x + y;
        }
    };

    Mock mock_x {};
    Mock mock_y {};
    Sut sut {mock_x, mock_y};

    SignalMapping("Test with multiple mocks")
    .OnTrigger(sut)
        .InjectTo  (mock_x, &Mock::set_value) .Args()
        .InjectTo  (mock_y, &Mock::set_value) .Args()
        .ObserveOn (sut)
    .Test
        ( 2,  2, 4)
        ( 2, -2, 0)
    ;
}




BOOST_FIXTURE_TEST_CASE(ZipParametrization, ModelTestFixture)
{
    struct Mock {
        void log(std::string msg) {
            return InterfaceRecord(&Mock::log).Hook(msg);
        }
    };

    struct Sut {
        Mock mock {};

        int foo(int)    { mock.log("Sut::foo"); return 42; };
        int bar(double) { mock.log("Sut::bar"); return 42; };
        int baz()       { mock.log("Sut::baz"); return 42; };
    };

    auto sut = std::make_shared<Sut>();

    Param const Ifc  {"test interface"};
    Param const Name {"interface name"};

    SignalMapping("Test with zip params on %s", Name)
    .OnTrigger(sut, Ifc)
        .InjectTo  (sut, Ifc)
        .ObserveOn (sut, Ifc)
        .ObserveOn (&Mock::log)
    .Test
        (Noop, 42, Name)
        (Noop, 42, Name)
        (Noop, 42, Name)
    .Zip
        (Ifc , &Sut::foo , &Sut::bar , &Sut::baz )
        (Name, "Sut::foo", "Sut::bar", "Sut::baz")
    .Description("Ifc: %s, Name: %s", Ifc, Name)
    ;
}


BOOST_AUTO_TEST_CASE(ZipParametrizationWithRegistry)
{
    struct Mock {
        void log(std::string msg) {
            return InterfaceRecord(&Mock::log).Hook(msg);
        }
    };

    struct Sut {
        Mock mock {};

        int foo(int)    { mock.log("Sut::foo"); return 42; };
        int bar(double) { mock.log("Sut::bar"); return 42; };
        int baz()       { mock.log("Sut::baz"); return 42; };
    };

    auto sut = std::make_shared<Sut>();

    Environment env{};
    env.RegisterTrigger(sut, &Sut::foo, "Sut::foo");
    env.RegisterTrigger(sut, &Sut::bar, "Sut::bar");
    env.RegisterTrigger(sut, &Sut::baz, "Sut::baz");
    env.RegisterInterface(&Mock::log, "Mock::log");

    Param const Ifc  {"test interface"};
    Param const Name {"interface name"};

    SignalMapping("Test with zip params")
    .OnTrigger(Ifc)
        .InjectTo  (Ifc)
        .ObserveOn (Ifc)
        .ObserveOn ("Mock::log")
    .Test
        (Noop, 42, Ifc)
        (Noop, 42, Ifc)
        (Noop, 42, Ifc)
    .Zip
        (Ifc, "Sut::foo", "Sut::bar", "Sut::baz")
    ;
}


BOOST_AUTO_TEST_CASE(NonScalarValues)
{
    auto sum = [](int x, int y){ return x + y; };

    SignalMapping("Simple test with non-scalar channel")
    .OnTrigger(sum)
        .InjectTo  (sum)
        .ObserveOn (sum)
    .Test
        ({2,  2}, 4)
        ({2, -2}, 0)
    ;
}


BOOST_FIXTURE_TEST_CASE(MatchExpressions, ModelTestFixture)
{
    auto sum = [](int x, int y){ return x + y; };

    SetFailureHandler([](auto report){
        zmbt::pretty_print(std::cerr, report);
        BOOST_FAIL("ZMBT_FAIL");
    });

    SignalMapping("Test with match expressions")
    .OnTrigger(sum)
        .InjectTo  (sum) .Args(0)
        .InjectTo  (sum) .Args(1)
        .ObserveOn (sum) .Return()
    .Test
        (2,  2, 4                )
        (2,  2, Eq(4)            ) ["same as above"]
        (2,  2, Ne(5)            ) ["not equals 5"]
        (2,  2, Not(Eq(5))       ) ["same as above"]
        (2,  2, Gt(1)            ) ["greater than 1"]
        (2,  2, And(Ge(0), Le(5))) ["between 0 and 5"]
        (2,  2, In({1,2,3,4})    ) ["is element of {1,2,3,4} set"]
        (2,  2, Near(3, .5)      ) ["approximation with relative tolerance"]
        (2,  2, Not(Nil)       ) ["bool(signal) resolves to true"]
    ;
}


BOOST_AUTO_TEST_CASE(SetMatchExpression)
{
    auto sut = [](std::vector<int> x){ return x; };

    SignalMapping("Test set expressions")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (sut)
    .Test
        ( {1,2,3}   , Eq({1,2,3})                   )
        ( {1,1,1}   , Size|3                        )
        ( {1,1,1}   , Card|1                        )
        ( {1,2,3}   , SetEq({1,1,2,2,3,3})          )
        ( {1,2,3}   , Subset({1,2,3})               )
        ( {1,2,3}   , Superset({1,2,3})             )
        ( {1,2,3}   , ProperSubset({1,2,3,4})       )
        ( {1,2,3}   , ProperSuperset({1,3})         )
        ( {1,2,3}   , Not(ProperSubset({1,2,3}))    )
        ( {1,2,3}   , Not(ProperSuperset({1,2,3}))  )
        ( {1,2,3}   , Contains(1)                   )
        ( {1,2,3}   , Not(Ni(42))                   )
        ( {1,2,3}   , Saturate(Ne(5), 2, 3)         )
        ( {1,2,3}   , Count(Ne(5))|3                )
        ( {1,2,3}   , Count(2)|Lt(3)                )
        ( {1,2,3}   , And(At(0)|Eq(1), At("/1")|Eq(2))    )
        ( {1,2,3}   , Re("^\\[1,2,3\\]$")           )
    ;
}

BOOST_AUTO_TEST_CASE(DeepParamExpression)
{
    auto sut = [](boost::json::value const& x){ return x; };

    Param const input {0};
    Param const at_ptr {1};
    Param const expect {2};


    SignalMapping("Test parametrized expressions")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (sut)
    .Test
        (input, At(at_ptr)|expect)
    .Zip
        (input , 42)
        (at_ptr, "")
        (expect, 42)
    .Zip
        (input , {"lol", 42})
        (at_ptr, 0    , 1 )
        (expect, "lol", 42)
    .Zip
        (input, {
            {"lol", 13},
            {"kek", {1,2,42}},
        })
        (at_ptr, "/lol", "/kek/2")
        (expect, 13    ,  42     )
    ;
}

BOOST_AUTO_TEST_CASE(DeepSetMatch)
{
    auto sut = [](boost::json::value const& x){ return x; };

    boost::json::object const supersetSample
    {
        {"foo", {
            {"bar", {1,2,3,1,2,3}},
            {"baz", nullptr}
        }}
    };

    boost::json::object const subsetSample
    {
        {"foo", {
            {"bar", {1,2,2,2}}
        }}
    };

    SignalMapping("Test set expressions on deep structures")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (sut)
    .Test
        (supersetSample, Superset(subsetSample)    )
        (subsetSample  , Subset(supersetSample)    )
        (subsetSample  , Not(SetEq(supersetSample)))
    ;
}


BOOST_AUTO_TEST_CASE(SignalTypeDecorator)
{
    auto DoubleToFloat = [](double x) -> float { return x; };

    SignalMapping("Narrowing conversion (double) -> float")
    .OnTrigger(DoubleToFloat)
        .InjectTo  (DoubleToFloat) .As(type<precise<double>>)
        .ObserveOn (DoubleToFloat) .As(type<precise<float>> )

    .Test
        (0.125                 , 0.125           ) ["ok: power of 2"]
        (.2                    , .2f             ) ["ok: correct repr of float"]
        // (.2                    , .2              ) ["fail from precise<float> ctor"]
        ("0x1.199999999999ap+0", "0x1.19999ap+0" ) ["ok with float literals"]
    ;
}


// SERIALIZATION DEMO

namespace {
enum class Foo { A, B, C };
struct Bar {
    Foo foo;
    int x;

    Bar() = delete;
    Bar(Foo br, int x) : foo{br}, x{x} {}
};

// define serialization with Boost Describe lib
BOOST_DESCRIBE_ENUM(Foo, A, B, C)
BOOST_DESCRIBE_STRUCT(Bar, (), (foo, x))
ZMBT_INJECT_JSON_TAG_INVOKE
}

// default initialization for classes with no default ctor
ZMBT_DEFINE_CUSTOM_INIT(Bar, (Foo::A, 0))

BOOST_AUTO_TEST_CASE(UserDataTypes)
{
    auto sut = [](Bar a, Bar b) {
        Foo foo = a.x > b.x ? a.foo : b.foo;
        int x = a.x + b.x;
        return Bar {foo, x};
    };

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .InjectTo (sut).Args("/0/x")
        .InjectTo (sut).Args("/1/x")
        .InjectTo (sut).Args("/0/foo")
        .InjectTo (sut).Args("/1/foo")
        .ObserveOn(sut).Return("/foo")
    .Test
        (1, 0, Foo::A, Foo::B, Foo::A)
        (0, 1, Foo::A, Foo::B, Foo::B)
    ;
}


BOOST_AUTO_TEST_CASE(FixtureTasks)
{
    struct Mock {
        int get_value() {
            return InterfaceRecord(&Mock::get_value).Hook();
        }
    };

    auto sut = []() { return Mock().get_value(); };

    SignalMapping("Test pre- and post-run tasks")
    .OnTrigger(sut)
        .InjectTo (&Mock::get_value)
        .ObserveOn(sut)
    .Test
        (13    , 13)
        (Noop  , 42)
    .PreRun([]{
        InterfaceRecord(&Mock::get_value).InjectReturn(42);
    })
    .PostRun([]{
        BOOST_CHECK_EQUAL(InterfaceRecord(&Mock::get_value).ObservedCalls(), 1);
    })
    ;
}


// ORDER OF CALLS AND CALL COUNT


BOOST_AUTO_TEST_CASE(CallCountAndOnCallsRange)
{
    struct Mock {
        Bar do_smth(int x, int y) {
            return InterfaceRecord(&Mock::do_smth).Hook(x, y);
        }
    };

    auto sut = [](int count) {
        Mock mock{};
        int sum = 0;
        while (--count, count >=0) sum += mock.do_smth(count, 42).x;
        return sum;
    };

    SignalMapping("Test CallCount")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).CallCount()
    .Test
        ( 0, Nil)
        (42, 42 )
    ;


    SignalMapping("Test CallRange")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).CallRange()
    .Test
        (  0, Nil                       )
        ( 42, Not(Nil)                  )
        ( 42, Bool                      )
        ( 42, Size| 42                  )
        ( 42, Size| And(Gt(41), Lt(43)) )
        ( 42, Size| Ne(13)              )
    ;

    SignalMapping("Test Timestamp and ThreadId")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).ThreadId().CallRange().Alias("tid")
        .ObserveOn (&Mock::do_smth).Timestamp().CallRange().Alias("ts")
    .Test
        ( 42, Count(Not(zmbt::get_tid()))| 0, Count(Le(zmbt::get_ts()))| 0 )
    ;

    SignalMapping("Test CallRange with arg index")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).Args(0).CallRange() // all arguments at 0 (x)
        .ObserveOn (&Mock::do_smth).Args(1).CallRange() // all arguments at 1 (y)
    .Test
        ( 3, {2,1,0}               , Ni(42)      ) ["exact match on all captured calls"]
        ( 5, {4,3,2,1,0}           , Size| Ge(3) ) ["exact match on all captured calls"]
        ( 5, Saturate(Eq(4), 2, Ge(0))        , _) ["elementwise saturation match"]
        ( 5, Not(Saturate(1, 1, 1))           , _) ["saturation negation"]
        ( 5, And(Saturate(3,2), Saturate(3,1)), _) ["DAG test"]
        ( 5, Count(Ne(3))|Eq(4)               , _) ["count not 3 matches"]
        ( 5, At(0)|4                          , _)
        ( 5, Not(At("/foo")|0)                , _)
    ;


    SignalMapping("Test CallRange first and last dropped")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).Args(0).CallRange(2, -2) // all arguments at 0 (x)
    .Test
        ( 3, SetEq({1}))
        ( 5, {3,2,1} )
    ;


    SignalMapping("Test CallRange reverse")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).Args(0).CallRange(-1, 1, -1)
    .Test
        ( 3, {0,1,2}    )
        ( 5, {0,1,2,3,4})
    ;

    Param const range {"call range"};

    SignalMapping("Test CallRange parametric")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .ObserveOn (&Mock::do_smth).Args(0).CallRange(range)
    .Test
        ( 3, Size|Ge(0))
        ( 5, Size|Ge(0))
    .Zip
        (range, {-1, 1, -1}, {1, 42, 1})
    ;

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .InjectTo (sut)
        .ObserveOn(&Mock::do_smth).Args(0).Call(1)
        .ObserveOn(&Mock::do_smth).Args(1).Call(-1)
    .Test
        ( 5,  4, 42)
        (42, 41, 42)
    ;

    SignalMapping("Test CallRange inject")
    .OnTrigger(sut)
        .InjectTo  (sut)
        .InjectTo (&Mock::do_smth).Return("/x").CallRange()
        .ObserveOn (sut)
    .Test
        ( 4, { 1,-1, 1,-1}        ,  0 )
        ( 5, { 3, 2, 1   }        ,  6 ) ["x=5 at first 3 calls, other calls x=0 (default from prototype)"]
        ( 5, {{"-1", 5}, {"2", 4}}, 24 ) ["x=2 at 2nd call, x=2 at default"]
    ;
}


BOOST_AUTO_TEST_CASE(Call)
{
    struct Mock {
        int get_value() {
            return InterfaceRecord(&Mock::get_value).Hook();
        }
    };

    auto sut = []() {
        Mock mock{};
        int x = mock.get_value();
        int y = mock.get_value();
        return x + y;
    };

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .InjectTo (&Mock::get_value).Call(1)
        .InjectTo (&Mock::get_value).Call(2)
        .ObserveOn(sut)
    .Test
        (2,  2, 4)
        (2, -2, 0)
    ;
}

