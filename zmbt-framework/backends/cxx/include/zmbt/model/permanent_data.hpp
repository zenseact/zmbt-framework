/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_PERMANENT_DATA_HPP_
#define ZMBT_MODEL_PERMANENT_DATA_HPP_

#include <functional>
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK
#include <boost/unordered/concurrent_flat_map.hpp>
#include <boost/optional.hpp>



#include "zmbt/core.hpp"
#include "zmbt/reflect/prototypes.hpp"
#include "output_recorder.hpp"


namespace zmbt {

struct PermanentEnvData final
{
    using SetupOutputRecorder = std::function<void(OutputRecorder&)>;

    boost::concurrent_flat_map<interface_id, object_id> default_objects{};
    boost::concurrent_flat_map<interface_id, reflect::Prototypes> prototypes{};
    boost::concurrent_flat_map<interface_id, SetupOutputRecorder> output_recorder_factories{};

    boost::optional<object_id>             get_default_object(interface_id const&) const;
    boost::optional<reflect::Prototypes>   get_prototypes(interface_id const&) const;
    boost::optional<SetupOutputRecorder>   get_output_recorder_factory(interface_id const&) const;
};

} // namespace zmbt

#endif