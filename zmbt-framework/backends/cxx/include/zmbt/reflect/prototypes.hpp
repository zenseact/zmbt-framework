/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
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
class Prototypes
{
    boost::json::array args_;
    boost::json::value return_;

  public:
    Prototypes(boost::json::array&& args, boost::json::value&& ret)
        : args_(std::move(args))
        , return_(std::move(ret))
    {
    }

    Prototypes() : Prototypes({}, {})
    {
    }

    template<class T>
    Prototypes(T const&)
        : Prototypes(
            detail::init_args<reflect::invocation_args_unqf_t<T const&>>(),
            detail::init_return<reflect::invocation_ret_unqf_t<T const&>>()
        )
    {
    }

    boost::json::array const& args() const { return args_; };
    boost::json::value const& ret() const { return return_; };

};

} // namespace reflect
} // namespace zmbt

#endif // ZMBT_REFLECT_PROTOTYPES_HPP_
