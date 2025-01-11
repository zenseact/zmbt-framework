/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_ENVIRONMENT_DATA_HPP_
#define ZMBT_MODEL_ENVIRONMENT_DATA_HPP_


#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <mutex>
#include <typeindex>
#include <type_traits>


#include "signal_operator_handler.hpp"
#include "test_failure.hpp"
#include "trigger.hpp"


namespace zmbt {



struct EnvironmentData {

    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;

    using shared_data_record = std::pair<std::type_index, std::shared_ptr<void>>;
    using shared_data_table = std::map<boost::json::string, shared_data_record>;
    shared_data_table shared;


    using FailureHandler = std::function<void(boost::json::value const&)>;
    FailureHandler failure_handler {default_test_failure};


    static boost::json::value init_json_data()
    {
        return {
            {"interface_records", boost::json::object()},
            {"prototypes"       , boost::json::object()},
            {"vars"             , boost::json::object()},
            {"refs"             , boost::json::object()}
        };
    }
    JsonNode json_data {init_json_data()};

    std::map<boost::json::string, std::function<void()>> callbacks;
    std::map<boost::json::string, Trigger> triggers;
    std::map<interface_id, TriggerIfc> trigger_ifcs;
    std::map<object_id, TriggerObj> trigger_objs;
    std::map<boost::json::string, SignalOperatorHandler> operators;

    mutex_t mutable mutex;

    EnvironmentData()
    {
    }

    EnvironmentData(EnvironmentData &&o)
    {
        lock_t lock(o.mutex);
        shared = std::move(o.shared);
        json_data = std::move(o.json_data);
        callbacks = std::move(o.callbacks);
        operators = std::move(o.operators);
        failure_handler = std::move(o.failure_handler);
    }

    EnvironmentData& operator=(EnvironmentData &&o)
    {
        if (this != &o)
        {
            lock_t l_lock(mutex, std::defer_lock);
            lock_t r_lock(o.mutex, std::defer_lock);
            std::lock(l_lock, r_lock);
            shared = std::move(o.shared);
            json_data = std::move(o.json_data);
            callbacks = std::move(o.callbacks);
            operators = std::move(o.operators);
            failure_handler = std::move(o.failure_handler);
        }
        return *this;
    }

    EnvironmentData(EnvironmentData const& o)
    {
        lock_t lock(o.mutex);
        shared = o.shared;
        json_data = o.json_data;
        callbacks = o.callbacks;
        operators = o.operators;
        failure_handler = o.failure_handler;
    }

    EnvironmentData& operator=(EnvironmentData const& o)
    {
        if (this != &o)
        {
            lock_t l_lock(mutex, std::defer_lock);
            lock_t r_lock(o.mutex, std::defer_lock);
            std::lock(l_lock, r_lock);
            shared = o.shared;
            json_data = o.json_data;
            callbacks = o.callbacks;
            operators = o.operators;
            failure_handler = o.failure_handler;
        }
        return *this;
    }


    virtual ~EnvironmentData() {
    }
};



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_DATA_HPP_
