/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_FILE_PARSER_HPP_
#define ZMBT_CORE_FILE_PARSER_HPP_

#include <array>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {


enum class FileFormat
{
    DeduceFromExtension,
    JSON,
    YAML,
    XML
};


}  // namespace zmbt

#endif
