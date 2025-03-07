/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
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

/// Environment operation error
struct environment_error : public base_error {
    using base_error::base_error;
};

/// Expression evaluation error
struct expression_error : public base_error {
    using base_error::base_error;
};

/// Expression not implemented
struct expression_not_implemented : public expression_error {
    using expression_error::expression_error;
};

}  // namespace zmbt

#endif
