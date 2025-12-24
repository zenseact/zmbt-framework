/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_EXCEPTIONS_HPP_
#define ZMBT_MODEL_EXCEPTIONS_HPP_

#include "zmbt/core/exceptions.hpp"

namespace zmbt {


/// Failed test assertion
struct test_assertion_failure : public base_error {
    using base_error::base_error;
};

/// Model definition error
struct model_error : public base_error {
    using base_error::base_error;
};


}  // namespace zmbt

#endif
