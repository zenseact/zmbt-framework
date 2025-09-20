/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/output_recorder.hpp"
#include "zmbt/model/environment.hpp"


namespace zmbt {

OutputRecorder::OutputRecorder()
    : registry_{}
{
}

OutputRecorder::~OutputRecorder()
{
    if (registry_ && registry_->lost_count.load())
    {
        ZMBT_LOG(WARNING) << "test system state leak detected"; // TODO: details
    }
}

std::size_t OutputRecorder::consume_all(OutputRecorder::consumer_fn_t const consume_fn)
{
    if (!registry_) return 0;
    auto const n = registry_->extract_fn(*registry_, consume_fn);
    return n;
}


void OutputRecorder::flush()
{
    if (registry_ && flags::TestIsRunning::status())
    {
        // TODO: enable with delayed conversion
        consume_all([&arr_out=registry_->serialized_frames](boost::json::value&& v){
            arr_out.push_back(std::move(v));
        });
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
        registry_->serialized_frames.clear();
        registry_->enable_categories_.reset();
        registry_->count.store(0);
    }
}

void OutputRecorder::enable_category(ChannelKind const ck)
{
    if (registry_)
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

}
