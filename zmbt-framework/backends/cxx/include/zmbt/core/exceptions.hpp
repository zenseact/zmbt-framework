/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_EXCEPTIONS_HPP_
#define ZMBT_CORE_EXCEPTIONS_HPP_


#include <boost/throw_exception.hpp>

#include "format_string.hpp"
#include "type_info.hpp"

namespace zmbt {

/// ZMBT Base exception
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


// using boost::throw_exception;

namespace detail
{
    void log_exception(char const* type, char const* what);
}

template <class E>
void throw_exception(E&& e)
{
    // auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
    detail::log_exception(type_name(e).c_str(), e.what());
    boost::throw_exception(std::forward<E>(e));
}

}  // namespace zmbt

#endif
