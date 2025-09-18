/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/output_capture.hpp"

namespace zmbt {

OutputCapture::OutputCapture()
    : registry_{}
{
}

OutputCapture::~OutputCapture()
{
    if (registry_ && registry_->lost_count.load())
    {
        ZMBT_LOG(WARNING) << "test system state leak detected"; // TODO: details
    }
}

std::size_t OutputCapture::consume_all(OutputCapture::consumer_fn_t const consume_fn)
{
    if (!registry_) return 0;
    registry_->serialization_in_progress.store(true);
    auto const n = registry_->extract_fn(*registry_, consume_fn);
    registry_->serialization_in_progress.store(false);
    return n;
}


void OutputCapture::flush()
{
    if (registry_ && flags::TestIsRunning::status())
    {
        // TODO: enable with delayed conversion
        consume_all([&arr_out=registry_->serialized_frames](boost::json::value&& v){
            arr_out.push_back(std::move(v));
        });
        ZMBT_LOG(TRACE) << object_id(this) << " is flushed";
    }
}


boost::json::array const& OutputCapture::data_frames() const
{
    if (!registry_)
    {
        throw_exception(environment_error("access to unregistered OutputCapture"));
    }
    return registry_->serialized_frames;
}

void OutputCapture::clear()
{
    if (registry_)
    {
        registry_->serialized_frames.clear();
        registry_->count.store(0);
    }
}


}
