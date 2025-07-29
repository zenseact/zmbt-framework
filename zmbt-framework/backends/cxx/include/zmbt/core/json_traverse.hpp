/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_JSON_TRAVERSE_HPP_
#define ZMBT_CORE_JSON_TRAVERSE_HPP_

#include <functional>
#include <string>
#include "boost/json.hpp"



namespace zmbt {



/// Boost JSON value depth-first traverser
class JsonTraverse
{
    void traverse(boost::json::value const& v, std::string const jptr) const;
public:

    typedef std::function<bool(boost::json::value const&, std::string const)> visitor_fn;

    /// @brief Construct JSON traverser
    /// @param visit JSON node visitor: (Node, JSON Pointer) -> node traverse stop
    JsonTraverse(visitor_fn visit) : visit_{visit} {}

    void operator()(boost::json::value const& v) const
    {
        return traverse(v, "");
    }
private:
    visitor_fn const visit_;
};

}

#endif // ZMBT_CORE_JSON_TRAVERSE_HPP_
