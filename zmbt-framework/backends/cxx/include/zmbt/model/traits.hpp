/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_TRAITS_HPP_
#define ZMBT_MODEL_TRAITS_HPP_


#include "zmbt/core.hpp"
#include "zmbt/model/expression.hpp"


namespace zmbt {


namespace detail {

template <class T>
using is_param = is_same<remove_cvref_t<T>, Param>;


template <class T>
using is_expr = is_same<remove_cvref_t<T>, Expression>;

template <class T>
using is_ref = mp_or<boost::json::is_string_like<T>, is_param<T>, is_expr<T>>;

template <class T>
using not_ref = mp_not<is_ref<T>>;

template <class T>
using maybe_obj = mp_and<not_ref<T>, is_constructible<object_id, T>>;

template <class T>
using is_cal = is_ifc_handle<T>;

template <class T>
using not_cal = mp_not<is_cal<T>>;

} // namespace detail

template <class T, class R = void>
using require_str = mp_if<boost::json::is_string_like<T>, R>;

template <class T, class R = void>
using require_not_str = mp_if<mp_not<boost::json::is_string_like<T>>, R>;

template <class T, class R = void>
using require_cal = mp_if<detail::is_cal<T>, R>;

template <class T, class R = void>
using require_not_cal = mp_if<detail::not_cal<T>, R>;

template <class T, class R = void>
using require_obj = mp_if<detail::maybe_obj<T>, R>;

template <class O, class I, class R = void>
using require_literal = mp_if<mp_and<detail::maybe_obj<O>, detail::is_cal<I>>, R>;

template <class T, class R = void>
using require_json_from = mp_if<boost::json::has_value_from<T>, R>;

template <class T, class R = void>
using require_no_json_from = mp_if<mp_not<boost::json::has_value_from<T>>, R>;

} // namespace zmbt


#endif
