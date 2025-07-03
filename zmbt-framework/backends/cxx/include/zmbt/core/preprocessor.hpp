/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 *
 * @brief type traits to check serialization compatibility
 */

#ifndef ZMBT_CORE_PREPROCESSOR_HPP_
#define ZMBT_CORE_PREPROCESSOR_HPP_

#include <boost/preprocessor.hpp> // IWYU pragma: export

#define ZMBT_STRINGIFY(x) ZMBT_STRINGIFY_IMPL(x)
#define ZMBT_STRINGIFY_IMPL(x) #x
#define ZMBT_CUR_LOC __FILE__ "#" ZMBT_STRINGIFY(__LINE__)

#define ZMBT_PP_NS_OPEN(s, state, ns) namespace ns {
#define ZMBT_PP_NS_CLOSE(s, state, ns) }
#define ZMBT_PP_NS_OPEN_ALL(...) BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_OPEN, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define ZMBT_PP_NS_CLOSE_ALL(...) BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_CLOSE, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define ZMBT_PP_INJECT_CODE_INTO_NS(code, ...) \
ZMBT_PP_NS_OPEN_ALL(__VA_ARGS__)               \
code                                            \
ZMBT_PP_NS_CLOSE_ALL(__VA_ARGS__)


#endif // ZMBT_CORE_PREPROCESSOR_HPP_
