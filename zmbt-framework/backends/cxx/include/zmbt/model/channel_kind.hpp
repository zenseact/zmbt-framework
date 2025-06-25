/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_CHANNEL_KIND_HPP_
#define ZMBT_MODEL_CHANNEL_KIND_HPP_


#include "zmbt/reflect.hpp"

namespace zmbt {

enum class ChannelKind
{
    Undefined, Args, Return, Exception, Timestamp, ThreadId, CallCount,
};



template<>
struct reflect::custom_serialization<ChannelKind> {

    static boost::json::value
    json_from(ChannelKind const& k)
    {
        switch(k)
        {
            case ChannelKind::Args:       return "args";
            case ChannelKind::Return:     return "return";
            case ChannelKind::CallCount:  return "call_count";
            case ChannelKind::Exception:  return "exception";
            case ChannelKind::Timestamp:  return "ts";
            case ChannelKind::ThreadId:   return "tid";
            default:
                return nullptr;
        }
    }

    static ChannelKind
    dejsonize(boost::json::value const& v)
    {
        if("args"       == v) return ChannelKind::Args;
        if("return"     == v) return ChannelKind::Return;
        if("call_count" == v) return ChannelKind::CallCount;
        if("exception"  == v) return ChannelKind::Exception;
        if("ts"         == v) return ChannelKind::Timestamp;
        if("tid"        == v) return ChannelKind::ThreadId;
        return ChannelKind::Undefined;
    }
};

}  // namespace zmbt

#endif  // ZMBT_MODEL_CHANNEL_KIND_HPP_
