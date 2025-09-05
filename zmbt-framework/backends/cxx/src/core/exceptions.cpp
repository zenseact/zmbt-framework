/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/application/log.hpp"
#include "zmbt/core/exceptions.hpp"


namespace zmbt {

void detail::log_exception(char const* type, char const* what)
{
    ZMBT_LOG_CERR(ERROR) << type << ": " << what;
    ZMBT_LOG_JSON(ERROR) << boost::json::object{
        {"type", type},
        {"what", what},
    };
}

}  // namespace zmbt
