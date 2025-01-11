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

namespace zmbt {
namespace mapping {

class ChannelHandle
{
    JsonNode data_;
    Environment env;

public:
    enum class Kind
    {
        Args, Return, Exception, Timestamp, ThreadId, CallCount,
    };

    ~ChannelHandle() = default;
    ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr);

    bool is_input() const;

    bool is_output() const;
    bool operator==(boost::json::value const& v);
    boost::json::string key() const;
    boost::json::value combine() const;

    object_id host() const;
    interface_id interface() const;
    SignalOperatorHandler op() const;
    boost::json::string full_path() const;
    boost::json::string signal_path() const;
    Kind kind() const;
    bool is_range() const;
    std::tuple<int,int,int> call() const;
    int on_call() const;


    operator boost::json::value() const
    {
        return data_.node();
    }

    /// Channel alias or index if not set
    boost::json::value alias() const;
    boost::json::array const& captures() const;
    void inject(boost::json::value value) const;
    boost::json::value observe() const;

    static boost::json::value observe_join(std::list<ChannelHandle> channels);
    static boost::json::value observe_series(std::list<ChannelHandle> channels);

};


} // namespace mapping
} // namespace zmbt

#endif