/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/core.hpp"

using namespace zmbt;
using namespace boost::json;



BOOST_AUTO_TEST_CASE(JsonZipIterEmpty)
{

    JsonZipIter zip {};
    BOOST_CHECK(!zip.halt());
    BOOST_CHECK_EQUAL(zip.get(), boost::json::array{});
    zip++;
    BOOST_CHECK(zip.halt());
    BOOST_CHECK_EQUAL(zip.get(), boost::json::array{});
}

BOOST_AUTO_TEST_CASE(JsonProdIterEmpty)
{

    JsonProdIter prod {};
    BOOST_CHECK(!prod.halt());
    BOOST_CHECK_EQUAL(prod.get(), boost::json::array{});
    prod++;
    BOOST_CHECK(prod.halt());
    BOOST_CHECK_EQUAL(prod.get(), boost::json::array{});
}

BOOST_AUTO_TEST_CASE(JsonIterUnary)
{
    JsonZipIter zip {boost::json::array{{42, 13}}};
    BOOST_CHECK(!zip.halt());
    BOOST_CHECK_EQUAL(*zip, (boost::json::array{42}));
    BOOST_CHECK_EQUAL(*zip++, (boost::json::array{13}));
    zip++;
    BOOST_CHECK(zip.halt());
}


BOOST_AUTO_TEST_CASE(JsonIterCopy)
{
    JsonZipIter orig {boost::json::array{
        { 1,  2},
        {10, 20}
    }};

    JsonZipIter copy{orig};

    BOOST_CHECK_EQUAL(*orig  , (boost::json::array{1, 10}));
    BOOST_CHECK_EQUAL(*orig++, (boost::json::array{2, 20}));
    orig++;
    BOOST_CHECK(orig.halt());

    BOOST_CHECK(!copy.halt());
    BOOST_CHECK_EQUAL(*copy  , (boost::json::array{1, 10}));
    BOOST_CHECK_EQUAL(*copy++, (boost::json::array{2, 20}));
    copy++;
    BOOST_CHECK(copy.halt());
}


BOOST_AUTO_TEST_CASE(JsonIterZip)
{

    boost::json::array a, b, c;
    a = {  3,   2,   1};
    b = { 30,  20,  10};
    c = {"a", "b", "c"};

    JsonZipIter zip {{a, b, c}}; // temporary arg is owned by iterator


    BOOST_CHECK_EQUAL(*zip, (boost::json::array{3, 30, "a"}));
    BOOST_CHECK_EQUAL(*zip++, (boost::json::array{2, 20, "b"}));
    BOOST_CHECK_EQUAL(*zip++, (boost::json::array{1, 10, "c"}));

    zip++;
    BOOST_CHECK(zip.halt());
    BOOST_CHECK_EQUAL(*zip, boost::json::array{});
    BOOST_CHECK_EQUAL(*zip++, boost::json::array{});
}

BOOST_AUTO_TEST_CASE(InconsistentZip)
{

    JsonZipIter zip {boost::json::array{
        { 1,  2, 3},
        {10, 20}
    }};


    BOOST_CHECK_EQUAL(*zip, (boost::json::array{1, 10}));
    BOOST_CHECK_EQUAL(*zip++, (boost::json::array{2, 20}));
    BOOST_CHECK_EQUAL(*zip++, (boost::json::array{}));
    BOOST_CHECK(zip.halt());
}

BOOST_AUTO_TEST_CASE(JsonIterProd)
{
    boost::json::array const arr {
        {  3,   2,   1},
        {"a", "b", "c"}
    };

    JsonProdIter prod {arr}; // lvalue is not owned, iterators may be invalidated

    BOOST_CHECK_EQUAL(*prod  , (boost::json::array{3, "a"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{3, "b"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{3, "c"}));

    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{2, "a"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{2, "b"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{2, "c"}));

    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{1, "a"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{1, "b"}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{1, "c"}));

    prod++;
    BOOST_CHECK(prod.halt());
    BOOST_CHECK_EQUAL(*prod, boost::json::array{});
    BOOST_CHECK_EQUAL(*prod++, boost::json::array{});
}


BOOST_AUTO_TEST_CASE(JsonIterProdOneToMany)
{
    boost::json::array const arr {
        {  3,   2,   1},
        boost::json::array{42}
    };

    JsonProdIter prod {arr}; // lvalue is not owned, iterators may be invalidated

    BOOST_CHECK_EQUAL(*prod  , (boost::json::array{3, 42}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{2, 42}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{1, 42}));
    BOOST_CHECK_EQUAL(*prod++, (boost::json::array{}));
}
