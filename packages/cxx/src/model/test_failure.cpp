/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/test_failure.hpp"



namespace
{
std::string format_failure_report(boost::json::value const& report)
{
    std::stringstream ss;
    ss << "ZMBT_TEST_FAILURE_BEGIN\n";
    zmbt::pretty_print(ss, report);
    ss << "\nZMBT_TEST_FAILURE_END\n";
    return ss.str();
}

} // namespace


namespace zmbt {


// GCOV_EXCL_START

void default_test_failure(boost::json::value const& report)
{
    auto formatter_report = format_failure_report(report);

#ifdef ZMBT_FAIL_TO_GTEST
#include <gtest/gtest.h>
    FAIL() << formatter_report;

#elif defined(ZMBT_FAIL_TO_BOOST)
#include <boost/test/unit_test.hpp>
    BOOST_ERROR(formatter_report);

#else
    throw std::runtime_error(formatter_report);

#endif
}
// GCOV_EXCL_END

} // namespace zmbt
