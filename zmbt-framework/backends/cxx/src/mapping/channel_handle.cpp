/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>

#include "zmbt/model/environment_interface_record.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/mapping/channel_handle.hpp"


namespace zmbt {
namespace mapping {

ChannelHandle::ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr)
    : data_(model.branch(cnl_ptr))
{
}

bool ChannelHandle::is_input() const
{
    return data_.at("/role") == "inject";
}


bool ChannelHandle::is_output() const
{
    return data_.at("/role") == "observe";
}

bool ChannelHandle::is_fixed_input() const
{
    return data_.contains("/keep");
}

bool ChannelHandle::is_fixed_output() const
{
    return data_.contains("/expect");
}


bool ChannelHandle::operator==(boost::json::value const& v)
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

boost::json::value ChannelHandle::combine() const
{
    return data_.get_or_default("/combine", nullptr);
}


interface_id ChannelHandle::interface() const
{
    return env.InterfaceId(data_.at("/interface").as_string());
}

lang::Operator ChannelHandle::overload() const
{
    return data_.contains("overload") ? lang::Operator{data_.at("overload").as_string()} : lang::Operator {};
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
    boost::json::string const& k = data_.at("/kind").as_string();

    if("args"       == k) return Kind::Args;
    if("return"     == k) return Kind::Return;
    if("call_count" == k) return Kind::CallCount;
    if("exception"  == k) return Kind::Exception;
    if("ts"         == k) return Kind::Timestamp;
    if("tid"        == k) return Kind::ThreadId;
    return Kind::Undefined;
}

bool ChannelHandle::is_range() const
{
    return data_.at("call").is_array();
}

std::tuple<int,int,int> ChannelHandle::call() const
{
    if (is_range())
    {
        return boost::json::value_to<std::tuple<int,int,int>>(data_.at("call"));
    }
    else
    {
        // make one-element range from scalar spec
        int const idx = data_.at("call").as_int64();
        return {idx, idx, 1};
    }
}

int ChannelHandle::on_call() const
{
    if (data_.at("call") == "all")
    {
        return 0;
    }
    else if (data_.at("call").is_int64())
    {
        return data_.at("call").get_int64();
    }
    else
    {
        throw model_error("unresolved parameter in model execution: %s", data_.at("call"));
        return 0;
    }
}


void ChannelHandle::inject(lang::Expression e) const
{
    if (e.is_noop()) return;
    auto handle = Environment::InterfaceHandle(interface(), host());
    auto const op = overload();
    if (op.annotation() != lang::Operator{}.annotation())
    {
        e = expr::Overload(e, op.annotation());
    }
    handle.Inject(e, data_.at("/kind").as_string(), signal_path());
}

lang::Expression ChannelHandle::keep() const
{
    auto const& recur = data_.at("keep");
    if (recur.is_null()) return expr::Noop;
    else return lang::Expression(recur);
}

lang::Expression ChannelHandle::expect() const
{
    auto const& expect = data_.at("expect");
    return expect.is_null() ? lang::Expression(expr::Noop) : lang::Expression(expect);
}


void ChannelHandle::inject_fixed() const
{
    return inject(keep());
}


boost::json::value ChannelHandle::observe() const
{
    if (!is_output())
    {
        throw model_error("calling observe on non-output channel");
    }
    auto ifc_handle = Environment::InterfaceHandle(interface(), host());
    switch (kind())
    {
    case Kind::Return:
    case Kind::Args:
        {
            boost::json::string_view const group = (kind() == Kind::Args) ? "/args%s" : "/return%s";
            boost::json::string const jp {format(group, signal_path())};
            int start, stop, step;
            std::tie(start, stop, step) = call();
            boost::json::array capture_list = ifc_handle.CaptureSlice(jp, start, stop, step);
            if (!is_range() && capture_list.empty())
            {
                throw model_error("no captures for requested channel");
            }
            return is_range() ? capture_list : capture_list.at(0);
        }
        break;
    case Kind::Timestamp:
    {
        int start, stop, step;
        std::tie(start, stop, step) = call();
        boost::json::array ts_list = ifc_handle.CaptureSlice("/ts", start, stop, step);
        if (!is_range() && ts_list.empty())
        {
            throw model_error("no captures for requested channel");
        }
        return is_range() ? ts_list : ts_list.at(0);
    }
    case Kind::ThreadId:
    {
        int start, stop, step;
        std::tie(start, stop, step) = call();
        boost::json::array tid_list = ifc_handle.CaptureSlice("/tid", start, stop, step);
        if (!is_range() && tid_list.empty())
        {
            throw model_error("no captures for requested channel");
        }
        return is_range() ? tid_list : tid_list.at(0);
    }
    case Kind::CallCount:
        return ifc_handle.ObservedCalls();
    case ChannelHandle::Kind::Exception:
        throw model_error("Exception channel not implemented");
        break;
    default:
        throw model_error("invalid channel kind");
        break;
    }
    return nullptr;
}

boost::json::array const& ChannelHandle::captures() const
{
    auto ifc_handle = Environment::InterfaceHandle(interface(), host());
    return ifc_handle.Captures();
}

boost::json::value ChannelHandle::observe_with(std::list<ChannelHandle> channels)
{
    boost::json::array union_signal {};

    for (auto const& channel: channels)
    {
        union_signal.push_back(channel.observe());
    }

    return union_signal;
}

boost::json::value ChannelHandle::observe_union(std::list<ChannelHandle> channels)
{
    if (channels.empty())
    {
        return boost::json::array{};
    }

    boost::json::array join_captures {};

    /// triplets in form {alias, value, timestamp}
    auto const sort_pred = [](boost::json::value const& a, boost::json::value const& b) -> bool {
        return a.as_array().at(2).as_uint64() < b.as_array().at(2).as_uint64();
    };

    while(!channels.empty())
    {
        ChannelHandle const channel {std::move(channels.front())};
        channels.pop_front();
        auto const& alias = channel.alias();
        auto const& captures = channel.captures();

        auto ifc_handle = Environment::InterfaceHandle(channel.interface(), channel.host());
        if (channel.kind() == Kind::CallCount)
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
        std::tie(start, stop, step) = channel.call();
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

} // namespace mapping
} // namespace zmbt
