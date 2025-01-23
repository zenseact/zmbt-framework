/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_FIXTURE_MODEL_TEST_HPP_
#define ZMBT_FIXTURE_MODEL_TEST_HPP_

#include <array>
#include <tuple>

#include "ns.hpp"


namespace zmbt {

namespace api {

/**
 * @brief Generic model-driven test fixture
 *
 */
struct ModelTestFixture : public Environment {

    ModelTestFixture() = default;
    virtual ~ModelTestFixture() = default;
    ModelTestFixture(ModelTestFixture&&) = default;
    ModelTestFixture& operator=(ModelTestFixture&&) = default;

    ModelTestFixture(const ModelTestFixture&) = delete;
    ModelTestFixture& operator=(const ModelTestFixture&) = delete;

}; // class ModelTestFixture


}  // namespace api
}  // namespace zmbt

#endif  // ZMBT_FIXTURE_MODEL_TEST_HPP_
