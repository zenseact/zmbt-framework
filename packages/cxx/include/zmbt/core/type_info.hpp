/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_TYPE_INFO_HPP_
#define ZMBT_CORE_TYPE_INFO_HPP_


#include <type_traits>
#include <boost/type_index.hpp>


#include "aliases.hpp"

namespace zmbt {


/**
 * @brief Prettified typename
 *
 * @tparam T
 * @return std::string
 */
template <class T>
inline std::string type_name()
{
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

/**
 * @brief Prettified typename
 *
 * @tparam T
 * @return std::string
 */
template <class T>
inline std::string type_name(T)
{
    return type_name<T>();
}


}  // namespace zmbt

#endif  // ZMBT_CORE_TYPE_INFO_HPP_
