/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_TEST_FAILURE_HPP_
#define ZMBT_MODEL_TEST_FAILURE_HPP_

#include <sstream>

#include <boost/json.hpp>

#include "zmbt/core/json_pretty_print.hpp"

namespace zmbt {

/// default failure report formatter
void format_failure_report(std::ostream& os, boost::json::value const& report);

/// default test failure handler
void default_test_failure(boost::json::value const& report);

}

#endif
