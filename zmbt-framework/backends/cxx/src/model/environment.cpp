/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/environment.hpp"

#include <zmbt/core/json_pretty_print.hpp>
#include <zmbt/model/environment_data.hpp>
#include <zmbt/model/exceptions.hpp>
#include <zmbt/model/test_failure.hpp>
#include <zmbt/model/trigger.hpp>
#include <mutex>


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
}


void Environment::ResetInterfaceDataFor(object_id obj)
{
    auto lock = Lock();
    data_->json_data("/interface_records").as_object().erase(obj.str());
}


void Environment::ResetAll()
{
    ResetInterfaceData();
    auto lock = Lock();
    data_->json_data() = EnvironmentData::init_json_data();
    data_->shared.clear();
}


void Environment::ResetAllFor(object_id obj)
{
    ResetInterfaceDataFor(obj);
    auto lock = Lock();
    data_->json_data("/interface_records").as_object().erase(obj.str());
    data_->json_data("/vars").as_object().erase(obj.str());
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



Environment& Environment::RegisterInterface(object_id const& obj_id, interface_id const& ifc_id, boost::json::string_view key)
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

Environment& Environment::RegisterInterface(object_id const& obj_id, interface_id const& ifc_id)
{
    return RegisterInterface(obj_id, ifc_id, autokey(obj_id, ifc_id));
}


Environment& Environment::RegisterOperator(SignalOperatorHandler const& op, boost::json::string_view key)
{
    auto lock = Lock();
    data_->operators.emplace(key, op);
    return *this;
}



SignalOperatorHandler Environment::GetOperator(boost::json::string_view name) const
{
    auto lock = Lock();
    return data_->operators.at(name);
}

SignalOperatorHandler Environment::GetOperatorOrDefault(boost::json::string_view name) const
{
    auto lock = Lock();
    return data_->operators.count(name) ? data_->operators.at(name) : SignalOperatorHandler{};
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
        RegisterInterface(obj_id, ifc_id, key);
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


}  // namespace zmbt
