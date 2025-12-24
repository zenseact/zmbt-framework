/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

 #include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>


#include <gtest/gtest.h>
#include <zenseact-mbt.hpp>

using namespace zmbt;
using namespace zmbt::api;
using namespace zmbt::api::expr;
using namespace std::literals::chrono_literals;


TEST(StressTest, SignalMappingMultiThread)
{
    Environment env {};

    int const NofWorkers {8};
    int const NofIterations {2'000};

    std::atomic<std::size_t> consumed_count{0U};
    std::atomic<std::size_t> produced_count{0U};
    std::atomic<int> ready_workers{0};
    std::mutex start_mutex;
    std::atomic_bool start_flag{false};
    std::condition_variable start_cv;



    struct Mock {
        int produce() const {
            return InterfaceRecord(&Mock::produce, this).Hook();
        }
        void consume(int const x) {
            return InterfaceRecord(&Mock::consume, this).Hook(x);
        }
    } producer, consumer;

    auto const task = [&](){
        {
            std::unique_lock<std::mutex> lk(start_mutex);
            ready_workers.fetch_add(1, std::memory_order_relaxed);
            start_cv.wait(lk, [&]{ return start_flag.load(std::memory_order_relaxed); });
        }
        for(std::size_t i = 0; i < NofIterations; ++i)
        {
            auto const item = producer.produce();
            produced_count.fetch_add(1, std::memory_order_relaxed);
            if (item <= 0) break;
            std::this_thread::sleep_for(1ms);
            consumed_count.fetch_add(1, std::memory_order_relaxed);
            consumer.consume(item);
        }
    };

    auto const SUT = [&](){
        std::vector<std::thread> threads;
        for(int i = 0; i < NofWorkers; ++i) threads.emplace_back(task);

        while (ready_workers.load(std::memory_order_acquire) < NofWorkers)
        {
            std::this_thread::yield();
        }

        {
            std::lock_guard<std::mutex> lk(start_mutex);
            start_flag.store(true, std::memory_order_release);
        }
        start_cv.notify_all();

        for(auto& thread: threads) thread.join();
        start_flag.store(false, std::memory_order_release);
    };


    SignalMapping("Test SUT with multithreading")
    .OnTrigger(SUT)
        .At(&Mock::produce, producer).Inject(~Sub(1'000)) // xi = 1000 - i
        .At(&Mock::consume, consumer).Take(Max).Expect(1'000)
        .At(&Mock::consume, consumer).CallCount().Expect(1'000)
        // .At(&Mock::consume, producer).ThreadId().Expect(Card|8) # FIXME
        .At(&Mock::consume, consumer).ThreadId().Expect(Card|Le(8))
        ;

    EXPECT_LE(produced_count.load(), NofIterations * NofWorkers);
    EXPECT_LE(consumed_count.load(), 1'000);
}
