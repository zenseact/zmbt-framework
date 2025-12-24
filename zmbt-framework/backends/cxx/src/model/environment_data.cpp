/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/environment_data.hpp"


namespace zmbt {

boost::json::value EnvironmentData::init_json_data()
{
    return {
        {"interface_records", boost::json::object()},
        {"vars"             , boost::json::object()},
        {"refs"             , boost::json::object()},
        {"test_error"       , nullptr              },
    };
}

EnvironmentData::EnvironmentData() {}

EnvironmentData::~EnvironmentData() {}

}  // namespace zmbt
