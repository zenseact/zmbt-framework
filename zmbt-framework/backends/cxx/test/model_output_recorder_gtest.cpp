/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <thread>
#include <condition_variable>
#include <mutex>


#include <gtest/gtest.h>
#include <zenseact-mbt.hpp>

using namespace zmbt;
using namespace zmbt::api;
using namespace zmbt::api::expr;

TEST(OutputRecorderTest, StressTest)
{
    Environment env {};
    int const NofWorkers {8};
    int const NofIterations {2'000};
    int const ThrowEvery {20};

    std::atomic<std::size_t> counter{0U};
    std::atomic<int> ready_workers{0};
    std::mutex start_mutex;
    std::condition_variable start_cv;

    auto const test_fun = [](std::size_t x){ return x; };
    env.InitializeInterfaceHandlers(test_fun);
    OutputRecorder output_recorder_under_test{test_fun, env.DefaultObjectId(test_fun)};
    output_recorder_under_test.enable_category(ChannelKind::Args);
    output_recorder_under_test.enable_category(ChannelKind::Exception);
    output_recorder_under_test.enable_category(ChannelKind::Return);

    auto const task = [&](){
        {
            std::unique_lock<std::mutex> lk(start_mutex);
            ready_workers.fetch_add(1, std::memory_order_relaxed);
            start_cv.wait(lk, [&]{ return flags::TestIsRunning::status(); });
        }
        for(std::size_t i = 0; i < NofIterations; ++i)
        {
            counter.fetch_add(1, std::memory_order_relaxed);
            bool const should_throw = (i % ThrowEvery) == 0;
            auto const err = should_throw
                ? ErrorOr<std::size_t>::MakeError({"lol", "kek", "hehe"})
                : ErrorOr<std::size_t>::MakeValue(73);

            output_recorder_under_test.push(std::make_tuple<std::size_t>(42), err);
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < NofWorkers; ++i) threads.emplace_back(task);

    while (ready_workers.load(std::memory_order_acquire) < NofWorkers)
    {
        std::this_thread::yield();
    }

    {
        std::lock_guard<std::mutex> lk(start_mutex);
        zmbt::flags::TestIsRunning::set();
    }
    start_cv.notify_all();

    for(auto& thread: threads) thread.join();
    zmbt::flags::TestIsRunning::clear();

    auto const ItemsProduced = counter.load();


    EXPECT_EQ(ItemsProduced, NofWorkers * NofIterations);
    EXPECT_EQ(ItemsProduced, output_recorder_under_test.count());

    output_recorder_under_test.flush();

    EXPECT_EQ(ItemsProduced, output_recorder_under_test.data_frames().size());

    std::size_t prev_ts = 0;
    std::size_t err_count = 0;


    for (auto const& rec: output_recorder_under_test.data_frames())
    {
        auto const curr_ts = rec.at("ts").as_uint64();
        auto const arg = rec.at_pointer("/args/0").as_uint64();
        ASSERT_GE(curr_ts, prev_ts);
        ASSERT_EQ(arg, 42);
        prev_ts = curr_ts;

        if (rec.as_object().contains("exception"))
        {
            ++err_count;
            ASSERT_EQ(rec.at_pointer("/exception/type")   , "lol" );
            ASSERT_EQ(rec.at_pointer("/exception/what")   , "kek" );
            ASSERT_EQ(rec.at_pointer("/exception/context"), "hehe");
            ASSERT_FALSE(rec.as_object().contains("return"));
        }
        else
        {
            ASSERT_EQ(rec.at_pointer("/return").as_uint64(), 73);
            ASSERT_FALSE(rec.as_object().contains("exception"));
        }
    }

    ASSERT_EQ(ItemsProduced / ThrowEvery, err_count);
}
