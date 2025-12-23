/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>

#include "zmbt/model/environment_interface_record.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/mapping/pipe_handle.hpp"



namespace zmbt {
namespace mapping {


PipeHandle::PipeHandle(JsonNode& model, std::size_t const pipe_idx)
    : data_(model.branch("/pipes/%d", pipe_idx))
{
    auto const repeat_is_set = model.contains("/repeat_trigger");
    auto const N = data_.at("/channels").as_array().size();

    Environment env{};
    auto const& trigger_ref = model.at("/trigger").as_string();
    auto const trigger_ifc = env.InterfaceId(trigger_ref);
    auto const trigger_obj = env.ObjectId(trigger_ref);

    auto const get_should_flatten = [trigger_ifc, trigger_obj, repeat_is_set](ChannelHandle const& cnl) -> bool {
        if (cnl.kind() == ChannelKind::CallCount)
            return true;
        return !repeat_is_set && (
            (cnl.host() == trigger_obj) && (cnl.interface() == trigger_ifc)
        );
    };

    bool should_flatten{true};
    for (std::size_t i = 0; i < N; i++)
    {
        ChannelHandle cnl{model, format("%s/channels/%d", data_.node_ptr(), i)};
        if (should_flatten && !get_should_flatten(cnl))
        {
            should_flatten = false;
        }
        channels_.push_back(std::move(cnl));
    }
    should_flatten_ = should_flatten;
}


boost::json::value PipeHandle::type() const
{
    return data_.get_or_default("/type", nullptr);
}


lang::Expression PipeHandle::expression() const
{
    lang::Expression e = expr::Noop;

    if (auto const p =  data_.find_pointer("/expr"))
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


bool PipeHandle::overload(lang::Expression& e) const
{
    if (data_.contains("/overload"))
    {
        auto const& op_ref = data_.at("/overload").as_string();
        bool const shall_decorate_result = (e.is_const() && !e.is_boolean());
        e = expr::Overload(op_ref, e);
        if (shall_decorate_result)
        {
            e = e | expr::Cast(op_ref);
        }
        return true;
    }
    return false;
}

void PipeHandle::inject(lang::Expression e) const
{
    if (e.is_noop()) return;
    overload(e);

    bool const is_blend = type() == "blend";

    if (is_blend)
    {
        auto generator = std::make_shared<Generator>(e);
        for (auto const& channel: channels_)
        {
            auto handle = Environment::InterfaceHandle(channel.interface(), channel.host());
            handle.Inject(generator, channel.transform(), channel.kind(), channel.signal_path());
        }
    }
    else
    {
        for (auto const& channel: channels_)
        {
            auto handle = Environment::InterfaceHandle(channel.interface(), channel.host());
            handle.Inject(std::make_shared<Generator>(e), channel.transform(), channel.kind(), channel.signal_path());
        }
    }


}


boost::json::value PipeHandle::observe() const
{
    if (!is_output())
    {
        throw_exception(model_error("calling observe on non-output channel"));
    }
    boost::json::value result;
    auto const pipe_type = type();

    if ("group" == pipe_type)
    {
        result.emplace_array();
        auto& group_result = result.get_array();
        for (auto const& channel: channels_)
        {
            auto const captures = channel.captures();
            auto const tf = channel.transform();

            auto o = should_flatten_ ? captures.at(0) : captures;
            o = tf.is_identity() ? o : tf.eval(o);
            group_result.push_back(o);
        }
    }
    else if ("blend" == pipe_type)
    {
        result = observe_blend();
    }
    else
    {
        auto const captures = channels_.back().captures();
        auto const tf = channels_.back().transform();

        result = should_flatten_ ? captures.at(0) : captures;
        result = tf.is_identity() ? result : tf.eval(result);
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
        auto const& alias = channel.tag();
        auto ifc_handle = Environment::InterfaceHandle(channel.interface(), channel.host());
        auto const& captures = ifc_handle.Captures();

        if (channel.kind() == ChannelKind::CallCount)
        {
            boost::json::array record {
                alias,
                ifc_handle.ObservedCalls(),
                0UL // With zero timestamp, ObservedCalls are joined in order of channel definition
            };
            join_captures.insert(
                std::upper_bound(join_captures.begin(), join_captures.end(), record,  sort_pred),
                record
            );
            continue;
        }

        auto const& full_path = channel.full_path();


        auto insert_begin = join_captures.begin();

        auto const tf = channel.transform();


        for (auto const& capture: captures)
        {
            boost::json::value signal_value;
            boost::system::error_code ec;

            if (auto const p = capture.find_pointer(full_path, ec))
            {

                signal_value = tf.is_identity() ? *p : tf.eval(*p);
            }

            boost::json::array record {
                alias,
                signal_value,
                capture.at_pointer("/ts")
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
    return data_.get_or_default("/column", -1).as_int64();
}


boost::json::value PipeHandle::index() const
{
    return data_.at("/index");
}

boost::json::value PipeHandle::id() const
{
    if (id_cache_.is_null())
    {
        auto const idx = index();
        auto const id_expr = lang::Expression(data_.get_or_default("/id", nullptr));
        auto const id = id_expr.eval({});
        if (id.is_null())
        {
            id_cache_ = idx;
        }
        else
        {
            id_cache_ = format("#%d: %s",idx, id);
        }
    }
    return id_cache_;
}


} // namespace mapping
} // namespace zmbt
