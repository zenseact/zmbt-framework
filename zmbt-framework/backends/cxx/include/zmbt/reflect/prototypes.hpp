/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_REFLECT_PROTOTYPES_HPP_
#define ZMBT_REFLECT_PROTOTYPES_HPP_

#include "zmbt/core.hpp"
#include "signal_traits.hpp"
#include "serialization.hpp"
#include "invocation.hpp"

namespace zmbt {
namespace reflect {

namespace detail
{
template <class T>
auto init_return() -> mp_if<is_void<T>, boost::json::value>
{
    return nullptr;
}

template <class T>
auto init_return() -> mp_if<mp_not<is_void<T>>, boost::json::value>
{
    return json_from(reflect::signal_traits<T>::init());
}

template <class T>
auto init_args()
{
    return json_from(reflect::init_tuple<T>()).as_array();
}

} // namespace detail


/// Interface prototypes in JSON as {"args": [...], "return": ... }
template<class T>
boost::json::object prototypes()
{
    static_assert(is_ifc_handle<T>::value, "");

    using args_unqf_t = reflect::invocation_args_unqf_t<T>;
    using unqf_ret_t = reflect::invocation_ret_unqf_t<T>;
    return {
        {"args"  , detail::init_args<args_unqf_t>()},
        {"return", detail::init_return<unqf_ret_t>()}
    };
}

} // namespace reflect
} // namespace zmbt

#endif // ZMBT_REFLECT_PROTOTYPES_HPP_
