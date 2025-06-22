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
    enum class Kind
    {
        Undefined, Args, Return, Exception, Timestamp, ThreadId, CallCount,
    };

    ~ChannelHandle() = default;
    ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr);

    bool is_input() const;
    bool has_expression() const;
    bool is_output() const;
    bool operator==(boost::json::value const& v);
    boost::json::string key() const;
    boost::json::value combine() const;

    object_id host() const;
    interface_id interface() const;
    lang::Operator overload() const;
    boost::json::string full_path() const;
    boost::json::string signal_path() const;
    Kind kind() const;
    bool is_batch() const;
    std::tuple<int,int,int> call() const;
    int on_call() const;


    operator boost::json::value() const
    {
        return data_.node();
    }

    std::size_t index() const;
    /// Channel alias or index if not set
    boost::json::value alias() const;
    boost::json::array const& captures() const;
    void inject(lang::Expression expr) const;
    void inject_expression() const;
    lang::Expression expression() const;
    boost::json::value observe(boost::json::string const& default_role = "") const;


    static boost::json::value observe_with(std::list<ChannelHandle> channels);
    static boost::json::value observe_union(std::list<ChannelHandle> channels);

};


} // namespace mapping
} // namespace zmbt

#endif