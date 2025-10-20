/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <chrono>

#include "zmbt/model/global_stats.hpp"
#include "zmbt/model/output_recorder.hpp"
#include "zmbt/model/environment.hpp"


namespace zmbt {

OutputRecorder::OutputRecorder(interface_id const& ifc_id, object_id const& obj_id)
    : ifc_id_{ifc_id}
    , obj_id_{obj_id}
    , registry_{}
{
}

OutputRecorder::~OutputRecorder()
{
    if (registry_ && registry_->lost_count.load())
    {
        ZMBT_LOG(WARNING)
        << "detected " << registry_->lost_count.load() << " lost calls on "
        << ifc_id_ << " : " << obj_id_;
        ;
    }
}



void OutputRecorder::flush()
{

    if (registry_)
    {
        auto const start = std::chrono::steady_clock::now();
        registry_->extract_fn(*registry_);
        flags::ConversionTime::add(std::chrono::steady_clock::now() - start);
    }
}


boost::json::array const& OutputRecorder::data_frames() const
{
    if (!registry_)
    {
        // not expected to be called in mock context
        throw_exception(output_recorder_error("access to unregistered OutputRecorder"));
    }
    return registry_->serialized_frames;
}

void OutputRecorder::clear()
{
    if (registry_)
    {
        registry_->frame_buff_map = std::make_shared<Registry::FramesBuffMap>();
        registry_->serialized_frames.clear();
        registry_->enable_categories_.reset();
        registry_->count.store(0);
    }
}

void OutputRecorder::enable_category(ChannelKind const ck)
{
    if (ensure_registry())
    {
        registry_->enable_categories_[static_cast<unsigned>(ck)] = 1;
    }
    else
    {
        // not expected to be called in mock context
        throw_exception(output_recorder_error("access to unregistered OutputRecorder on enable_category"));
    }
}

void OutputRecorder::report_test_error(ErrorInfo const& ei) const
{
    Environment().SetTestError({
        {"error"    , "unhandled exception in test record"},
        {"interface", registry_->interface_name           },
        {"context"  , ei.context                          },
        {"what"     , ei.what                             },
        {"type"     , ei.type                             },
    });
}

bool OutputRecorder::ensure_registry()
{
    if (registry_)
    {
        return true;
    }
    else
    {
        auto maybe_factory = Environment().permanent_data_->get_output_recorder_factory(ifc_id_);
        if (!maybe_factory.has_value())
        {
            return false;
        }
        else
        {
            maybe_factory.value().operator()(*this);
            return nullptr != registry_;
        }
    }
}

}
