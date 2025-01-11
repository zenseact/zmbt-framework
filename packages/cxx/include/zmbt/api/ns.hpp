/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_API_NS_HPP_
#define ZMBT_API_NS_HPP_

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model.hpp"
#include "zmbt/mapping.hpp"

namespace zmbt {
namespace api {

using zmbt::Param;
using zmbt::type;

using zmbt::Environment;
using zmbt::InterfaceRecord;
using zmbt::precise;

using zmbt::Expression;
using namespace zmbt::expr;

using zmbt::mapping::SignalMapping;


struct ModelTestFixture;


#ifdef _DOXYGEN_

    /// @brief zmbt::Environment alias
    class Environment {};

    /// @brief zmbt::precise alias
    class precise {};

    /**
     * @brief zmbt::InterfaceRecord alias
     *
     * @param ...
     * @return auto
     */
    auto InterfaceRecord(...);

    /// @brief zmbt::type alias
    constexpr auto type;

    /// @brief zmbt::arg_index alias
    constexpr auto arg_index;

    /// @brief zmbt::return_value alias
    constexpr auto return_value;

#endif


}
}

#endif
