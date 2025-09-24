/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/permanent_data.hpp"


namespace zmbt {

boost::optional<reflect::Prototypes> PermanentEnvData::get_prototypes(interface_id const& id) const
{
    boost::optional<reflect::Prototypes> result{};
    prototypes.visit(id, [&result](auto& record){
        result = record.second;
    });
    return result;
}

boost::optional<object_id> PermanentEnvData::get_default_object(interface_id const& id) const
{
    boost::optional<object_id> result{};
    default_objects.visit(id, [&result](auto& record){
        result = record.second;
    });
    return result;
}

boost::optional<PermanentEnvData::SetupOutputRecorder> PermanentEnvData::get_output_recorder_factory(interface_id const& id) const
{
    boost::optional<SetupOutputRecorder> result{};
    output_recorder_factories.visit(id, [&result](auto& record){
        result = record.second;
    });
    return result;
}

} // namespace zmbt
