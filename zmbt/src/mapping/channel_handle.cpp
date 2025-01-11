/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>

#include "zmbt/model/environment_interface_record.hpp"
#include "zmbt/mapping/channel_handle.hpp"


namespace
{
boost::json::value get_at_pointer(boost::json::value const& val, boost::json::string_view ptr)
{
    try
    {
        return val.at_pointer(ptr);
    }
    catch(const std::exception& e)
    {
        throw zmbt::model_error("failed to retrieve `%s` from `%s`, %s", ptr, val, e.what());
    }
    return nullptr;
}
} // namespace


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


SignalOperatorHandler ChannelHandle::op() const
{
    return data_.contains("operator") ? env.GetOperator(data_.at("operator").as_string()) : SignalOperatorHandler {};
}


boost::json::string ChannelHandle::full_path() const
{
    return format("/%s%s",data_.at("/kind"), data_.at("/signal_path")).c_str();
}


boost::json::string ChannelHandle::signal_path() const
{
    return data_.at("/signal_path").as_string();
}

boost::json::value ChannelHandle::alias() const
{
    return data_.at("/alias");
}


ChannelHandle::Kind ChannelHandle::kind() const
{
    static std::unordered_map<boost::json::string, Kind> const map =
    {
        {"call_count", Kind::CallCount},
        {"exception" , Kind::Exception},
        {"ts"        , Kind::Timestamp},
        {"tid"       , Kind::ThreadId },
        {"args"      , Kind::Args     },
        {"return"    , Kind::Return   }
    };
    boost::json::string const& k = data_.at("/kind").as_string();
    return map.at(k);
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


void ChannelHandle::inject(boost::json::value value) const
{
    auto handle = Environment::IfcRec(host(), interface());
    value = op().decorate(value);
    bool success {false};

    switch (kind())
    {
    case Kind::Return:
    case Kind::Args:
    {
        bool const is_args = kind() == ChannelHandle::Kind::Args;
        if (is_range())
        {
            boost::json::string_view group = is_args ? "/args%s" : "/return%s";
            boost::json::string jp {format(group, signal_path())};
            handle.SetInjectsRange(value, jp);
            success = true;
        }
        else {
            boost::json::error_code ec;
            boost::json::value signal_node;
            bool const is_args = kind() == ChannelHandle::Kind::Args;
            signal_node = is_args ? handle.GetInjectionArgs(on_call()) : handle.GetInjectionReturn(on_call());
            success = signal_node.set_at_pointer(signal_path(), value, ec);
            if (success)
            {
                is_args ? handle.InjectArgs(signal_node.as_array(), on_call()) : handle.InjectReturn(signal_node, on_call());
            }
            else {
                // TODO: log
                throw std::out_of_range(
                    zmbt::format("%s node not found at %s", signal_path(), signal_node).c_str()
                );
            }
        }
    }
    break;
    case Kind::Exception:
        // TODO: impl
        throw base_error("Exception clause not implemented");
        break;
    default:
        throw base_error("invalid injection kind");
        break;
    }
}

boost::json::value ChannelHandle::observe() const
{
    if (is_input())
    {
        throw model_error("calling observe on input channel");
    }
    auto ifc_handle = Environment::IfcRec(host(), interface());
    switch (kind())
    {
    case Kind::Return:
        {
            return get_at_pointer(ifc_handle.ObservedReturn(on_call()), signal_path());
        }
        break;
    case Kind::Args:
        {
            boost::json::string jp {format("/args%s", signal_path())};
            int start, stop, step;
            std::tie(start, stop, step) = call();
            boost::json::array arg_list = ifc_handle.CaptureSlice(jp, start, stop, step);
            if (!is_range() && arg_list.empty())
            {
                throw model_error("no captures for requested channel");
            }
            return is_range() ? arg_list : arg_list.at(0);
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
        throw base_error("Exception channel not implemented");
        break;
    default:
        throw base_error("implementation error");
        break;
    }
    return nullptr;
}

boost::json::array const& ChannelHandle::captures() const
{
    auto ifc_handle = Environment::IfcRec(host(), interface());
    return ifc_handle.Captures();
}

boost::json::value ChannelHandle::observe_join(std::list<ChannelHandle> channels)
{
    boost::json::array union_signal {};

    for (auto const& channel: channels)
    {
        union_signal.push_back(channel.observe());
    }

    return union_signal;
}

boost::json::value ChannelHandle::observe_series(std::list<ChannelHandle> channels)
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

        auto ifc_handle = Environment::IfcRec(channel.host(), channel.interface());
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
        // handle 1-based indexation
        if (start > 0) { start -= 1; }
        if (stop  > 0) { stop  -= 1; }
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
