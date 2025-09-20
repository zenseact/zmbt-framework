/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_ENVIRONMENT_DATA_HPP_
#define ZMBT_MODEL_ENVIRONMENT_DATA_HPP_

#include <atomic>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <mutex>
#include <typeindex>
#include <type_traits>
#include <boost/unordered/concurrent_flat_map.hpp>


#include "trigger.hpp"
#include "generator.hpp"
#include "channel_kind.hpp"
#include "output_recorder.hpp"


namespace zmbt {




/// Data container for the Environment
struct EnvironmentData final {

    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;

    using shared_data_record = std::pair<std::type_index, std::shared_ptr<void>>;
    using shared_data_table = std::map<boost::json::string, shared_data_record>;
    using FailureHandler = std::function<void(boost::json::value const&)>;


    static boost::json::value init_json_data();

    shared_data_table shared;
    JsonNode json_data {init_json_data()};

    std::map<boost::json::string, std::function<void()>> callbacks;
    std::map<boost::json::string, Trigger> triggers;
    std::map<interface_id, TriggerIfc> trigger_ifcs;
    std::map<object_id, TriggerObj> trigger_objs;


    struct InputRecord
    {
        boost::json::string jptr;
        Generator::Shared generator;
        lang::Expression transform;
    };

    using InputRecordList = std::vector<InputRecord>;
    /// Args, Return, Exception
    using InjectionTable = std::array<InputRecordList, 3U>;

    boost::concurrent_flat_map<
        std::pair<interface_id,object_id>,
            InjectionTable> injection_tables{};

    boost::concurrent_flat_map<
        std::pair<interface_id,object_id>,
            std::shared_ptr<OutputRecorder>> output_recorders{};

    std::atomic_bool has_test_error{false};


    mutex_t mutable mutex;

    EnvironmentData();

    EnvironmentData(EnvironmentData &&o) = delete;
    EnvironmentData(EnvironmentData const& o) = delete;
    EnvironmentData& operator=(EnvironmentData &&o) = delete;
    EnvironmentData& operator=(EnvironmentData const& o) = delete;

    ~EnvironmentData();
};



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_DATA_HPP_
