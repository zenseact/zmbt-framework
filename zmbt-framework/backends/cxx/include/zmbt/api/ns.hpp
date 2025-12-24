/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_API_NS_HPP_
#define ZMBT_API_NS_HPP_

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model.hpp"
#include "zmbt/decor.hpp"
#include "zmbt/mapping.hpp"

namespace zmbt {
namespace api {

using zmbt::Param;
using zmbt::type;

using zmbt::Environment;
using zmbt::InterfaceRecord;

using lang::Expression;

namespace decor = zmbt::decor;
namespace expr = zmbt::expr;

using zmbt::mapping::SignalMapping;

struct ModelTestFixture;

#ifdef _DOXYGEN_

/// Alias for zmbt::Param
class Param {};
/// Alias for zmbt::type
constexpr auto  type;
/// Alias for zmbt::Environment
class Environment {};
/// Alias for zmbt::InterfaceRecord
auto InterfaceRecord(...);
/// Alias for zmbt::precise
class precise {};
/// Alias for lang::Expression
class Expression {};
/// Alias for zmbt::mapping::SignalMapping
class SignalMapping {};

/// \brief Signal type decorators API
/// \details \see zmbt::decor
namespace decor {}

/// \brief Expression Language API
/// \details \see zmbt::expr
namespace expr {}

#endif

}
}

#endif
