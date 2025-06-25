/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <mutex>

#include "zmbt/model/environment.hpp"

#include <zmbt/core/json_pretty_print.hpp>
#include <zmbt/model/environment_data.hpp>
#include <zmbt/model/exceptions.hpp>
#include <zmbt/model/test_failure.hpp>
#include <zmbt/model/trigger.hpp>


namespace zmbt {



Environment::Environment()
{
    static std::mutex ctor_mt;
    std::lock_guard<std::mutex> guard(ctor_mt);

    static std::weak_ptr<EnvironmentData> instance;

    data_ = instance.lock();
    if (not data_)
    {
        data_ = std::make_shared<EnvironmentData>();
        instance = data_;
    }

    static std::shared_ptr<PersistentConfig> config;
    config_ = config;
    if (not config_)
    {
        config_ = std::make_shared<PersistentConfig>();
        config = config_;
    }
}

Environment::lock_t Environment::Lock() const
{
    return lock_t{data_->mutex};
}


Environment::lock_t Environment::TryLock() const
{
    return lock_t{data_->mutex, std::try_to_lock};
}


Environment::lock_t Environment::DeferLock() const
{
    return lock_t{data_->mutex, std::defer_lock};
}

void Environment::DumpJsonData(std::ostream &os)
{
    pretty_print(os, data_->json_data.node());
}



void Environment::ResetInterfaceData()
{
    auto lock = Lock();
    data_->json_data("/interface_records").as_object().clear();
    data_->input_generators.clear();

}


void Environment::ResetInterfaceDataFor(object_id obj)
{
    auto lock = Lock();
    data_->json_data("/interface_records").as_object().erase(obj.str());
    data_->input_generators[obj].clear();
}


void Environment::ResetAll()
{
    ResetInterfaceData();
    auto lock = Lock();
    data_->json_data() = EnvironmentData::init_json_data();
    data_->shared.clear();
    data_->input_generators.clear();
}


void Environment::ResetAllFor(object_id obj)
{
    ResetInterfaceDataFor(obj);
    auto lock = Lock();
    data_->json_data("/interface_records").as_object().erase(obj.str());
    data_->json_data("/vars").as_object().erase(obj.str());
    data_->input_generators[obj].clear();
}


boost::json::string Environment::GetOrRegisterParametricTrigger(object_id const& obj_id, interface_id const& ifc_id)
{
    bool const is_obj_ok = data_->trigger_objs.count(obj_id);
    bool const is_ifc_ok = data_->trigger_ifcs.count(ifc_id);
    bool const is_complete_trigger = data_->triggers.count(ifc_id.str());

    if (is_obj_ok && is_ifc_ok)
    {
        TriggerObj const& obj = data_->trigger_objs.at(obj_id);
        TriggerIfc const& ifc = data_->trigger_ifcs.at(ifc_id);
        Trigger trigger {obj, ifc};
        auto key = GetOrRegisterInterface(obj_id, ifc_id);
        auto lock = Lock();
        data_->triggers.emplace(key, std::move(trigger));
        data_->json_data("/triggers/%s", key) = 0; // TODO: timestamp
        return key;
    }
    else if (is_obj_ok && is_complete_trigger)
    {
        throw environment_error(
            "`%s` is registered as complete trigger and can't be used together object reference",
            ifc_id
        );
        return "";
    }
    else if (!is_obj_ok && is_ifc_ok)
    {
        throw environment_error(
            "`%s` is registered as interface and can't be used together object reference",
            ifc_id
        );
        return "";
    }
    else if (!is_obj_ok && is_ifc_ok)
    {
        throw environment_error(
            "%s object id is not found in registered triggers",
            obj_id
        );
        return "";
    }
    else
    {
        throw environment_error(
            "Can't resolve registered trigger for (%s, %s)", obj_id, ifc_id
        );
        return "";
    }

}



bool Environment::HasTrigger(boost::json::string_view key) const
{
    auto lock = Lock();
    return data_->triggers.count(key) > 0;
}


bool Environment::HasAction(boost::json::string_view key) const
{
    auto lock = Lock();
    return data_->callbacks.count(key) > 0;
}



Environment& Environment::RegisterInterface(boost::json::string_view key, interface_id const& ifc_id, object_id const& obj_id)
{

    JsonNode refs_ids =  data_->json_data.branch("/refs/key2ids/%s", key);
    JsonNode refs_key =  data_->json_data.branch("/refs/ids2key/%s/%s", obj_id, ifc_id);

    boost::json::string_view errmsg {"register failure: node \"%s\" is not empty"};
    if (!refs_key.is_null() && (refs_key.node() != key))
    {
        throw environment_error("RegisterInterface failure: `%s` is already registered as %s", key, refs_key.node());
    }
    if (!refs_ids.is_null() && ((refs_ids.at("/obj") != obj_id) || (refs_ids.at("/ifc") != ifc_id)))
    {
        throw environment_error(
            "RegisterInterface failure: `%s` is already registered for other interface", key
        );
    }

    refs_ids("/obj") = obj_id;
    refs_ids("/ifc") = ifc_id;
    refs_key() = key;
    return *this;
}


Environment::PersistentConfig Environment::Config() const
{
    return *config_;
}

Environment& Environment::SetPrettyPrint(bool const pretty_print)
{
    config_->pretty_print = pretty_print;
    return *this;
}

Environment& Environment::SetFailureHandler(std::function<void(boost::json::value const&)> const& fn)
{
    config_->failure_handler = fn;
    return *this;
}

Environment& Environment::ResetFailureHandler()
{
    config_->failure_handler = default_test_failure;
    return *this;
}

Environment& Environment::HandleTestFailure(boost::json::value const& diagnostics)
{
    config_->failure_handler(diagnostics);
    return *this;
}


object_id Environment::ObjectId(boost::json::string_view interface_key) const
{
    auto lock = Lock();
    return object_id{json_data().at("/refs/key2ids/%s/obj", interface_key)};
}

interface_id Environment::InterfaceId(boost::json::string_view interface_key) const
{
    auto lock = Lock();
    return interface_id{json_data().at("/refs/key2ids/%s/ifc", interface_key)};
}

boost::json::string Environment::GetOrRegisterInterface(object_id const& obj_id, interface_id const& ifc_id)
{
    auto ptr = format("/refs/ids2key/%s/%s", obj_id, ifc_id);
    auto lock = Lock();
    if (!json_data().contains(ptr))
    {
        auto key = autokey(obj_id, ifc_id);
        RegisterInterface(key, ifc_id, obj_id);
        return key;
    }
    else {
        return json_data().at(ptr).as_string();
    }
}

object_id Environment::DefaultObjectId(interface_id const& ifc_id) const
{
    auto lock = Lock();
    return object_id{json_data().at("/refs/defobj/%s", ifc_id)};
}


void Environment::SetVar(lang::Expression const& key_expr, boost::json::value var)
{
    auto const key = format("/vars/%s", key_expr.eval());
    auto lock = Lock();
    data_->json_data(key) = var;
}

boost::json::value Environment::GetVar(lang::Expression const& key_expr)
{
    auto const key = format("/vars/%s", key_expr.eval());
    auto lock = Lock();
    if (auto const ptr = data_->json_data.find_pointer(key))
    {
        return *ptr;
    }
    else
    {
        throw environment_error("Environment variable `%s` not found", key);
        return nullptr;
    }
}

boost::json::value Environment::GetVarOrUpdate(lang::Expression const& key_expr, boost::json::value update_value)
{
    auto const key = format("/vars/%s", key_expr.eval());
    auto lock = Lock();
    if (auto const ptr = data_->json_data.find_pointer(key))
    {
        return *ptr;
    }
    else
    {
        data_->json_data(key) = update_value;
        return update_value;
    }
}

boost::json::value Environment::GetVarOrDefault(lang::Expression const& key_expr, boost::json::value default_value)
{
    auto const key = format("/vars/%s", key_expr.eval());
    auto lock = Lock();
    if (auto const ptr = data_->json_data.find_pointer(key))
    {
        return *ptr;
    }
    else
    {
        return default_value;
    }
}

bool Environment::ContainsShared(lang::Expression const& key_expr) const
{
    boost::json::string key = key_expr.eval().as_string();
    auto lock = Lock();
    return data_->shared.count(key) > 0;
}

Environment& Environment::RegisterAction(lang::Expression const& key_expr, std::function<void()> action)
{
    boost::json::string const key = key_expr.eval().as_string();
    auto lock = Lock();
    if (data_->callbacks.count(key))
    {
        throw environment_error("Callback registering failed: key \"%s\" already exists", key);
    }
    data_->callbacks.emplace(key, action);
    data_->json_data("/callbacks/%s", key) = 0; // TODO: timestamp
    return *this;
}


Environment& Environment::RunAction(lang::Expression const& key_expr)
{
    boost::json::string const key = key_expr.eval().as_string();
    if (0 == data_->callbacks.count(key))
    {
        throw environment_error("Callback execution failed: %s is not registered", key);
    }

    try
    {
        data_->callbacks.at(key).operator()();
    }
    catch(const std::exception& e)
    {
        throw environment_error("Callback execution failed: %s throws %s", key, e.what());
    }
    return *this;
}

}  // namespace zmbt
