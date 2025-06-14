/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>
#include "zmbt/model.hpp"


using namespace zmbt::reflect;
using namespace zmbt::dsl;
using namespace zmbt;
using boost::json::array;
using boost::json::object;


void test_logic(GenericSignalOperator const& T, GenericSignalOperator const& F)
{
    BOOST_CHECK(T);
    BOOST_CHECK(!F);

    BOOST_CHECK(T || F);
    BOOST_CHECK(F || T);
    BOOST_CHECK(!(T && F));
    BOOST_CHECK(!(F && T));
}

void test_order(GenericSignalOperator const& A, GenericSignalOperator const& B, GenericSignalOperator const& C)
{
    // For all a, !(a < a).
    auto holds_irreflexivity = [](auto const& a)
    {
        return !(a < a);
    };

    // If a < b then !(b < a).
    auto holds_assymetry = [](auto const& a, auto const& b)
    {
        return (a < b) ? !(b < a) : true;
    };

    // If a < b and b < c then a < c.
    auto holds_transitivity = [](auto const& a, auto const& b, auto const& c)
    {
        return ((a < b) && (b < c)) ? (a < c) : true;
    };

    auto equiv = [](auto const& a, auto const& b)
    {
        return !(a < b) && !(b < a);
    };

    // If equiv(a, b) and equiv(b, c), then equiv(a, c).
    auto holds_equivalence_transitivity = [equiv](auto const& a, auto const& b, auto const& c)
    {
        return (equiv(a, b) && equiv(b, c)) ? equiv(a, c) : true;
    };

    BOOST_CHECK_PREDICATE(holds_irreflexivity, (A));
    BOOST_CHECK_PREDICATE(holds_irreflexivity, (B));
    BOOST_CHECK_PREDICATE(holds_irreflexivity, (C));

    BOOST_CHECK_PREDICATE(holds_assymetry, (A)(B));
    BOOST_CHECK_PREDICATE(holds_assymetry, (A)(C));
    BOOST_CHECK_PREDICATE(holds_assymetry, (B)(A));
    BOOST_CHECK_PREDICATE(holds_assymetry, (B)(C));
    BOOST_CHECK_PREDICATE(holds_assymetry, (C)(A));
    BOOST_CHECK_PREDICATE(holds_assymetry, (C)(B));

    BOOST_CHECK_PREDICATE(holds_transitivity, (A)(B)(C));
    BOOST_CHECK_PREDICATE(holds_transitivity, (A)(C)(B));
    BOOST_CHECK_PREDICATE(holds_transitivity, (B)(A)(C));
    BOOST_CHECK_PREDICATE(holds_transitivity, (B)(C)(A));
    BOOST_CHECK_PREDICATE(holds_transitivity, (C)(A)(B));
    BOOST_CHECK_PREDICATE(holds_transitivity, (C)(B)(A));

    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (A)(B)(C));
    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (A)(C)(B));
    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (B)(A)(C));
    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (B)(C)(A));
    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (C)(A)(B));
    BOOST_CHECK_PREDICATE(holds_equivalence_transitivity, (C)(B)(A));
}

BOOST_AUTO_TEST_CASE(OrderedType)
{
    test_order(1, 2, 3);
    test_order("a", "b", "c");
    test_order(array{1,2}, array{2,3}, array{3,4});
}

BOOST_AUTO_TEST_CASE(LogicType)
{
    test_logic(1, 0);
    test_logic("a", "");
    test_logic(array{0}, array{});
    test_logic(object{{"", 0}}, object{});
}


BOOST_AUTO_TEST_CASE(NumberComparison)
{
    GenericSignalOperator x = 13.0;
    GenericSignalOperator y = 13;
    BOOST_CHECK_EQUAL(x, y);
    BOOST_CHECK_EQUAL(y, x);
}

BOOST_AUTO_TEST_CASE(SequenceEqual)
{
    GenericSignalOperator x = array{1,2,3,4,5};
    GenericSignalOperator y = array{1,2,3,4,5.0};

    BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE(SequenceOrder)
{
    GenericSignalOperator x = array{1,2,3,4,5};
    GenericSignalOperator y = array{1,2,3,4,6};

    BOOST_CHECK(x < y);
    BOOST_CHECK(y > x);

    BOOST_CHECK_LT(x, y);
    // BOOST_CHECK_GT(y, x);
}



BOOST_AUTO_TEST_CASE(GenericSignalSerialization)
{
    BOOST_CHECK(boost::json::has_value_from<GenericSignalOperator>::value);

    BOOST_CHECK_NO_THROW(json_from(GenericSignalOperator{nullptr}));
}