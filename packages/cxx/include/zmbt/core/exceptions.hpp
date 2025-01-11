/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_EXCEPTIONS_HPP_
#define ZMBT_CORE_EXCEPTIONS_HPP_

#include <array>
#include <type_traits>

#include "aliases.hpp"
#include "format_string.hpp"

namespace zmbt {


struct base_error : public std::runtime_error {
    using std::runtime_error::runtime_error;

    template <class... A>
    base_error(boost::json::string_view fmtstr, A&&... args) : std::runtime_error(format(fmtstr, std::forward<A>(args)...))
    {
    }
};


/// Failed serialization
struct serialization_error : public base_error {
    using base_error::base_error;
};

}  // namespace zmbt

#endif
