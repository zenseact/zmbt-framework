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



class PipeHandle
{
    JsonNode data_;
    Environment env;

    std::list<ChannelHandle> channels_;
    boost::json::value observe_blend() const;


public:
    ~PipeHandle() = default;
    PipeHandle(JsonNode& model, std::size_t const pipe_idx)
        : data_(model.branch("/pipes/%d", pipe_idx))
    {
        auto const N = data_.at("/channels").as_array().size();
        auto const ptr_pref = data_.node_ptr();
        for (std::size_t i = 0; i < N; i++)
        {
            channels_.push_back({model, format("%s/channels/%d", data_.node_ptr(), i)});
        }
    }


    boost::json::value type() const;
    bool is_input() const;
    bool is_output() const;
    bool has_expression() const;
    lang::Expression expression() const;
    lang::Expression overload(lang::Expression const& e) const;

    void inject(lang::Expression expr) const;
    boost::json::value observe() const;

    int column() const;
    boost::json::value index() const;

};


} // namespace mapping
} // namespace zmbt

#endif