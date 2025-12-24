/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "zmbt/expr.hpp"


namespace utf = boost::unit_test;

using namespace zmbt::lang;

namespace std{
std::ostream& operator<<(std::ostream& os, Keyword const& k)
{
    os << keyword_to_str(k);
    return os;
}
}


static Encoding const TestEncoding = []{
    // Add(1) | Mul(2)
    Encoding e;
    e.keywords = {Keyword::Pipe, Keyword::Add, Keyword::Literal, Keyword::Mul, Keyword::Literal};
    e.depth = {0, 1, 2, 1, 2};
    e.data = {nullptr, nullptr, 1, nullptr, 2};
    return e;
}();


void test_slice(EncodingView const& view)
{
    auto const d = view.depth_offset();
    for (auto const& row : view)
    {
        BOOST_CHECK_EQUAL( row.keyword, TestEncoding.keywords.at(row.index));
        BOOST_CHECK_EQUAL( row.depth+d, TestEncoding.depth   .at(row.index));
        BOOST_CHECK_EQUAL(*row.data   , TestEncoding.data    .at(row.index));
    }
}


BOOST_AUTO_TEST_CASE(ViewSizeEmpty)
{
    EncodingView const view(TestEncoding);
    BOOST_CHECK_EQUAL(view.size(), 5);
    BOOST_CHECK(!view.empty());
}

BOOST_AUTO_TEST_CASE(ViewIndexing)
{
    EncodingView view(TestEncoding);

    for (std::size_t i = 0; i < TestEncoding.keywords.size(); i++)
    {
        BOOST_CHECK_EQUAL( view[i].keyword, TestEncoding.keywords.at(i));
        BOOST_CHECK_EQUAL( view[i].depth  , TestEncoding.depth   .at(i));
        BOOST_CHECK_EQUAL(*view[i].data   , TestEncoding.data    .at(i));
    }

    BOOST_CHECK_EQUAL(view.front().keyword, TestEncoding.keywords.front());
    BOOST_CHECK_EQUAL(view.back().keyword, TestEncoding.keywords.back());
}



BOOST_AUTO_TEST_CASE(ViewIteration)
{
    EncodingView const view(TestEncoding);
    test_slice(view);
}

BOOST_AUTO_TEST_CASE(ViewSlice)
{
    EncodingView const view(TestEncoding);

    auto const slice = view.slice(1, 3);
    BOOST_CHECK_EQUAL(slice.size(), 3);

    BOOST_REQUIRE_EQUAL(slice[0].index, 1);
    BOOST_REQUIRE_EQUAL(slice[1].index, 2);
    BOOST_REQUIRE_EQUAL(slice[2].index, 3);
    test_slice(slice);
}

BOOST_AUTO_TEST_CASE(ViewSubtree)
{
    EncodingView const view(TestEncoding);
    BOOST_CHECK_EQUAL(view.size(), 5);


    auto const subtree1 = view.subtree(1);
    BOOST_CHECK_EQUAL(subtree1.size(), 2);

    BOOST_CHECK_EQUAL(subtree1.front().keyword, Keyword::Add);
    BOOST_CHECK_EQUAL(subtree1.back().keyword, Keyword::Literal);

    BOOST_CHECK_EQUAL(subtree1.front().index, 1);
    BOOST_CHECK_EQUAL(subtree1.back().index, 2);
    test_slice(subtree1);


    auto const subtree2 = view.subtree(3);
    BOOST_CHECK_EQUAL(subtree2.size(), 2);

    BOOST_CHECK_EQUAL(subtree2.front().keyword, Keyword::Mul);
    BOOST_CHECK_EQUAL(subtree2.back().keyword, Keyword::Literal);

    BOOST_CHECK_EQUAL(subtree2.front().index, 3);
    BOOST_CHECK_EQUAL(subtree2.back().index, 4);
    test_slice(subtree2);


    auto const subtree3 = subtree2.subtree(1);
    BOOST_CHECK_EQUAL(subtree3.size(), 1);
    BOOST_CHECK_EQUAL(subtree3.back().keyword, Keyword::Literal);
    BOOST_CHECK_EQUAL(subtree3.back().index, 4);
    test_slice(subtree3);
}

BOOST_AUTO_TEST_CASE(ViewSubtrees)
{
    EncodingView const view(TestEncoding);
    auto const subtrees = view.children();

    BOOST_REQUIRE_EQUAL(subtrees.size(), 2);
    BOOST_CHECK(subtrees.front() == view.subtree(1));
    BOOST_CHECK(subtrees.back() == view.subtree(3));
}


BOOST_AUTO_TEST_CASE(ViewFreeze)
{
    EncodingView const view(TestEncoding);
    auto const subtree = view.subtree(3);
    Encoding const frozen = subtree.freeze();
    EncodingView const copyview(frozen);


    for (size_t i = 0; i < subtree.size(); i++)
    {
        BOOST_CHECK_EQUAL( subtree[i].keyword,  copyview[i].keyword);
        BOOST_CHECK_EQUAL( subtree[i].depth  ,  copyview[i].depth  );
        BOOST_CHECK_EQUAL(*subtree[i].data   , *copyview[i].data   );

        BOOST_CHECK_EQUAL(copyview[i].index, i  );
        BOOST_CHECK_EQUAL(subtree[i] .index, i+3);
    }
}



BOOST_AUTO_TEST_CASE(ViewCompare)
{
    {
        EncodingView const view(TestEncoding);
        auto other = view.freeze();
        EncodingView other_view(other);
        BOOST_CHECK(view == other_view);
        other.depth.back()++;
        BOOST_CHECK(view != other_view);
    }

    {
        boost::json::value const token("$[lol]");
        Encoding const enc1(token);
        Encoding const enc2(token);
        EncodingView const view1(enc1);
        EncodingView const view2(enc2);

        BOOST_TEST_INFO("" << zmbt::json_from(enc1));
        BOOST_TEST_INFO("" << zmbt::json_from(enc2));
        BOOST_CHECK(view1 == view2);
    }

}

BOOST_AUTO_TEST_CASE(EncodingJson)
{
    auto const js = zmbt::json_from(TestEncoding);
    std::cerr << js << '\n';
    Encoding from_js(js);

    std::cerr << zmbt::json_from(from_js.keywords) << '\n';
}

