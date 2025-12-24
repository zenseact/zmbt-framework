/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_ERROR_INFO_HPP_
#define ZMBT_MODEL_ERROR_INFO_HPP_

#include <boost/json.hpp>


namespace zmbt
{

struct ErrorInfo
{
    boost::json::string type;
    boost::json::string what;
    boost::json::string context;

    boost::json::value to_json() const
    {
        return boost::json::object {
            {"type"   , type   },
            {"what"   , what   },
            {"context", context},
        };
    }
};

} // namespace zmbt

#endif
