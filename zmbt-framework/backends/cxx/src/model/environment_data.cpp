/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/environment_data.hpp"


namespace zmbt {

boost::json::value EnvironmentData::init_json_data()
{
    return {
        {"interface_records", boost::json::object()},
        {"prototypes"       , boost::json::object()},
        {"vars"             , boost::json::object()},
        {"refs"             , boost::json::object()}
    };
}

EnvironmentData::EnvironmentData() {}

EnvironmentData::~EnvironmentData() {}

EnvironmentData::EnvironmentData(EnvironmentData &&o)
{
    lock_t lock(o.mutex);
    shared = std::move(o.shared);
    json_data = std::move(o.json_data);
    callbacks = std::move(o.callbacks);
    failure_handler = std::move(o.failure_handler);
}

EnvironmentData& EnvironmentData::operator=(EnvironmentData &&o)
{
    if (this != &o)
    {
        lock_t l_lock(mutex, std::defer_lock);
        lock_t r_lock(o.mutex, std::defer_lock);
        std::lock(l_lock, r_lock);
        shared = std::move(o.shared);
        json_data = std::move(o.json_data);
        callbacks = std::move(o.callbacks);
        failure_handler = std::move(o.failure_handler);
    }
    return *this;
}

EnvironmentData::EnvironmentData(EnvironmentData const& o)
{
    lock_t lock(o.mutex);
    shared = o.shared;
    json_data = o.json_data;
    callbacks = o.callbacks;
    failure_handler = o.failure_handler;
}

EnvironmentData& EnvironmentData::operator=(EnvironmentData const& o)
{
    if (this != &o)
    {
        lock_t l_lock(mutex, std::defer_lock);
        lock_t r_lock(o.mutex, std::defer_lock);
        std::lock(l_lock, r_lock);
        shared = o.shared;
        json_data = o.json_data;
        callbacks = o.callbacks;
        failure_handler = o.failure_handler;
    }
    return *this;
}

}  // namespace zmbt
