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



BOOST_AUTO_TEST_CASE(FunctionTrigger, *boost::unit_test::disabled())
{
    SignalMapping("SignalMapping on function: compilation test with channels")
    .OnTrigger(foo)

    .At(foo).Inject()
    .At(foo).Args(0).Inject()
    .At(foo).Args("").Inject()
    .At(foo).Args("%s-%s", Param(1), 42).Inject()
    .At(foo).Return("").Inject()
    .At(foo).Return("%s", Param(2)).Inject()
    .At(foo).Return().Inject()
    .At(foo).Args().Inject()
    .At(foo).As(type<int>).Inject()

    .At(foo, nullptr).Inject()
    .At(foo, nullptr).Args(0).Inject()
    .At(foo, nullptr).Args("").Inject()
    .At(foo, nullptr).Args("%s", Param(3)).Inject()
    .At(foo, nullptr).Return("").Inject()
    .At(foo, nullptr).Return("%s", Param(4)).Inject()
    .At(foo, nullptr).Return().Inject()
    .At(foo, nullptr).Args().Inject()
    .At(foo, nullptr).As(type<int>).Inject()
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

    .At(fctor).Inject()
    .At(fctor).Args(0).Inject()
    .At(fctor).Args("").Inject()
    .At(fctor).As(type<int>).Inject()
    .At(fctor).Return("").Inject()
    .At(fctor).Return().Inject()
    .At(fctor).Args().Inject()

    .At(fctor, nullptr).Inject()
    .At(fctor, nullptr).Args(0).Inject()
    .At(fctor, nullptr).Args("").Inject()
    .At(fctor, nullptr).As(type<int>).Inject()
    .At(fctor, nullptr).Return("").Inject()
    .At(fctor, nullptr).Return().Inject()
    .At(fctor, nullptr).Args().Inject()
    .Test(_);
}

BOOST_AUTO_TEST_CASE(ModelExecutor)
{
    SignalMapping("Mapping execution test: type default")
    .OnTrigger (foo)
        .At(foo).Inject()
        .At(foo).Expect()
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
        .At(foo).As(type<int>).Inject()
        .At(foo).As(type<int>).Expect()
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
        .At(bar).Inject()
        .At(bar).Expect()
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
        .At(Identity).Inject()
        .At(Identity).Expect()
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
    .OnTrigger(&Base::foo, base)
        .At(&Base::foo, base).Expect()
    .Test("Base")
    ;


    SignalMapping("Test virtual method on a subclass")
    .OnTrigger(&Base::foo, final)
        .At(&Base::foo, final).Expect()
    .Test("Final")
    ;
}

BOOST_AUTO_TEST_CASE(VirtualMethodDefaultRefobj)
{

    Base base;
    Final fin;

    SignalMapping("Test virtual method on a parent")
        .OnTrigger (&Base::foo, base)
        .At(&Base::foo).Expect()
    .Test("Base")
    ;

    SignalMapping("Test virtual method on a subclass")
        .OnTrigger (&Base::foo, fin)
        .At(&Base::foo).Expect()
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
        .At(callTestMethod).Inject()
        .At(callTestMethod).Expect()
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
        .At(identity).Inject()
        .At(identity).Return("/test_field").Expect()
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

    auto FieldParam = Param("lol");

    SignalMapping("Test inherited data on a subclass")
    .OnTrigger(identity)
        .At(identity).Args("/%d/%s", 0, FieldParam).Inject()
        .At(identity).Return("/%s", FieldParam).Expect()
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
        .At(sut).Return("/%d/0", IndexA).Expect()
        .At(sut).Return("/%d/1", IndexB).Expect()
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
        .At(sut).Args(0).Inject()
        .At(sut).Args(1).Inject()
        .At(sut).Args(2).Inject()
        .At(sut).Return().Expect()
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
        .At(identity).Inject()
        .At(identity).Expect()
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


    SignalMapping("Test side effect mocking")
    .OnTrigger(SUT)
        .At(SUT).Inject()
        .At(&TestObject::in_out).Args("/1").Inject()
        .At(&TestObject::in_out).Args("/0").Take(Last).Expect()
        .At(SUT).Expect()
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
    auto const F = false;
    SignalMapping("Test Call Inject")
    .OnTrigger(SUT)
        .At(&Mock::break_loop).Return().Inject()
        .At(&Mock::break_loop).CallCount().Expect()
    .Test
        ( Eq(3)|And(T)|Or(F), 4 )
        ( Lookup({F,F,F,T}) , 4 )
    ;
}


BOOST_AUTO_TEST_CASE(TakeClause)
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

    Param const Zero{"zero"};


    SignalMapping("Test Call Observe")
    .OnTrigger(SUT)
        .At(SUT).Take(At(0)).Inject()
        .At(&Mock::consume_value).Args().Take(At(Zero|Add(0))).Expect()
        .At(&Mock::consume_value).Args().Take(At(Zero|Add(1))).Expect()
        .At(&Mock::consume_value).Args().Take(At(Zero|Add(2))).Expect()
    .Test
        ( L{ 0},  1,  2,  3 )
        ( L{42}, 43, 44, 45 )
    .Zip
        (Zero, 0)
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
    SignalMapping("Parametric call # match")
    .OnTrigger(SUT)
        .At(&TestObject::get_value).Inject()
        .At(SUT).Return().Expect()
        .At(SUT).Return("/%d", NofCall).Expect()
    .Test
        ( Ne(NofCall)|And(13)|Or(0) , Superset({13,  0}),  0)
        ( Eq(NofCall)|And(-1)|Or(13), Superset({13, -1}), -1)
        ( Eq(NofCall)|And(42)|Or(13), Superset({13, 42}), 42)
    .Zip
        (NofCall, 0, 1, 2)
    ;
}

BOOST_AUTO_TEST_CASE(ExpressionClauses)
{
    auto identity = [](boost::json::value const& x){
        return x;
    };

    auto N = Param(1);

    SignalMapping("Parametric Repeat (%d)", N)
    .OnTrigger(identity).Repeat(N)
        .At(identity).Inject()
        .At(identity).Expect()
    .Test
        (Id               , N | Arange                    )
        (Flip(Sub(N))     , "%d:0:-1" | Fmt(N) | Arange   )
        (Recur(2, Pow(2)) , N | Sub(1) | Unfold(2, Pow(2)))
    .Zip
        (N, 1, 42);
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

    env .RegisterTrigger("sut-1", sut1)
        .RegisterTrigger("sut-2", sut2)
        .RegisterTrigger("sut-3", sut3);

    auto SUT = Param(1);

    SignalMapping("Test interface zipping")
    .OnTrigger(SUT)
        .At(SUT).Inject()
        .At(SUT).Expect()
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
        .At(SUT).Inject()
        .At(SUT).Expect()
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
        .At(SUT).Inject()
        .At(SUT).Expect()
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
    .OnTrigger(IFC, SUT)
        .At(IFC, SUT).Inject()
        .At(IFC, SUT).Expect()
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
    .OnTrigger(&Consumer::foo, SUT)
        .At(IFC).Return().Inject()
        .At(&Consumer::foo, SUT).Expect()
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
    .OnTrigger(IFC, SUT)
        .At(IFC, SUT).Inject()
        .At(IFC, SUT).Expect()
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
    .OnTrigger(IFC, SUT)
        .At(IFC, SUT).Inject()
        .At(IFC, SUT).Expect()
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
        .At(SUT).Inject()
        .At(SUT).Expect()
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
    env .RegisterTrigger("sut-1", sut1)
        .RegisterTrigger("sut-2", sut2)
        .RegisterTrigger("sut-3", sut3);

    Param const N {"N"};

    SignalMapping("Test interface zipping")
    .OnTrigger("sut-%d", N)
    .At("sut-%d", N).Expect()
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
        .At(sut).Args("/0/%s", FieldParam).Inject()
        .At(sut).Args("/1").Inject()
        .At(sut).Expect()
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

BOOST_AUTO_TEST_CASE(DeepParamExpression)
{
    auto sut = [](boost::json::value const& x){ return x; };

    Param const input {0};
    Param const at_ptr {1};
    Param const expect {2};


    SignalMapping("Test parametrized expressions")
    .OnTrigger(sut)
        .At(sut).Inject()
        .At(sut).Expect()
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


BOOST_AUTO_TEST_CASE(TestWith)
{
    auto test_pair = [](int x, int y){ return std::make_pair(x, y); };

    SignalMapping("And clause test")
    .OnTrigger(test_pair)

        .At(test_pair) .Args(0) .Inject()
        .At(test_pair) .Args(1) .Inject()

        .At(test_pair) .Return() .Expect()

        .At(test_pair) .Return(1) .Group()
        .At(test_pair) .Return(0) .Expect()

    .Test
        (  _ ,  _ ,  Size|2  ,  Size|2  )
        (  0 ,  0 , { 0,  0} , { 0,  0} )
        ( 42 , 13 , {42, 13} , {13, 42} )
        ( 42 , 13 , Gt       , Lt       )
    ;


    using test_map_t = std::map<int, int>;
    auto test_map = [](test_map_t const& map){
        return map;
    };

    SignalMapping("And clause test")
    .OnTrigger(test_map)

        .At(test_map) .Inject()
        .At(test_map) .Return("/0/1") .Group()
        .At(test_map) .Return("/1/1") .Expect()

    .Test
        (_             ,  _   )
        ({{1,2}, {3,4}}, {2,4})
    ;
}


BOOST_AUTO_TEST_CASE(TestUnion)
{
    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    };

    auto test = [](std::list<int> nums){
        Mock mock{};
        bool flip {true};
        while (!nums.empty())
        {
            int x = nums.front();
            nums.pop_front();
            flip ? mock.foo(x) : mock.bar(x);
            flip = !flip;
        }
    };


    SignalMapping("Test series with default clause")
    .OnTrigger(test)
        .At(test).Inject({1,2,3,4})

        .At(&Mock::foo).Alias("f").Blend()
        .At(&Mock::bar).Alias("b").Expect(L{
            {"f", 1},
            {"b", 2},
            {"f", 3},
            {"b", 4}
        })
    ;

    SignalMapping("Test series with range")
    .OnTrigger(test)
        .At(test).Inject()
        .At(&Mock::foo).Alias("f").Blend()
        .At(&Mock::bar).Alias("b").Expect()
    .Test
        ({1,2,3,4}, L{{"f", 1}, {"b", 2}, {"f", 3}, {"b", 4}})
        ({1,2,3,4}, Saturate({"f", 1}, {"b", 2}, {"f", 3}, {"b", 4}))
    ;

    SignalMapping("Test series with CallCount clause")
    .OnTrigger(test)
        .At(test).Inject({1,2,3,4,5,6,7})
        .At(&Mock::foo).CallCount().Alias("f").Blend()
        .At(&Mock::bar).CallCount().Alias("b")
            // CallCount does not have timestamp, so it is blended in order of definition
            .Expect(L{{"f", 4},{"b", 3}});
}


BOOST_AUTO_TEST_CASE(TestFixedChannels)
{
    auto sut = [](boost::json::value const& x){ return x; };

    SignalMapping("Test fixing clauses Inject and Expect")
    .OnTrigger(sut)

    .At(sut) .Args("/0")              .Inject({0,0,0})
    .At(sut) .Args("/0/%d", 0|Add(1)) .Inject(1)
    .At(sut) .Args("/0/%d", 0|Add(2)) .Inject(42)

    .At(sut) .Return("/1") .Expect(1)
    .At(sut) .Return("/2") .Expect(42)

    .At(sut) .Return("/2") .Group()
    .At(sut) .Return("/1") .Expect({42,1})

    .At(sut) .Return("/1") .Alias("a").Blend()
    .At(sut) .Return("/2") .Alias("b").Expect(
        L{
            {"a",  1},
            {"b", 42},
        })
    ;
}


BOOST_AUTO_TEST_CASE(TriggerReturnRef)
{

    int x = 42;
    auto const getx = [&x] () -> int& { return x; };

    SignalMapping("trigger return reference")
    .OnTrigger (getx)
        .At(getx).Return().Expect(42);
}


BOOST_AUTO_TEST_CASE(MockReturnRef)
{

    struct Mock
    {
        int&  x()
        {
            return InterfaceRecord(&Mock::x, this).Hook();
        }

        int const&  y()
        {
            return InterfaceRecord(&Mock::y, this).Hook();
        }
    } mock1, mock2;


    auto const SUT = [&] ()  { return mock1.x() + mock2.y(); };

    SignalMapping("mock return reference")
    .OnTrigger (SUT)
        .At(&Mock::x, &mock1).Inject()
        .At(&Mock::y, &mock2).Inject()
        .At(SUT).Return().Expect()
    .Test
        ( 2,  2,  4)
        ( 3,  3,  6)
        (42, -3, 39)
    ;
}



BOOST_AUTO_TEST_CASE(FlattenExpect)
{
    struct Mock
    {
        static void ping()
        {
            return InterfaceRecord(&Mock::ping).Hook();
        }
    };

    auto SUT = [](int& x)
    {
        Mock::ping();
        x = 42;
        return 13;
    };

    SignalMapping("Flatten Trigger arg when no repeat set ")
    .OnTrigger(SUT).At(SUT).Args(0).Expect(42);

    SignalMapping("Flatten Trigger return when no repeat set ")
    .OnTrigger(SUT).At(SUT).Expect(13);

    SignalMapping("Flatten mock callcount")
    .OnTrigger(SUT).At(&Mock::ping).CallCount().Expect(1);

    SignalMapping("Do not flatten mock assertions when no repeat set")
    .OnTrigger(SUT).At(&Mock::ping).Expect(L{}|ToList); // args are empty on void input

    SignalMapping("Do not flatten mock assertions when repeat is set")
    .OnTrigger(SUT).Repeat(2).At(&Mock::ping).Expect({L{}, L{}});
}


BOOST_AUTO_TEST_CASE(BadSignalPath)
{
    boost::json::value error(nullptr);
    zmbt::Config()
        .SetFailureHandler([&error](boost::json::value const& sts){ error = sts; });

    auto const sut = [](int){return 42;};

    SignalMapping("Invalid signal path")
    .OnTrigger(sut)
        .At(sut).Return("/foo/bar") .Expect(42)
    ;

    BOOST_CHECK_NE(error, nullptr);
    BOOST_CHECK_EQUAL(error.at("verdict"), "ERROR");
}

BOOST_AUTO_TEST_CASE(LangGlobalEnv)
{
    struct Mock
    {
        int produce()
        {
            return InterfaceRecord(&Mock::produce, this).Hook();
        }
    } mock_x, mock_y;

    auto const sut = [&](){
        auto x = mock_x.produce();
        auto y = mock_y.produce();
        return boost::json::array{x, y};
    };

    BOOST_CHECK_EQUAL(*(42 | EnvStore("foo")), 42); // storing x = 42

    SignalMapping("lang::GlobalEnv test")
    .OnTrigger(sut).Repeat(3)
        .At(&Mock::produce, mock_x).Return() .Inject(EnvStore("x") | Trace("x-store"))
        .At(&Mock::produce, mock_y).Return() .Inject(Noop | EnvLoad("x") | Trace("x-load") | D(0) | Mul(-1))
        .At(sut).Return().Expect()
        .Test
            (L{{0,0},{1,-1},{2,-2}}) ["env is reset before tests"]
            (L{{0,0},{1,-1},{2,-2}}) ["same output, env is reset before tests"]
        .PreRun([]{
            BOOST_CHECK_EQUAL(*EnvLoad("foo"), nullptr); // env is reset before tests
        })
    ;
    BOOST_CHECK_EQUAL(*EnvLoad("x"), 2); // state preserved after test
    zmbt::lang::GlobalEnv().Reset();
    BOOST_CHECK_EQUAL(*EnvLoad("x"), nullptr);
}
