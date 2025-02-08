/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"
#include "zmbt/decor.hpp"



namespace utf = boost::unit_test;

using namespace zmbt;
using namespace zmbt::reflect;

using namespace zmbt::expr;
using namespace boost::json;

using V = boost::json::value;


BOOST_AUTO_TEST_CASE(LiteralExpr)
{
    BOOST_CHECK_EQUAL(Expression(42).eval(42), true);
    BOOST_CHECK_EQUAL((Size|2).eval({1,2}), true);
    BOOST_CHECK_EQUAL(Expression(nullptr).eval(nullptr), true);

    BOOST_CHECK_EQUAL(Expression(42).underlying(), 42);
    BOOST_CHECK_EQUAL(Expression(42).params(), 42);
    BOOST_CHECK_EQUAL(Expression("Lol").underlying(), "Lol");
    BOOST_CHECK_EQUAL(Expression("Lol").params(), "Lol");
}

BOOST_AUTO_TEST_CASE(GenericTernaryAndOr)
{
    using O = GenericSignalOperator;

    BOOST_CHECK_EQUAL((O{true} and O{true}), O{true});
    BOOST_CHECK_EQUAL((O{true} and O{false}), O{false});
    BOOST_CHECK_EQUAL((O{false} and O{true}), O{false});

    BOOST_CHECK_EQUAL((O{true} and O{42}), O{42});
    BOOST_CHECK_EQUAL((O{42} or O{""}), O{42});

    BOOST_CHECK_EQUAL((O{"foo"} and O{"bar"}) or O{"baz"}, O{"bar"});
    BOOST_CHECK_EQUAL((O{""}    and O{"bar"}) or O{"baz"}, O{"baz"});

    BOOST_CHECK_EQUAL((And(42)|Or(13)).eval(true), 42);
    BOOST_CHECK_EQUAL((And(42)|Or(13)).eval(false), 13);
}

boost::json::object NF(Keyword const& kw, boost::json::value const& v)
{
    return {{json_from(kw).as_string(), v}};
}


BOOST_AUTO_TEST_CASE(NormalFormBijection)
{
    Expression original {42};
    value js = original.serialize();
    Expression converted = js;

    BOOST_CHECK(original == converted);
}


BOOST_AUTO_TEST_CASE(ExplicitSerializationBijection)
{
    Expression original {42};
    value js = json_from(original);
    Expression converted = dejsonize<Expression>(js);
    BOOST_CHECK(original == converted);
}


BOOST_AUTO_TEST_CASE(ImplicitSerializationBijection)
{
    Expression original {42};
    value js = original;
    Expression converted = js;
    BOOST_CHECK(original == converted);
}

BOOST_AUTO_TEST_CASE(EqNoop)
{
    value js = Noop;
    BOOST_CHECK_EQUAL(js, json_from(Keyword::Noop));
}

BOOST_AUTO_TEST_CASE(Negation)
{
    BOOST_CHECK_NE(42|Not, Eq(42)|Not);

    BOOST_CHECK_EQUAL((42|Not).eval(13), Ne(42).eval(13));
    BOOST_CHECK_EQUAL((42|Not).eval(42), Ne(42).eval(42));

    BOOST_CHECK_EQUAL((42|Not|Not).eval(13), Eq(42).eval(13));
    BOOST_CHECK_EQUAL((42|Not|Not).eval(42), Eq(42).eval(42));
}

BOOST_AUTO_TEST_CASE(Conjunction)
{
    value expression = All(Gt(0), Lt(42));
    value should_be = NF(Keyword::All, array{
        NF(Keyword::Gt, 0),
        NF(Keyword::Lt, 42),
    });

    BOOST_CHECK_EQUAL(expression, should_be);
}


BOOST_AUTO_TEST_CASE(Disjunction)
{
    value expression = Any(Gt(0), Lt(42));
    value should_be = NF(Keyword::Any, array{
        NF(Keyword::Gt, 0),
        NF(Keyword::Lt, 42),
    });
    BOOST_CHECK_EQUAL(expression, should_be);
}

BOOST_AUTO_TEST_CASE(CustomOperator)
{
    BOOST_CHECK(Eq(42).match(42, SignalOperatorHandler(type<int>)));
}

// SET OPS
/*

| lhs      | rhs    |  op   |  v  |
|----------|--------|-------|-----|
| arrays                          |
| {a}      | {a, b}  |  ∈   |  F  |
| {a}      |{{a},{b}}|  ∈   |  T  |
| {a}      | {a, b}  |  ⊆   |  T  |
| {a}      |{{a},{b}}|  ⊆   |  F  |
|  a       | {a, b}  |  ∈   |  T  |
|  a       | {a, b}  |  ⊆   |  T  | <-- due to js ctor equivalence for {a} == a this resolves to {a} ⊆ {a, b}
| objects                         |
|  key     | obj    |  ∈    |  T  |
|  key     | obj    |  ⊆    |  F  |
|{key, val}| obj    |  ∈    |  T  |
|{key, val}| obj    |  ⊆    |  T  |

*/


BOOST_AUTO_TEST_CASE(OrderMatch)
{
    BOOST_CHECK(Lt(42).match(41));
    BOOST_CHECK(!Lt(42).match(42));
    BOOST_CHECK(Gt(42).match(43));
    BOOST_CHECK(!Gt(42).match(42));
    BOOST_CHECK(Le(42).match(41));
    BOOST_CHECK(Le(42).match(42));
    BOOST_CHECK(Ge(42).match(43));
    BOOST_CHECK(Ge(42).match(42));

    // auto args
    BOOST_CHECK(Lt.match({41, 42}));
    BOOST_CHECK(!Lt.match({42, 42}));
    BOOST_CHECK(Gt.match({43, 42}));
    BOOST_CHECK(!Gt.match({42, 42}));
    BOOST_CHECK(Le.match({41, 42}));
    BOOST_CHECK(Le.match({42, 42}));
    BOOST_CHECK(Ge.match({43, 42}));
    BOOST_CHECK(Ge.match({42, 42}));
}


BOOST_AUTO_TEST_CASE(InNotIn)
{
    BOOST_CHECK(In({1, 2}).match(2)); // `2 ∋ [1,2]`
    BOOST_CHECK(NotIn({1, 2}).match(3));

    BOOST_CHECK(In.match({2, {1, 2}})); // 2 ∋ [1,2]
    BOOST_CHECK(NotIn.match({3, {1, 2}}));
}


BOOST_AUTO_TEST_CASE(NiNotNi)
{
    BOOST_CHECK(Ni(2).match({1, 2})); // `[1,2] ∈ 2`
    BOOST_CHECK(NotNi(3).match({1, 2}));

    BOOST_CHECK(Ni.match({{1, 2}, 2})); // `[1,2] ∈ 2`
    BOOST_CHECK(NotNi.match({{1, 2}, 3}));
}


BOOST_AUTO_TEST_CASE(MatchWithPreciseOp)
{
    auto op = SignalOperatorHandler(type<decor::precise<double>>);
    BOOST_CHECK(In({0.5, 0.2})    .match(0.2)    );
    BOOST_CHECK(In({0.5, 0.2})    .match(0.2, op));
    BOOST_CHECK(In({1.1f, 0.2, 1}).match(1, op)  );

    BOOST_CHECK(NotIn({0.5, 0.2})    .match(0.3)    );
    BOOST_CHECK(NotIn({0.5, 0.2})    .match(0.3, op));
    BOOST_CHECK(NotIn({1.1f, 0.2, 1}).match(2, op)  );
}


BOOST_AUTO_TEST_CASE(SetOperationsOnArray)
{

    BOOST_CHECK(Subset({13, 42}).match(boost::json::array{13}));
    BOOST_CHECK(In({13, 42}).match(13));

    // json value init list ctor in match arg: {13} resolves to 13
    BOOST_CHECK_THROW(Subset({13, 42}).match({13}), std::exception);
    // SetExpression arg: {13} preserved as array
    BOOST_CHECK_NO_THROW(Superset({13}).match({13, 42}));
    BOOST_CHECK_NO_THROW(Subset({13}).match({13, 42}));

    boost::json::array SUP {13, 42};
    boost::json::array sub {13, 13, 13}; // set equivalent to {13}
    boost::json::value X {13};


    BOOST_CHECK( Subset(SUP).match(sub));
    BOOST_CHECK(!Subset(sub).match(SUP));
    BOOST_CHECK( Subset.match({sub, SUP}));
    BOOST_CHECK(!Subset.match({SUP, sub}));

    BOOST_CHECK( Superset(sub).match(SUP));
    BOOST_CHECK(!Superset(SUP).match(sub));
    BOOST_CHECK( Superset.match({SUP, sub}));
    BOOST_CHECK(!Superset.match({sub, SUP}));

    BOOST_CHECK(Subset  (SUP).match(SUP));
    BOOST_CHECK(Subset  (sub).match(sub));
    BOOST_CHECK(Superset(SUP).match(SUP));
    BOOST_CHECK(Superset(sub).match(sub));

    BOOST_CHECK(!ProperSubset  (SUP).match(SUP));
    BOOST_CHECK(!ProperSubset  (sub).match(sub));
    BOOST_CHECK(!ProperSuperset(SUP).match(SUP));
    BOOST_CHECK(!ProperSuperset(sub).match(sub));

    BOOST_CHECK_THROW(      Subset  (SUP).match(X), std::exception);
    BOOST_CHECK_THROW(      Superset(SUP).match(X), std::exception);
    BOOST_CHECK_THROW(ProperSubset  (SUP).match(X), std::exception);
    BOOST_CHECK_THROW(ProperSuperset(SUP).match(X), std::exception);


    BOOST_CHECK(!In(SUP).match(sub));
    BOOST_CHECK(!Ni(sub).match(SUP));

    BOOST_CHECK(In(SUP).match(X));
    BOOST_CHECK(Ni(X).match(SUP));
}


BOOST_AUTO_TEST_CASE(SetOperationsOnObject)
{

    BOOST_CHECK(Subset({{"lol", 42}, {"kek", 13}}).match({{"lol", 42}}));
    BOOST_CHECK(In({{"lol", 42}, {"kek", 13}}).match({"lol", 42}));

    boost::json::object set {{"lol", 42}, {"kek", 13}};
    boost::json::object obj {{"lol", 42}, {"lol", 0}}; // will create {{"lol", 42}}
    boost::json::array  kvp {"lol", 42};
    boost::json::string key {"lol"};

    BOOST_CHECK(Subset(set).match(obj));
    BOOST_CHECK(In    (set).match(kvp));
    BOOST_CHECK(In    (set).match(key));

    BOOST_CHECK_THROW(Subset(set).match(kvp), std::exception);
    BOOST_CHECK_THROW(Subset(set).match(key), std::exception);
    BOOST_CHECK_THROW(In    (set).match(obj), std::exception);
}

BOOST_AUTO_TEST_CASE(ListInitObj)
{
    Expression expr1 {{"lol", 42}, {"kek", 13}};
    Expression expr2 = {{"lol", 42}, {"kek", 13}};
    Expression expr3 ({{"lol", 42}, {"kek", 13}});
    Expression expr4 = [](Expression const& e) -> Expression { return e; }({{"lol", 42}, {"kek", 13}});
    Expression expr5 = [](Expression const&, Expression const& e) -> Expression { return e; }("ignored", {{"lol", 42}, {"kek", 13}});

    BOOST_CHECK(expr1 == expr2);
    BOOST_CHECK(expr2 == expr3);
    BOOST_CHECK(expr3 == expr1);
    BOOST_CHECK(expr4 == expr1);
    BOOST_CHECK(expr5 == expr1);
}

BOOST_AUTO_TEST_CASE(ListInitArr)
{
    Expression expr1 {1, 2};
    Expression expr2 = {1, 2};
    Expression expr3 ({1, 2});
    Expression expr4 = [](Expression const& e) -> Expression { return e; }({1, 2});
    Expression expr5 = [](Expression const&, Expression const& e) -> Expression { return e; }("ignored", {1, 2});

    BOOST_CHECK(expr1 == expr2);
    BOOST_CHECK(expr2 == expr3);
    BOOST_CHECK(expr3 == expr1);
    BOOST_CHECK(expr4 == expr1);
    BOOST_CHECK(expr5 == expr1);
}

BOOST_AUTO_TEST_CASE(SeqComp)
{
    auto expr = Lt(boost::json::array{0,9,2,3,4,5,6,7,8,9 });
    BOOST_CHECK(expr.match(boost::json::array{0,1,2,3,4,5,6,7,8,9}));
}

BOOST_AUTO_TEST_CASE(AtQueryMatch)
{
    boost::json::object obj {
        {"home",{
            {"user",{
                {"Desktop"  , {"foo", "bar", "baz"}},
                {"Downloads", {"lol", "kek"}}
            }}
        }}
    };

    BOOST_CHECK_MESSAGE((At("/home/user/Downloads/1")| "kek").match(obj), obj);
    BOOST_CHECK((At("/home/user/Desktop/2")|"baz").match(obj));
    BOOST_CHECK((At("/home/user/Desktop") | At(1) | Eq("bar")).match(obj));

    // recursive At
    BOOST_CHECK((At("/home")|At("/user")|At("/Desktop")   | At(0) | Eq("foo")).match(obj));
    BOOST_CHECK((At("/home")|At("/user")|At("/Downloads") | Contains("kek")).match(obj));
    BOOST_CHECK((At("/undefined") | nullptr).match(obj));


    // joining At
    // query array
    BOOST_CHECK((At({"/home/user/Downloads/1", "/home/user/Desktop/2"}) | Eq({"kek", "baz"})).match(obj));
    // query nested array
    BOOST_CHECK((At({{"/home/user/Downloads/1", "/home/user/Desktop/2"}, "/undefined" }) | Eq({{"kek", "baz"}, nullptr})).match(obj));
    // query object
    BOOST_CHECK((At({{"a", "/home/user/Downloads/1"}, {"b", "/home/user/Desktop/2"}}) | Eq({{"a", "kek"}, {"b", "baz"}})).match(obj));
    // query nested structures, a: array, b: object
    BOOST_CHECK((At({{"a", {"/home/user/Downloads/1", "/home/user/Desktop/2"}}, {"b", {{"null", "/undefined"}}}}) | Eq({{"a", {"kek", "baz"}}, {"b", {{"null", nullptr}}}})).match(obj));

    // dynamic keys
    BOOST_CHECK((At({{"$/home/user/Desktop/0", "/home/user/Desktop/1"}}) | Eq({{"foo", "bar"}}) ).match(obj));

}


BOOST_AUTO_TEST_CASE(TestId)
{
    BOOST_CHECK_EQUAL(Id.eval(42), 42);
    BOOST_CHECK_EQUAL(Id.eval(""), "");
    BOOST_CHECK_EQUAL(Id.eval(42), 42);
}

BOOST_AUTO_TEST_CASE(TestConst)
{
    BOOST_CHECK_EQUAL(True.eval(42) , true   );
    BOOST_CHECK_EQUAL(False.eval(42), false  );
    BOOST_CHECK_EQUAL(Null.eval(42) , nullptr);
}

BOOST_AUTO_TEST_CASE(TestBool)
{
    BOOST_CHECK_EQUAL(true , Bool.eval(42));
    BOOST_CHECK_EQUAL(true , Bool.eval("42"));
    BOOST_CHECK_EQUAL(true , Bool.eval({1,2}));

    BOOST_CHECK_EQUAL(false, Bool.eval(false));
    BOOST_CHECK_EQUAL(false, Bool.eval(nullptr));
    BOOST_CHECK_EQUAL(false, Bool.eval(boost::json::array{}));
}



BOOST_AUTO_TEST_CASE(TestAdd)
{

    BOOST_CHECK_EQUAL(4,  Add.eval({2,2}));
    BOOST_CHECK_EQUAL(-4, Add.eval({-2,-2}));

    BOOST_CHECK_EQUAL( 4, Add( 2).eval( 2));
    BOOST_CHECK_EQUAL(-4, Add(-2).eval(-2));

    BOOST_CHECK_THROW(Add.eval({2,2,2}), std::exception);
}

BOOST_AUTO_TEST_CASE(TestMod)
{

    BOOST_CHECK_EQUAL(Mod.eval({4,2}), 0);
    BOOST_CHECK_EQUAL(Mod.eval({5,2}), 1);
    BOOST_CHECK_EQUAL(Mod.eval({7,4}), 3);

    BOOST_CHECK_EQUAL(Mod(2).eval(1), 1);
    BOOST_CHECK_EQUAL(Mod(2).eval(2), 0);
    BOOST_CHECK_EQUAL(Mod(2).eval(3), 1);
    BOOST_CHECK_EQUAL(Mod(2).eval(4), 0);


}


BOOST_AUTO_TEST_CASE(TestReduce)
{
    BOOST_CHECK_EQUAL(8,  Reduce(Add, 0).eval({2,2,2,2}));
    BOOST_CHECK_EQUAL(10, Reduce(Add, 0).eval({1,2,3,4}));
    BOOST_CHECK_EQUAL(13, Reduce(Add, 3).eval({1,2,3,4}));

    BOOST_CHECK_EQUAL("bar", Reduce(And, 42).eval({"foo", "bar"}));
    BOOST_CHECK_EQUAL(0    , Reduce(And,  0).eval({"foo", "bar"}));

    BOOST_CHECK_EQUAL(42   , Reduce(Or , 42).eval({"foo", "bar"}));
    BOOST_CHECK_EQUAL("foo", Reduce(Or ,  0).eval({"foo", "bar"}));
}


BOOST_AUTO_TEST_CASE(TestRepeat)
{
    BOOST_CHECK_EQUAL(boost::json::value({42,42,42}), Repeat(3).eval(42));
}

BOOST_AUTO_TEST_CASE(TestComposePipe)
{
    auto const test = boost::json::parse(R"({
        "foo": {
            "bar": {
                "baz": 42
            }
        }
    })");

    BOOST_CHECK_EQUAL(42, Compose(At("/baz"), At("/bar"), At("/foo")).eval(test));
    BOOST_CHECK_EQUAL(42, (At("/foo") | At("/bar") | At("/baz")).eval(test));
}

BOOST_AUTO_TEST_CASE(TestComposeRepeat)
{
    boost::json::value const rep4x3 {
        {42,42,42},
        {42,42,42},
        {42,42,42},
        {42,42,42},
    };
    BOOST_CHECK_EQUAL(rep4x3, Compose(Repeat(4), Repeat(3)).eval(42));
    BOOST_CHECK_EQUAL(rep4x3, (Repeat(3) | Repeat(4)).eval(42));
    BOOST_CHECK_EQUAL(rep4x3, (Repeat(3) | Repeat(4) <<= 42).eval());
}

BOOST_AUTO_TEST_CASE(TestComposeMapFilterAt)
{
    auto const AllTrueFirst = Compose(Map(At(0)), Filter(At(1)|true));
    auto const AllFalseFirst  = Filter(At(1) | Eq(false)) | Map(At(0));

    boost::json::array const pairs {
        {"lol", true},
        {"kek", true},
        {"foo", false},
        {"bar", false},
    };

    BOOST_CHECK_EQUAL(AllTrueFirst.eval(pairs), V({"lol", "kek"}));
    BOOST_CHECK_EQUAL(AllFalseFirst.eval(pairs), V({"foo", "bar"}));
}



BOOST_AUTO_TEST_CASE(TestMap)
{
    BOOST_CHECK_EQUAL(Map(Add(10)).eval({1,2,3,4}), V({11,12,13,14}));
    BOOST_CHECK_EQUAL(Map(Mod(2)).eval({1,2,3,4}), V({1,0,1,0}));

    BOOST_CHECK_EQUAL(Map(Pow(0.5)).eval({4,9,16,25}), V({2,3,4,5}));
}



BOOST_AUTO_TEST_CASE(TestP)
{
    Param const p1 {1};
    Param const p2 {2};

    auto const expr = Compose(Map(At(p1)), Filter(At(p2)|false));
    boost::json::array list{};

    JsonTraverse([&](boost::json::value const& v, std::string const jp){
        if (Param::isParam(v)) list.push_back(value_from(jp));
        return false;
    })(expr.underlying());
    BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(TestFilter)
{
    BOOST_CHECK_EQUAL(Filter(Mod(2)).eval({1,2,3,4}), V({1,3}));
    BOOST_CHECK_EQUAL(Filter(Mod(2)|Nil).eval({1,2,3,4}), V({2,4}));

    auto F = Compose(Eq(3), Mod(4));
    BOOST_CHECK_EQUAL(Filter(F).eval({7,8,10,11,13,15}), V({7,11,15}));
}




BOOST_AUTO_TEST_CASE(SerializationUndefinedSpeed, *utf::timeout(1))
{
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<zmbt::Keyword>(":ArbitraryStringNotPresentInKeywords");
        if (kw != zmbt::Keyword::Undefined)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}

BOOST_AUTO_TEST_CASE(SerializationSpeed, *utf::timeout(1))
{
    boost::json::string const test_kw = json_from(zmbt::Keyword::ProperSuperset).as_string();
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<zmbt::Keyword>(test_kw);
        if (kw != zmbt::Keyword::ProperSuperset)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}

BOOST_AUTO_TEST_CASE(TestPow)
{
    BOOST_CHECK_EQUAL(Pow.eval({3,2}), 9);
    BOOST_CHECK_EQUAL(Pow.eval({9,0.5}), 3);

    BOOST_CHECK_EQUAL(Pow(2).eval(3), 9);
    BOOST_CHECK_EQUAL(Pow(0.5).eval(9), 3);

}

BOOST_AUTO_TEST_CASE(TestApply)
{
    BOOST_CHECK_EQUAL(Apply(Pow(2), 3).eval(), 9);

    BOOST_CHECK_EQUAL((Pow(3) <<= 4).eval(), 64);
}

BOOST_AUTO_TEST_CASE(TestComposeVsApplyPrecedence)
{
    // <<= has lower precedence than |

    // Invalid expression: Add(2) <<= 2 | Eq(4) is Add(2) <<= (2 | Eq(4))
    BOOST_CHECK_NE(Add(2) <<= 2 | Eq(4), Compose(Eq(4), Apply(Add(2), 2)));

    BOOST_CHECK_EQUAL((Add(2) <<= 2) | Eq(4), Compose(Eq(4), Apply(Add(2), 2)));
    BOOST_CHECK_EQUAL(((Add(2) <<= 2) | Eq(4)).eval(), true);
}


BOOST_AUTO_TEST_CASE(NotImplemented)
{
    BOOST_CHECK_THROW(Quot.eval(2), std::exception);
    BOOST_CHECK_THROW(Log.eval(2), std::exception);
    BOOST_CHECK_THROW(Sin.eval(2), std::exception);
    BOOST_CHECK_THROW(Cos.eval(2), std::exception);
    BOOST_CHECK_THROW(Tan.eval(2), std::exception);
    BOOST_CHECK_THROW(Asin.eval(2), std::exception);
    BOOST_CHECK_THROW(Acos.eval(2), std::exception);
    BOOST_CHECK_THROW(Atan.eval(2), std::exception);
    BOOST_CHECK_THROW(Sinh.eval(2), std::exception);
    BOOST_CHECK_THROW(Cosh.eval(2), std::exception);
    BOOST_CHECK_THROW(Tanh.eval(2), std::exception);
    BOOST_CHECK_THROW(Asinh.eval(2), std::exception);
    BOOST_CHECK_THROW(Acosh.eval(2), std::exception);
    BOOST_CHECK_THROW(Atanh.eval(2), std::exception);
}
