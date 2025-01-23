/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/test_failure.hpp"
#include "zmbt/model/exceptions.hpp"



// GCOV_EXCL_START

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
void default_test_failure(boost::json::value const& report)
{
    throw test_assertion_failure(format_failure_report(report));
}
} // namespace zmbt

// GCOV_EXCL_END
