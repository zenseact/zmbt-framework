/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>


#include "zmbt/core.hpp"

using namespace zmbt;

constexpr std::int64_t M = std::numeric_limits<std::int64_t>::max();

BOOST_AUTO_TEST_CASE(Slice)
{
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, 3, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, M, 1), (boost::json::array{0, 1, 2}));

    // overflow in span
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, 4, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -4, 3, 1), (boost::json::array{0, 1, 2}));

    // overflow outside span
    BOOST_CHECK_EQUAL(slice({0,1,2},  4,  4, 1), (boost::json::array{}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -4, -4, 1), (boost::json::array{}));

    // negat stop
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, -1, 1), (boost::json::array{0, 1}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, 0, -2, 1), (boost::json::array{0,}));

    // negat start
    BOOST_CHECK_EQUAL(slice({0,1,2}, -3, 3, 1), (boost::json::array{0, 1, 2}));
    BOOST_CHECK_EQUAL(slice({0,1,2}, -2, 2, 1), (boost::json::array{   1,  }));


    // reverse
    BOOST_CHECK_EQUAL(slice({0,1,2},  2, M, -1), (boost::json::array{2,1,0}));
    BOOST_CHECK_EQUAL(slice({0,1,2},  0, 2, -1), (boost::json::array{}));

    // single element forward step yields identity, equivalent to python slice [0:1:1]
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 1, 1), (boost::json::array{42}));
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 1, 2), (boost::json::array{42}));
    // single element reverse step yields empty set, equivalent to python slice [0:1:-1]
    BOOST_CHECK_EQUAL(slice({42, 43},  0, 1, -1), (boost::json::array{}));

    BOOST_CHECK_THROW(slice({42, 43},  0, 0, 0), std::exception);
    BOOST_CHECK_THROW(slice({42, 43},  0, 0, 0), base_error);
}

struct StrToSliceSample
{
    boost::json::string_view slice_expr;
    std::int64_t start;
    std::int64_t stop;
    std::int64_t step;

    friend std::ostream& operator<<(std::ostream& os, StrToSliceSample const& sample)
    {
        os << format("StrToSliceSample(%s) => {start=%d, stop=%d, step=%d}", sample.slice_expr, sample.start, sample.stop, sample.step);
        return os;
    }
};

std::vector<StrToSliceSample> const StrToSliceSamples
{
    {"::"       ,   M   ,   M    ,  1     },
    {"42:"      ,  42   ,   M    ,  1     },
    {"42::"     ,  42   ,   M    ,  1     },
    {":42:"     ,   M   ,  42    ,  1     },
    {"::42"     ,   M   ,   M    , 42     },
    {"42:-42"   ,  42   , -42    ,  1     },
    {"3:4:5"    ,   3   ,   4    ,  5     },
    {"-3:-4:-5" ,  -3   ,  -4    , -5     },
};


BOOST_DATA_TEST_CASE(StrToSlice, StrToSliceSamples)
{


    using zmbt::detail::str_to_slice_idx;
    auto const idx = str_to_slice_idx(sample.slice_expr);
    BOOST_CHECK_EQUAL(idx.at(0), sample.start);
    BOOST_CHECK_EQUAL(idx.at(1), sample.stop);
    BOOST_CHECK_EQUAL(idx.at(2), sample.step);
}
