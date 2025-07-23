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


#include "test_failure.hpp"
#include "trigger.hpp"
#include "generator.hpp"
#include "channel_kind.hpp"


namespace zmbt {




/// Data container for the Environment
struct EnvironmentData {

    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;

    using shared_data_record = std::pair<std::type_index, std::shared_ptr<void>>;
    using shared_data_table = std::map<boost::json::string, shared_data_record>;
    using FailureHandler = std::function<void(boost::json::value const&)>;

    using GeneratorsTable = std::map<boost::json::string,    // jptr
        std::pair<Generator::Shared, lang::Expression>>;


    static boost::json::value init_json_data();

    shared_data_table shared;
    FailureHandler failure_handler {default_test_failure};
    JsonNode json_data {init_json_data()};

    std::map<boost::json::string, std::function<void()>> callbacks;
    std::map<boost::json::string, Trigger> triggers;
    std::map<interface_id, TriggerIfc> trigger_ifcs;
    std::map<object_id, TriggerObj> trigger_objs;
    std::map<object_id,                  // ifc
        std::map<interface_id,           // obj
        std::map<ChannelKind,            // grp
        GeneratorsTable>>> input_generators;


    mutex_t mutable mutex;

    EnvironmentData();

    EnvironmentData(EnvironmentData &&o);

    EnvironmentData& operator=(EnvironmentData &&o);

    EnvironmentData(EnvironmentData const& o);

    EnvironmentData& operator=(EnvironmentData const& o);

    virtual ~EnvironmentData();
};



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_DATA_HPP_
