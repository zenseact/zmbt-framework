/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_GLOBAL_ENV_HPP_
#define ZMBT_EXPR_GLOBAL_ENV_HPP_

#include <memory>
#include <ostream>
#include <boost/json.hpp>

namespace zmbt {
namespace lang {


/// Global Storage env for Expression side effects
class GlobalEnv
{
    class Impl;
    std::shared_ptr<Impl> impl_;
public:

    GlobalEnv();

    GlobalEnv(GlobalEnv const&) = default;
    GlobalEnv(GlobalEnv&&) = default;
    GlobalEnv& operator=(GlobalEnv const&) = default;
    GlobalEnv& operator=(GlobalEnv&&) = default;

    ~GlobalEnv();

    // Store global variable at given JSON Pointer and return value if success, otherwise return error expression
    boost::json::value Store(boost::json::string_view const json_pointer, boost::json::value const& value) noexcept;

    // Load global variable if exist, otherwise return null
    boost::json::value Load(boost::json::string_view const json_pointer) const noexcept;

    // Reset global env state
    void Reset() noexcept;

    friend std::ostream& operator<<(std::ostream& os, GlobalEnv const& env);

};

}  // namespace lang
}  // namespace zmbt


#endif
