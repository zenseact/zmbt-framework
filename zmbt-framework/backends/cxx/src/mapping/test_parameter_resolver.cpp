/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <cstddef>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/json.hpp>

#include <zmbt/application.hpp>
#include <zmbt/core.hpp>
#include <zmbt/model.hpp>
#include <zmbt/expr.hpp>

#include "zmbt/mapping/test_parameter_resolver.hpp"


namespace zmbt {
namespace mapping {



void TestParameterResolver::resolve_deferred_params(JsonNode& next_model)
try
{

    auto& name = next_model("/name");
    auto& description = next_model("/description");
    auto& trigger = next_model("/trigger");

    name = lang::Expression(name).eval();
    description = lang::Expression(description).eval();
    trigger = lang::Expression(trigger).eval();

    interface_id trig_ifc;
    object_id    trig_obj;

    if (trigger.is_string())
    {
        trig_ifc = env.InterfaceId(trigger.get_string());
        trig_obj = env.ObjectId(trigger.get_string());
    }
    else if (trigger.is_object() && trigger.get_object().contains("ifc"))
    {
        trig_ifc = interface_id (trigger.as_object().at("ifc"));
        trig_obj = object_id    (trigger.as_object().at("obj"));
        trigger = env.GetOrRegisterParametricTrigger(trig_obj, trig_ifc);
    }
    else{
        throw_exception(model_error("invalid trigger: %s", trigger));
    }

    for(auto& pipe: next_model("/pipes").as_array())
    {
        auto const& role = pipe.as_object().at("role");
        for (auto& channel: pipe.as_object()["channels"].as_array())
        {
            auto& kind        = channel.as_object().at("kind");
            auto& interface   = channel.as_object().at("interface");
            auto& signal_path = channel.as_object().at("signal_path");

            interface   = lang::Expression(interface).eval();
            signal_path = lang::Expression(signal_path).eval();

            if (signal_path.is_number())
            {
                signal_path = format("/%d", signal_path);
            }

            interface_id ifc_id;

            if (interface.is_string())
            {
                ifc_id = env.InterfaceId(interface.get_string());
            }
            else if (interface.is_object())
            {
                ifc_id = interface_id(interface.as_object().at("ifc"));
                auto const& obj_param = interface.as_object().at("obj");
                object_id obj_id;
                if (obj_param == "$(default)") {
                    obj_id = (ifc_id == trig_ifc) ? trig_obj : env.DefaultObjectId(ifc_id);
                } else {
                    obj_id = object_id{lang::Expression(obj_param).eval()};
                }

                interface = env.GetOrRegisterInterface(obj_id, ifc_id);
            }
            else{
                throw_exception(model_error("invalid interface: %s", interface));
            }


            if (kind == "$(default)")
            {
                bool const on_trigger = trigger == interface;
                bool const is_inject = role == "inject";
                if (is_inject) {
                    kind = on_trigger ? "args" : "return";
                } else {
                    kind = on_trigger ? "return" : "args";
                }
            }

            if (signal_path == "$(default)")
            {
                bool const is_unary = env.GetPrototypes(ifc_id).args().size() == 1;
                signal_path = (is_unary && (kind == "args")) ? "/0" : "";
            }
            else if (!signal_path.is_string())
            {
                throw_exception(model_error("invalid signal_path: %s", signal_path));
            }
        }
    }
}
catch (std::exception const& e)
{
    log_debug(next_model);
    throw_exception(model_error("Resolving model parameters failed with `%s`", e.what()));
}



void TestParameterResolver::log_debug(JsonNode next_model)
{
    env.DumpToJsonLog();
    ZMBT_LOG_JSON(DEBUG) << boost::json::object{
        {"PROTO MODEL",  model_.node()},
        {"EVALUATED MODEL",  next_model.node()},
    };
}


void TestParameterResolver::init_param_iters()
{
    auto const& model_param_groups = model_.at("/param_groups").as_array();
    size_t const group_count = model_param_groups.size();
    std::vector<boost::json::array> parameter_groups(group_count);
    std::vector<boost::json::string_view> param_names;


    for (auto const& kv: model_.at("/params").as_object())
    {
        auto group_it = parameter_groups.begin();
        param_names.push_back(kv.key());
        for (auto const& value_group: model_.at("/params/%s/value_groups", kv.key()).as_array())
        {
            group_it->push_back(value_group);
            group_it++;
        }
    }

    for (std::size_t grp_idx = 0; grp_idx < group_count; grp_idx++)
    {
        if (model_param_groups.at(grp_idx) == "Prod")
        {
            iterators_.push_back(
                std::make_shared<JsonProdIter>(std::move(parameter_groups.at(grp_idx)))
            );
            continue;
        }
        auto& p_group = parameter_groups.at(grp_idx);
        std::vector<std::size_t> continuation_parameters;
        std::size_t zip_length {0};
        for (std::size_t i = 0; i < p_group.size(); i++)
        {
            std::size_t const N = p_group.at(i).as_array().size();
            if (N == 1)
            {
                continuation_parameters.push_back(i);
            }
            else if (zip_length && (N != zip_length)) {
                throw_exception(model_error("inconsistent zip parameters shape on parameter %s", param_names.at(i)));
            }
            else
            {
                zip_length = N;
            }
        }
        // TODO: handle this in zip iterator to avoid repeated value allocation
        if (continuation_parameters.size() < p_group.size())
        {
            for (std::size_t const i: continuation_parameters)
            {
                boost::json::array continue_param (zip_length);
                std::fill(continue_param.begin(), continue_param.end(), p_group[i].as_array().at(0));
                p_group[i] = continue_param;
            }
        }
        iterators_.push_back(
            std::make_shared<JsonZipIter>(std::move(parameter_groups.at(grp_idx)))
        );
    }
}



TestParameterResolver::TestParameterResolver(JsonNode const& model) : model_(model)
{
    if (model_.at("/param_groups").as_array().size())
    {
        init_param_iters();
    }
    else
    {
        iterators_.push_back(std::make_shared<JsonZipIter>()); // empty zip for invariant model
    }
}


JsonNode TestParameterResolver::Next()
{
    if (iterators_.empty())
    {
        return JsonNode{};
    }

    auto iter = iterators_.back();
    if (iter->halt())
    {
        iterators_.pop_back();
        return Next();
    }


    JsonNode next_model {model_.copy()};

    boost::json::array const param_values = iter->get();
    boost::json::object const& params = model_.at("/params").as_object();

    auto param_it = params.cbegin();
    auto value_it =  param_values.cbegin();

    while (param_it != params.cend())
    {
        boost::json::array const& p_pointers = param_it->value().as_object().at("pointers").as_array();
        for (boost::json::value const& ptr: p_pointers)
        {
            try
            {
                next_model(ptr.as_string()) = *value_it;
            }
            catch(const std::exception& e)
            {
                throw_exception(model_error("failed to resolve parameter on %s, error:", ptr, e.what()));
            }
        }

        value_it++;
        param_it++;
    }

    resolve_deferred_params(next_model);
    next_model.as_object().erase("params");
    next_model.as_object().erase("param_groups");
    (*iter)++;
    return next_model;
}


} // namespace mapping
} // namespace zmbt
