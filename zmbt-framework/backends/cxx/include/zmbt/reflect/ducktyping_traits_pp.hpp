/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_
#define ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_

#define CAT(a, b) a##b

#define ZMBT_TRAIT_HAS_TYPE(trait, name)     \
    template <class T, class = void>         \
    struct trait : std::false_type {};       \
    template <class T>                       \
    struct trait<                            \
        T, zmbt::void_t<typename T::name>>   \
    : std::true_type {};


#define ZMBT_TRAIT_HAS_MEMBER(trait, name)           \
    template <class T, class = void>                 \
    struct trait : std::false_type {};               \
    template <class T>                               \
    struct trait<T,                                  \
    zmbt::void_t<decltype(std::declval<T>().name())>>\
    : std::true_type {};


#define ZMBT_HAS_TYPE(name) ZMBT_TRAIT_HAS_TYPE(CAT(has_type_, name), name)
#define ZMBT_HAS_MEMBER(name) ZMBT_TRAIT_HAS_MEMBER(CAT(has_member_, name), name)




#endif // ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_
