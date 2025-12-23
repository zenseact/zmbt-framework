/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_PIPE_HANDLE_HPP_
#define ZMBT_MAPPING_PIPE_HANDLE_HPP_

#include <list>
#include <boost/json.hpp>

#include "zmbt/core.hpp"
#include "zmbt/model/environment.hpp"
#include "zmbt/expr.hpp"
#include "channel_handle.hpp"


namespace zmbt {
namespace mapping {


/// Pipe clause handle for SignalMapping model
class PipeHandle
{
    JsonNode data_;
    Environment env;
    bool should_flatten_;

    std::list<ChannelHandle> channels_;
    boost::json::value observe_blend() const;
    mutable boost::json::value id_cache_{};

public:


    PipeHandle(JsonNode& model, std::size_t const pipe_idx);

    ~PipeHandle() = default;

    boost::json::value type() const;
    bool is_input() const;
    bool is_output() const;
    bool has_expression() const;
    lang::Expression expression() const;
    bool overload(lang::Expression& e) const;

    void inject(lang::Expression expr) const;
    boost::json::value observe() const;

    int column() const;
    boost::json::value index() const;
    boost::json::value id() const;

    std::list<ChannelHandle> const& channels() const
    {
        return channels_;
    }

};


} // namespace mapping
} // namespace zmbt

#endif