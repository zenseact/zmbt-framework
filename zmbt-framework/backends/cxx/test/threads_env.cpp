#include <thread>

#include <gtest/gtest.h>

#include "zmbt/model.hpp"

using namespace zmbt;


TEST(ModelEnvironment, ThreadSafety)
{
    Environment env {};
    int const kWorkers {8};
    int const kIterations {10000};

    auto test_fun = [](int x){ return x; };

    auto task = [&](){
        auto handle = InterfaceRecord(test_fun);

        for(int i = 0; i < kIterations; ++i)
        {
            auto lock = handle.Env().Lock();

            int arg_x = handle.GetInjectionArgs()[0].as_int64();
            int ret_x = handle.GetInjectionReturn().as_int64();

            handle.InjectArgs({++arg_x});
            handle.InjectReturn(++ret_x);
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < kWorkers; ++i) threads.emplace_back(task);
    for(auto& thread: threads) thread.join();

    EXPECT_EQ(kWorkers * kIterations, InterfaceRecord(test_fun).GetInjectionArgs()[0].as_int64());
    EXPECT_EQ(kWorkers * kIterations, InterfaceRecord(test_fun).GetInjectionReturn().as_int64());
}
