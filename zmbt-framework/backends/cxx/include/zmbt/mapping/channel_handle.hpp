/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_CHANNEL_HANDLE_HPP_
#define ZMBT_MAPPING_CHANNEL_HANDLE_HPP_

#include <list>
#include <boost/json.hpp>

#include "zmbt/core.hpp"
#include "zmbt/model/environment.hpp"
#include "zmbt/expr.hpp"

namespace zmbt {
namespace mapping {


/// Channel clause handle for SignalMapping model
class ChannelHandle
{
    JsonNode data_;
    Environment env;

public:
    using ConditionPipe = std::list<ChannelHandle>;
    using Kind = ChannelKind;

    ~ChannelHandle() = default;
    ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr);

    boost::json::string key() const;

    object_id host() const;
    interface_id interface() const;
    boost::json::string full_path() const;
    boost::json::string signal_path() const;
    Kind kind() const;
    std::tuple<int,int,int> slice() const;

    bool operator==(boost::json::value const& v) const;

    operator boost::json::value() const
    {
        return data_.node();
    }

    std::size_t index() const;
    /// Channel alias or index if not set
    boost::json::value alias() const;

    boost::json::array captures() const;

    lang::Expression transform() const;

};



} // namespace mapping
} // namespace zmbt

#endif