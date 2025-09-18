/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>

#include "zmbt/model/environment_interface_record.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/mapping/channel_handle.hpp"



namespace zmbt {
namespace mapping {

ChannelHandle::ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr)
    : data_(model.branch(cnl_ptr))
{
}



bool ChannelHandle::operator==(boost::json::value const& v) const
{
    return data_.node() == v;
}

boost::json::string ChannelHandle::key() const
{
    return data_.at("/interface").as_string();
}

object_id ChannelHandle::host() const
{
    return env.ObjectId(data_.at("/interface").as_string());
}


lang::Expression ChannelHandle::transform() const
{
    lang::Expression e = expr::Noop;

    if (auto const p =  data_.find_pointer("transform"))
    {
        e = *p;
    }
    return e;
}

interface_id ChannelHandle::interface() const
{
    return env.InterfaceId(data_.at("/interface").as_string());
}


boost::json::string ChannelHandle::full_path() const
{
    return format("/%s%s",data_.at("/kind"), data_.at("/signal_path")).c_str();
}


boost::json::string ChannelHandle::signal_path() const
{
    return data_.at("/signal_path").as_string();
}


std::size_t ChannelHandle::index() const
{
    return data_.at("/index").as_uint64();

}

boost::json::value ChannelHandle::alias() const
{
    return data_.at("/alias");
}


ChannelHandle::Kind ChannelHandle::kind() const
{
    return dejsonize<ChannelKind>(data_.at("/kind"));
}



boost::json::array ChannelHandle::captures() const
{

    boost::json::array observed;
    auto const ifc_handle = Environment::InterfaceHandle(interface(), host());
    switch (kind())
    {
    case Kind::Return:
        observed = ifc_handle.CaptureSlice(format("/return%s", signal_path()));
        break;
    case Kind::Args:
        observed = ifc_handle.CaptureSlice(format("/args%s", signal_path()));
        break;
    case Kind::Timestamp:
        observed = ifc_handle.CaptureSlice("/ts");
        break;
    case Kind::ThreadId:
        observed = ifc_handle.CaptureSlice("/tid");
        break;
    case Kind::CallCount:
        observed.push_back(ifc_handle.ObservedCalls());
        break;
    case ChannelHandle::Kind::Exception:
        observed = ifc_handle.CaptureSlice("/exception");
        break;
    default:
        throw_exception(model_error("invalid channel kind"));
        break;
    }

    return observed;
}

} // namespace mapping
} // namespace zmbt
