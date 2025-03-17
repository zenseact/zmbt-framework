/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/mapping.hpp"
#include "zmbt/model.hpp"


using namespace zmbt::mapping;
using namespace zmbt::reflect;

using namespace zmbt;
using namespace zmbt::expr;

namespace
{
    int foo(int x) {
        return x;
    }

    void bar(int) { }
}

static_assert(
    zmbt::reflect::has_invocation_for<decltype(&foo)>::value,
    "This should never happen!"
);

BOOST_AUTO_TEST_CASE(TestModelInitCompilation)
{
    auto model = SignalMapping("SignalMapping init compilation test");
    model.OnTrigger(foo);
}

BOOST_AUTO_TEST_CASE(FunctionTrigger, *boost::unit_test::disabled())
{
    SignalMapping("SignalMapping on function: compilation test with channels")
    .OnTrigger(foo)

    .InjectTo  (foo)
    .InjectTo  (foo).Args(0)
    .InjectTo  (foo).Args("")
    .InjectTo  (foo).Args("%s-%s", Param(1), 42)
    .InjectTo  (foo).Return("")
    .InjectTo  (foo).Return("%s", Param(2))
    .InjectTo  (foo).Return()
    .InjectTo  (foo).Call(-1)
    .InjectTo  (foo).Args()
    .InjectTo  (foo).As(type<int>)

    .InjectTo  (nullptr, foo)
    .InjectTo  (nullptr, foo).Args(0)
    .InjectTo  (nullptr, foo).Args("")
    .InjectTo  (nullptr, foo).Args("%s", Param(3))
    .InjectTo  (nullptr, foo).Return("")
    .InjectTo  (nullptr, foo).Return("%s", Param(4))
    .InjectTo  (nullptr, foo).Return()
    .InjectTo  (nullptr, foo).Call(-1)
    .InjectTo  (nullptr, foo).Args()
    .InjectTo  (nullptr, foo).As(type<int>)
    .Test(_)
    ;

}

BOOST_AUTO_TEST_CASE(FunctorTrigger, *boost::unit_test::disabled())
{
    auto fctor = [&](int) -> int { return 0; };
    using fctor_t = decltype(fctor);

    static_assert(has_invocation_for<fctor_t&>::value, "");

    SignalMapping("SignalMapping on functor: compilation test")
    .OnTrigger(fctor)

    .InjectTo  (fctor)
    .InjectTo  (fctor).Args(0)
    .InjectTo  (fctor).Args("")
    .InjectTo  (fctor).As(type<int>)
    .InjectTo  (fctor).Return("")
    .InjectTo  (fctor).Return()
    .InjectTo  (fctor).Call(-1)
    .InjectTo  (fctor).Args()

    .InjectTo  (nullptr, fctor)
    .InjectTo  (nullptr, fctor).Args(0)
    .InjectTo  (nullptr, fctor).Args("")
    .InjectTo  (nullptr, fctor).As(type<int>)
    .InjectTo  (nullptr, fctor).Return("")
    .InjectTo  (nullptr, fctor).Return()
    .InjectTo  (nullptr, fctor).Call(-1)
    .InjectTo  (nullptr, fctor).Args()
    .Test(_);
}

BOOST_AUTO_TEST_CASE(ModelExecutor)
{
    SignalMapping("Mapping execution test: type default")
    .OnTrigger (foo)
        .InjectTo  (foo)
        .ObserveOn (foo)
    .Test
        (42,     42)
        (13, Ne(14))
        (13, All(Gt(12),Lt(14)))
    ;
}

BOOST_AUTO_TEST_CASE(ModelExecutorDecorated)
{
    SignalMapping("Mapping execution test: type<int>")
    .OnTrigger (foo)
        .InjectTo  (foo).As(type<int>)
        .ObserveOn (foo).As(type<int>)
    .Test
        ( 0,  0)
        (-1, -1)
        (42, 42)
    ;
}

BOOST_AUTO_TEST_CASE(ModelExecutorReturnVoid)
{
    SignalMapping("Mapping execution test: return void")
    .OnTrigger (bar)
        .InjectTo  (bar)
        .ObserveOn (bar)
    .Test
        (11, Noop)
    ;
}

BOOST_AUTO_TEST_CASE(TestVectorInitList)
{
    auto Identity = [](boost::json::value const& v) -> boost::json::value { return v; };

    boost::json::object obj {
        {"lol", 42},
        {"kek", 13}
    };

    SignalMapping("Mapping execution test: init list")
    .OnTrigger (Identity)
        .InjectTo  (Identity)
        .ObserveOn (Identity)
    .Test
        (42   , 42   )
        ("lol", "lol")
        (obj  , obj  )
        ({"lol", 42}  , {"lol", 42})
        ({{"lol", 42}}  , {{"lol", 42}}  )
    ;
}


namespace
{

struct Base
{
    int test_field;
    int another_field;

    Base() : test_field{0}, another_field{0} {}
    Base(int x) : test_field{x}, another_field{x} {}

    virtual ~Base() = default;
    Base(Base const&) = default;
    Base(Base &) = default;
    Base& operator=(Base const&) = default;
    Base& operator=(Base &) = default;

    virtual std::string foo() const
    {
        return { "Base" };
    }
};

struct Final final : public virtual Base
{
    using Base::Base;
    using Base::operator=;
    virtual ~Final() = default;

    virtual std::string foo() const final
    {
        return { "Final" };
    }
};

ZMBT_INJECT_JSON_TAG_INVOKE

BOOST_DESCRIBE_STRUCT(Base, (void), (test_field, another_field))
BOOST_DESCRIBE_STRUCT(Final, (Base), ((Final&(Final const&))operator=))

} // namespace s


BOOST_AUTO_TEST_CASE(Invocation)
{

    Base const drv {13};
    Final const fin {13};

    auto ifc = &Base::foo;
    using ifc_t = decltype(ifc);
    using reflection = zmbt::reflect::invocation<ifc_t>;

    auto drv_ret = reflection::apply(drv, ifc, {});
    auto fin_ret = reflection::apply(fin, ifc, {});

    BOOST_TEST(drv_ret == "Base");
    BOOST_TEST(fin_ret == "Final");
}

BOOST_AUTO_TEST_CASE(Serialization)
{

    BOOST_TEST(json_from(Base {  }) == json_from(Final {  }));
    BOOST_TEST(json_from(Base {13}) == json_from(Final {13}));
}

BOOST_AUTO_TEST_CASE(VirtualMethod)
{

    Base base;
    Final final;

    SignalMapping("Test virtual method on a parent")
    .OnTrigger(base, &Base::foo)
        .ObserveOn (base, &Base::foo)
    .Test("Base")
    ;


    SignalMapping("Test virtual method on a subclass")
    .OnTrigger(final, &Base::foo)
        .ObserveOn (final, &Base::foo)
    .Test("Final")
    ;
}

BOOST_AUTO_TEST_CASE(VirtualMethodDefaultRefobj)
{

    Base base;
    Final fin;

    SignalMapping("Test virtual method on a parent")
        .OnTrigger (base, &Base::foo)
        .ObserveOn (&Base::foo)
    .Test("Base")
    ;

    SignalMapping("Test virtual method on a subclass")
        .OnTrigger (fin, &Base::foo)
        .ObserveOn (&Base::foo)
    .Test("Final")
    ;
}

BOOST_AUTO_TEST_CASE(SubclassSignalSlicing)
{

    std::function<std::string(Base const& sig)> const& callTestMethod {
        [](Base const& sig) -> std::string {
            return sig.foo();
        }
    };

    SignalMapping("Object slicing on default serialization")
    .OnTrigger(callTestMethod)
        .InjectTo (callTestMethod)
        .ObserveOn(callTestMethod)
    .Test
        (Base  {}, "Base")
        (Final {}, "Base")
    .Description("Virtual inheritance on signal types is not supported by default")
    ;
}

BOOST_AUTO_TEST_CASE(SubclassSignalData)
{
    BOOST_TEST_MESSAGE("Tags: U-GEN S-INHR");

    std::function<Base (Base const& sig)> const identity {
        [](Base const& sig) -> Base { return sig; }
    };

    SignalMapping("Test inherited data on a subclass")
    .OnTrigger(identity)
        .InjectTo  (identity)
        .ObserveOn (identity).Return("/test_field")
    .Test
        (Base  {  },  0)
        (Base  {13}, 13)
        (Final {  },  0)
        (Final {42}, 42)
    ;
}


BOOST_AUTO_TEST_CASE(ZipModel)
{
    BOOST_TEST_MESSAGE("Tags: U-GEN S-INHR");

    auto identity = [](Base const& sig) -> Base { return sig; };

    auto FieldParam = Param(1);

    SignalMapping("Test inherited data on a subclass")
    .OnTrigger(identity)
        .InjectTo  (identity).Args("/%d/%s", 0, FieldParam)
        .ObserveOn (identity).Return("/%s", FieldParam)
    .Test
        ( 0,  0)
        (13, 13)
        ( 0,  0)
        (42, 42)
    .Zip
        (FieldParam, "test_field", "another_field")
    ;
}



BOOST_AUTO_TEST_CASE(LongZipModel)
{

    auto sut = []() -> std::vector<std::pair<int, int>> {
        std::vector<std::pair<int, int>> test_vector(16);
        std::fill(test_vector.begin(), test_vector.end(), std::make_pair(42, 43));
        return test_vector;
    };

    auto IndexA = Param(1);
    auto IndexB = Param(2);

    SignalMapping("Test long zip wrapping")
    .OnTrigger(sut)
        .ObserveOn (sut).Return("/%d/0", IndexA)
        .ObserveOn (sut).Return("/%d/1", IndexB)
    .Test
        ( 42,  43)
    .Zip
        (IndexA,  0,  1,  2,  3)
        (IndexB,  0,  1,  2,  3)

        (IndexA,  4,  5,  6,  7)
        (IndexB,  4,  5,  6,  7)

        (IndexA,  8,  9, 10, 11)
        (IndexB,  8,  9, 10, 11)

        (IndexA, 12, 13, 14, 15)
        (IndexB, 12, 13, 14, 15)
    ;
}


BOOST_AUTO_TEST_CASE(ZipContinuation)
{
    auto sut = [](int x, int y, int c) { return (x+y)*c; };

    auto X = Param(1);
    auto Y = Param(2);
    auto C = Param(3);
    auto Ret = Param(4);

    SignalMapping("Test inherited data on a subclass")
    .OnTrigger(sut)
        .InjectTo  (sut).Args(0)
        .InjectTo  (sut).Args(1)
        .InjectTo  (sut).Args(2)
        .ObserveOn (sut).Return()
    .Test
        (X, Y, C, Ret)

    .Zip
        (C   , 1) // continued param, equivalent to (C, 1, 1, 1)
        (X   , 2, 3, 4)
        (Y   , 2, 3, 3)
        (Ret , 4, 6, 7)

    .Zip
        (C   , -1) // continued param
        (X   ,  2,  3,  4)
        (Y   ,  2,  3,  3)
        (Ret , -4, -6, -7)
    ;
}


BOOST_AUTO_TEST_CASE(ZipProdBrackets)
{

    auto identity = [](boost::json::value const& x) { return x; };

    auto X = Param(1);

    SignalMapping("Test Zip/Prod clauses compiletaion with json bracket init")
    .OnTrigger(identity)
        .InjectTo  (identity)
        .ObserveOn (identity)
    .Test
        (X, X)
    .Zip
        (X, {},{},{},{},{42},{""},{{}},{{"",42}}, Base{})
    .Prod
        (X, {},{},{},{},{42},{""},{{}},{{"",42}}, Base{})
    .Zip
        (X, Base{}, Base{}, Base{}, Base{})
    .Prod
        (X, Base{}, Base{}, Base{}, Base{})
    ;
}

struct TestObject
{
    void in_out(int const& in, int& out) const
    {
        return InterfaceRecord(&TestObject::in_out).Hook(in, out);
    }

    int get_value() const
    {
        return InterfaceRecord(&TestObject::get_value).Hook();
    }
};


BOOST_AUTO_TEST_CASE(ModelWithMocks)
{
    auto SUT = [](int in) {
        int out;
        TestObject().in_out(in, out);
        return out;
    };

    auto FieldParam = Param(1);


    SignalMapping("Test side effect mocking")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .InjectTo  (&TestObject::in_out).Args("/1")
        .ObserveOn (&TestObject::in_out).Args("/0")
        .ObserveOn (SUT)
    .Test
        ( _,  _,  _,  _)
        ( 1,  _,  1,  _)
        (42,  _, 42,  _)
        ( _,  1,  _,  1)
        ( _, 42,  _, 42)
        ( _, 42,  _, 42)
    ;
}


BOOST_AUTO_TEST_CASE(OnCallInject)
{
    struct Mock
    {
        static bool break_loop()
        {
            return InterfaceRecord(&Mock::break_loop).Hook();
        }
    };

    auto SUT = []()
    {
        int count = 10;
        while (--count) {
            if (Mock::break_loop()) break;
        }
    };

    auto const T = true;
    SignalMapping("Test Call Inject")
    .OnTrigger(SUT)
        .InjectTo  (&Mock::break_loop).Return().Call(4)
        .InjectTo  (&Mock::break_loop).Return().Call(5)
        .ObserveOn (&Mock::break_loop).CallCount()
    .Test
        ( T    , false, 4 )
        ( false, T    , 5 )
    ;
}


BOOST_AUTO_TEST_CASE(OnCallObserve)
{
    struct Mock
    {
        static void consume_value(int x)
        {
            return InterfaceRecord(&Mock::consume_value).Hook(x);
        }
    };

    auto SUT = [](int start)
    {
        Mock::consume_value(++start);
        Mock::consume_value(++start);
        Mock::consume_value(++start);
    };

    SignalMapping("Test Call Observe")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .ObserveOn (&Mock::consume_value).Args().Call(1)
        .ObserveOn (&Mock::consume_value).Args().Call(2)
        .ObserveOn (&Mock::consume_value).Args().Call(3)
    .Test
        (  0,  1,  2,  3 )
        ( 42, 43, 44, 45 )
    ;
}


BOOST_AUTO_TEST_CASE(OnCallParametric)
{
    auto SUT = []() -> std::vector<int> {
        TestObject mock{};
        return std::vector<int> {
            mock.get_value(),
            mock.get_value(),
            mock.get_value()
        };
    };


    auto NofCall = Param(1);
    auto OutIndex = Param(2);

    SignalMapping("Test parametric Call")
    .OnTrigger(SUT)
        .ObserveOn (&TestObject::get_value) .CallCount()
        .InjectTo  (&TestObject::get_value) .Call(NofCall)
        .ObserveOn (SUT)                    .Return("/%d", OutIndex)
        .ObserveOn (SUT)                    .Return()
    .Test
        ( 3,  0,   0, Noop            )
        ( 3, -1,  -1, Superset({0, -1}) )
        ( 3, 42,  42, Superset({0, 42}) )
    .Zip
        (NofCall , 1, 2, 3)
        (OutIndex, 0, 1, 2)
    ;
}


BOOST_AUTO_TEST_CASE(ZipRegisteredInterfaces)
{
    int sut1_calls {0};
    int sut2_calls {0};
    int sut3_calls {0};
    auto sut1 = [&](int x){ sut1_calls++; return x; };
    auto sut2 = [&](int x){ sut2_calls++; return x; };
    auto sut3 = [&](int x){ sut3_calls++; return x; };

    Environment env {};

    env .RegisterTrigger(sut1, "sut-1")
        .RegisterTrigger(sut2, "sut-2")
        .RegisterTrigger(sut3, "sut-3");

    auto SUT = Param(1);

    SignalMapping("Test interface zipping")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .ObserveOn (SUT)
    .Test
        (  0,   0)
        ( -1,  -1)
        ( 42,  42)
    .Zip
        (SUT, "sut-1", "sut-2", "sut-3")
    ;

    BOOST_CHECK_EQUAL(sut1_calls, 3);
    BOOST_CHECK_EQUAL(sut2_calls, 3);
    BOOST_CHECK_EQUAL(sut3_calls, 3);
}


BOOST_AUTO_TEST_CASE(ZipInterfaceFunctorLiterals)
{

    int sut1_calls {0};
    int sut2_calls {0};
    int sut3_calls {0};
    auto sut1 = [&](int x){ sut1_calls++; return x; };
    auto sut2 = [&](int x){ sut2_calls++; return x; };
    auto sut3 = [&](int x){ sut3_calls++; return x; };

    auto SUT = Param(1);

    SignalMapping("Test interface zipping")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .ObserveOn (SUT)
    .Test
        (  0,   0)
        ( -1,  -1)
        ( 42,  42)
    .Zip
        (SUT, sut1, sut2, sut3)
    ;

    BOOST_CHECK_EQUAL(sut1_calls, 3);
    BOOST_CHECK_EQUAL(sut2_calls, 3);
    BOOST_CHECK_EQUAL(sut3_calls, 3);
}

BOOST_AUTO_TEST_CASE(ZipInterfaceFreeFnLiterals)
{
    auto SUT = Param(1);

    SignalMapping("Test interface zipping over free functions")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .ObserveOn (SUT)
    .Test(_, _)
    .Zip
        (SUT, foo, bar)
    ;
}


struct O
{
    int foo_calls {0};
    int bar_calls {0};
    int baz_calls {0};

    int foo(int)   { foo_calls++; return 42; };
    int bar(double){ bar_calls++; return 42; };
    int baz()      { baz_calls++; return 42; };
};


BOOST_AUTO_TEST_CASE(ZipInterfaceMfpLiteralsInvariantSut)
{

    auto SUT = std::make_shared<O>();
    auto IFC = Param(1);

    SignalMapping("Test interface zipping")
    .OnTrigger(SUT, IFC)
        .InjectTo  (SUT, IFC)
        .ObserveOn (SUT, IFC)
    .Test
        ( Noop,  42)
        ( Noop,  42)
        ( Noop,  42)
    .Zip
        (IFC, &O::foo, &O::bar, &O::baz)
    ;

    BOOST_CHECK_EQUAL(SUT->foo_calls, 3);
    BOOST_CHECK_EQUAL(SUT->bar_calls, 3);
    BOOST_CHECK_EQUAL(SUT->baz_calls, 3);
}


struct Mock
{
    int foo() {
        return InterfaceRecord(&Mock::foo).Hook();
    }

    int bar() {
        return InterfaceRecord(&Mock::bar).Hook();
    }
};

struct Consumer
{
    int foo()
    {
        Mock mock{};
        return mock.foo() + mock.bar();
    }
};


BOOST_AUTO_TEST_CASE(ZipInterfaceMfpLiteralsMock)
{

    auto SUT = std::make_shared<Consumer>();
    auto IFC = Param(1);

    SignalMapping("Test interface zipping")
    .OnTrigger(SUT, &Consumer::foo)
        .InjectTo  (IFC).Return()
        .ObserveOn (SUT, &Consumer::foo)
    .Test
        ( 42, 42 )
        ( 13, 13 )
        (  1,  1 )
    .Zip
        (IFC, &Mock::foo, &Mock::bar)
    ;
}




BOOST_AUTO_TEST_CASE(ZipInterfaceMfpLiterals)
{

    auto Sut1 = std::make_shared<O>();
    auto Sut2 = std::make_shared<O>();
    auto Sut3 = std::make_shared<O>();

    auto SUT = Param(1);
    auto IFC = Param(2);


    SignalMapping("Test interface zipping")
    .OnTrigger(SUT, IFC)
        .InjectTo  (SUT, IFC)
        .ObserveOn (SUT, IFC)
    .Test
        ( Noop,  42)
        ( Noop,  42)
        ( Noop,  42)
    .Zip
        (SUT, Sut1   , Sut2   , Sut3   )
        (IFC, &O::foo, &O::bar, &O::baz)
    ;

    BOOST_CHECK_EQUAL(Sut1->foo_calls, 3);
    BOOST_CHECK_EQUAL(Sut2->bar_calls, 3);
    BOOST_CHECK_EQUAL(Sut3->baz_calls, 3);

    BOOST_CHECK_EQUAL(Sut1->bar_calls, 0);
    BOOST_CHECK_EQUAL(Sut1->baz_calls, 0);

    BOOST_CHECK_EQUAL(Sut2->foo_calls, 0);
    BOOST_CHECK_EQUAL(Sut2->baz_calls, 0);

    BOOST_CHECK_EQUAL(Sut3->foo_calls, 0);
    BOOST_CHECK_EQUAL(Sut3->bar_calls, 0);

}

BOOST_AUTO_TEST_CASE(ZipInterfaceMfpLiteralsUnsafe)
{
    struct O
    {
        int foo_calls {0};
        int bar_calls {0};
        int baz_calls {0};

        int foo(int)   { foo_calls++; return 42; };
        int bar(double){ bar_calls++; return 42; };
        int baz()      { baz_calls++; return 42; };
    };

    auto const Sut1 = O();
    auto const Sut2 = O();
    auto const Sut3 = O();

    auto SUT = Param(1);
    auto IFC = Param(2);


    SignalMapping("Test interface zipping")
    .OnTrigger(SUT, IFC)
        .InjectTo  (SUT, IFC)
        .ObserveOn (SUT, IFC)
    .Test
        ( Noop,  42)
        ( Noop,  42)
        ( Noop,  42)
    .Zip
        (SUT, Sut1   , Sut2   , Sut3   )
        (IFC, &O::foo, &O::bar, &O::baz)
    ;

    BOOST_CHECK_EQUAL(Sut1.foo_calls, 3);
    BOOST_CHECK_EQUAL(Sut2.bar_calls, 3);
    BOOST_CHECK_EQUAL(Sut3.baz_calls, 3);

    BOOST_CHECK_EQUAL(Sut1.bar_calls, 0);
    BOOST_CHECK_EQUAL(Sut1.baz_calls, 0);

    BOOST_CHECK_EQUAL(Sut2.foo_calls, 0);
    BOOST_CHECK_EQUAL(Sut2.baz_calls, 0);

    BOOST_CHECK_EQUAL(Sut3.foo_calls, 0);
    BOOST_CHECK_EQUAL(Sut3.bar_calls, 0);

}



BOOST_AUTO_TEST_CASE(ZipWithExpressionParams)
{

    auto sut1 = [](int x){ return x+1; };
    auto sut2 = [](int x){ return x+2; };
    auto sut3 = [](int x){ return x+3; };

    auto SUT = Param(1);
    auto ARG = Param(2);
    auto RET = Param(3);

    SignalMapping("Test ifc + expr zip")
    .OnTrigger(SUT)
        .InjectTo  (SUT)
        .ObserveOn (SUT)
    .Test
        (ARG, RET)
    .Zip
        (SUT, sut1, sut2, sut3)
        (ARG,   10,   10,   10)
        (RET,   11,   12,   13)

        (SUT, sut1, sut2, sut3)
        (ARG,   20,   20,   20)
        (RET,   21,   22,   23)
    ;
}

BOOST_AUTO_TEST_CASE(ZipWithDeferredReferences)
{
    auto sut1 = [&](){ return 1; };
    auto sut2 = [&](){ return 2; };
    auto sut3 = [&](){ return 3; };

    Environment env {};
    env .RegisterTrigger(sut1, "sut-1")
        .RegisterTrigger(sut2, "sut-2")
        .RegisterTrigger(sut3, "sut-3");

    Param const N {"N"};

    SignalMapping("Test interface zipping")
    .OnTrigger("sut-%d", N)
    .ObserveOn("sut-%d", N)
    .Test(N)
    .Zip(N, 1, 2, 3)
    ;
}


BOOST_AUTO_TEST_CASE(ProdModel)
{
    BOOST_TEST_MESSAGE("Tags: U-GEN S-INHR");

    int call_cnt {0};
    auto sut = [&](Base const& sig, int cf) -> int {
        ++call_cnt;
        return sig.test_field * cf + sig.another_field * cf;
    };

    Param const FieldParam {1};
    Param const CoefParam {2};

    SignalMapping("Test prod")
    .OnTrigger(sut)
        .InjectTo  (sut).Args("/0/%s", FieldParam)
        .InjectTo  (sut).Args("/1")
        .ObserveOn (sut)
    .Test
        ( 0, CoefParam,  0)
        (13, CoefParam, 13)
        (42, CoefParam, 42)
    .Prod
        (FieldParam, "test_field", "another_field")
        (CoefParam, 1)
    .Prod
        (FieldParam, "test_field", "another_field")
        (CoefParam, 1)
    ;

    BOOST_CHECK(call_cnt == 12); // 3 tests * (2*1 + 2*1) params
}
