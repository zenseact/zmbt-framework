/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>

#include "zmbt/model/environment_interface_record.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/mapping/channel_handle.hpp"


namespace
{
    bool should_flatten_response(std::size_t const repeat_trigger, std::size_t const observed_size, boost::json::string_view role)
    {
        if (repeat_trigger > 1)
        {
            return false;
        }

        if (role.ends_with("one"))
        {
            if (observed_size == 1)
            {
                return true;
            }
            else
            {
                // TODO: return Error
                throw zmbt::model_error("expected single value capture, but got %s samples", observed_size);
            }
        }
        else if (role.ends_with("batch"))
        {
            return false;
        }
        else
        {
            if (observed_size == 0)
            {
                throw zmbt::model_error("expected non-empty capture, but got 0 samples");
            }
            return observed_size == 1;
        }
    }
}

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



std::tuple<int,int,int> ChannelHandle::slice() const
{
    return {0, -1, 1};
    // if (is_batch())
    // {
    //     return boost::json::value_to<std::tuple<int,int,int>>(data_.at("call"));
    // }
    // else
    // {
    //     // make one-element range from scalar spec
    //     int const idx = data_.at("call").as_int64();
    //     return {idx, idx, 1};
    // }
}

boost::json::array ChannelHandle::captures() const
{

    int start, stop, step;
    std::tie(start, stop, step) = slice();
    boost::json::array observed;
    auto const ifc_handle = Environment::InterfaceHandle(interface(), host());
    switch (kind())
    {
    case Kind::Return:
        observed = ifc_handle.CaptureSlice(format("/return%s", signal_path()), start, stop, step);
        break;
    case Kind::Args:
        observed = ifc_handle.CaptureSlice(format("/args%s", signal_path()), start, stop, step);
        break;
    case Kind::Timestamp:
        observed = ifc_handle.CaptureSlice("/ts", start, stop, step);
        break;
    case Kind::ThreadId:
        observed = ifc_handle.CaptureSlice("/tid", start, stop, step);
        break;
    case Kind::CallCount:
        observed.push_back(ifc_handle.ObservedCalls());
        break;
    case ChannelHandle::Kind::Exception:
        throw model_error("Exception channel not implemented");
        break;
    default:
        throw model_error("invalid channel kind");
        break;
    }

    return observed;
}


boost::json::value PipeHandle::type() const
{
    return data_.get_or_default("/type", nullptr);
}



lang::Expression PipeHandle::expression() const
{
    lang::Expression e = expr::Noop;

    if (auto const p =  data_.find_pointer("expr"))
    {
        e = *p;
    }
    return e;
}

bool PipeHandle::is_input() const
{
    return data_.at("/role") == "inject";
}


bool PipeHandle::is_output() const
{
    return data_.at("/role") != "inject";
}

bool PipeHandle::has_expression() const
{
    return data_.contains("/expr");
}


lang::Expression PipeHandle::overload(lang::Expression const& e) const
{
    if (data_.contains("overload"))
    {
        return expr::Overload(data_.at("overload").as_string(), e);
    }
    return e;
}

void PipeHandle::inject(lang::Expression e) const
{
    if (e.is_noop()) return;
    e = overload(e);

    bool const is_blend = type() == "blend";

    if (is_blend)
    {
        auto generator = std::make_shared<Generator>(e);
        for (auto const& channel: channels_)
        {
            auto handle = Environment::InterfaceHandle(channel.interface(), channel.host());
            handle.Inject(generator, channel.kind(), channel.signal_path());
        }
    }
    else
    {
        for (auto const& channel: channels_)
        {
            auto handle = Environment::InterfaceHandle(channel.interface(), channel.host());
            handle.Inject(std::make_shared<Generator>(e), channel.kind(), channel.signal_path());
        }
    }

}

boost::json::value PipeHandle::observe() const
{
    if (!is_output())
    {
        throw model_error("calling observe on non-output channel");
    }
    auto const& role = data_.at("role").as_string();
    boost::json::value result;
    auto const pipe_type = type();
    auto const repeat_trigger = boost::json::value_to<std::size_t>(data_.root_node().get_or_default("/repeat_trigger", 1));
    if ("group" == pipe_type)
    {
        result.emplace_array();
        auto& group_result = result.get_array();
        for (auto const& channel: channels_)
        {
            auto const captures = channel.captures();
            bool const flatten = should_flatten_response(repeat_trigger, captures.size(), role);
            group_result.push_back(flatten ? captures.at(0) : captures);
        }
    }
    else if ("blend" == pipe_type)
    {
        if (role.ends_with("one"))
        {
            throw model_error("cant use %s on blend pipe, try without -One", role);
        }
        return observe_blend();
    }
    else
    {
        auto const captures = channels_.back().captures();
        bool const flatten = should_flatten_response(repeat_trigger, captures.size(), role);
        result = flatten ? captures.at(0) : captures;
    }

    return result;
}



boost::json::value PipeHandle::observe_blend() const
{
    if (channels_.empty())
    {
        return boost::json::array{};
    }

    boost::json::array join_captures {};

    /// triplets in form {alias, value, timestamp}
    auto const sort_pred = [](boost::json::value const& a, boost::json::value const& b) -> bool {
        return a.as_array().at(2).as_uint64() < b.as_array().at(2).as_uint64();
    };

    for (auto const& channel: channels_)
    {
        auto const& alias = channel.alias();
        auto ifc_handle = Environment::InterfaceHandle(channel.interface(), channel.host());
        // Captures()
        auto const& captures = ifc_handle.Captures();
        //  channel.captures();

        if (channel.kind() == ChannelKind::CallCount)
        {
            boost::json::array record {
                alias,
                captures.size(),
                captures.empty() ? 0U : captures.crbegin()->as_object().at("ts")
            };
            join_captures.insert(
                std::upper_bound(join_captures.begin(), join_captures.end(), record,  sort_pred),
                record
            );
            continue;
        }

        auto const& full_path = channel.full_path();

        int start, stop, step;
        std::tie(start, stop, step) = channel.slice();
        auto captures_slice = make_slice_const_generator(captures, start, stop, step);
        auto insert_begin = join_captures.begin();
        auto capture = captures.cend();


        while ((capture = captures_slice()) != captures.cend())
        {
            boost::json::error_code ec;
            boost::json::value const* signal_value = capture->find_pointer(full_path, ec);
            boost::json::array record {
                alias,
                signal_value ? *signal_value : nullptr,
                capture->at_pointer("/ts")
            };

            insert_begin = join_captures.insert(
                std::upper_bound( insert_begin, join_captures.end(), record,  sort_pred),
                record
            );
        }
    }

    for (auto& row: join_captures)
    {
        row.as_array().pop_back();
    }

    return join_captures;
}


int PipeHandle::column() const
{
    return data_.get_or_default("column", -1).as_int64();
}


boost::json::value PipeHandle::index() const
{
    return data_.at("index");
}


} // namespace mapping
} // namespace zmbt
