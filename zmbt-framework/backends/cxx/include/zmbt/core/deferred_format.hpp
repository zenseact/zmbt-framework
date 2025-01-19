/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_DEFERRED_FORMAT_HPP_
#define ZMBT_MODEL_DEFERRED_FORMAT_HPP_

#include <boost/json.hpp>
#include <utility>

#include "json_node.hpp"

namespace zmbt {



/// Deferred string format, supports Param tokens
class DeferredFormat
{
    JsonNode data;

public:

    ~DeferredFormat() = default;
    DeferredFormat(DeferredFormat const&) = default;
    DeferredFormat(DeferredFormat &&) = default;
    DeferredFormat& operator=(DeferredFormat const&) = default;
    DeferredFormat& operator=(DeferredFormat &&) = default;

    static bool isDeferredFormat(boost::json::value const v);

    DeferredFormat(boost::json::value const& v);

    template <class T, class... R>
    DeferredFormat(boost::json::string_view base,  T&& param1, R&&... params)
        : DeferredFormat(boost::json::array{base, std::forward<T>(param1), std::forward<R>(params)...})
    {
    }


    // deferred params  map {param -> param pointer}
    boost::json::object const& deferred_params() const&;

    operator boost::json::value() const;

    boost::json::string to_string() const;
};


}

#endif