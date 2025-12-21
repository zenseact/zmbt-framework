#include <thread>

#include <gtest/gtest.h>
#include <zenseact-mbt.hpp>

using namespace zmbt::api;
using namespace zmbt::api::expr;

TEST(RunInGtest, EnvThreadSafety)
{
    Environment env {};
    int const kWorkers {8};
    int const kIterations {10000};

    auto test_fun = [](int x){ return x; };
    auto handle = InterfaceRecord(test_fun);
    handle.InjectArgs(Add(1), "/0");
    handle.InjectReturn(Add(1));

    auto task = [&](){

        for(int i = 0; i < kIterations; ++i)
        {
            static_cast<void>(handle.YieldInjectionArgs());
            static_cast<void>(handle.YieldInjectionReturn());
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < kWorkers; ++i) threads.emplace_back(task);
    for(auto& thread: threads) thread.join();

    EXPECT_EQ(kWorkers * kIterations + 1, handle.YieldInjectionArgs().as_array().front());
    EXPECT_EQ(kWorkers * kIterations + 1, handle.YieldInjectionReturn());
}


TEST(RunInGtest, SignalMapping)
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("SignalMapping test")
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        (2|Add(2)    , 4                                 ) ["Pipe on input"]
        ({42, 42, 42}, 42|Repeat(3)                      ) ["Pipe on output"]
        ("[42,42,42]", Parse & (42|Repeat(3)) | Eq       ) ["Nested Pipe"]
        ("1:5"|Arange, Reduce(Add) & Size | Div | Eq(2.5)) ["Complex example (computing average)"]
    ;


    Param const X {"X"};
    Param const Y {"Y"};

    SignalMapping("Parametric SignalMapping with X = %d, Y = %d", X, Y)
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        ((X, X, X)             , X|Repeat(3)                )
        ("[%s,%s]"|Format(X, Y), Parse | Any({13,0}, {19,1}))
    .Zip
        (X, 13, 19)
        (Y,  0,  1)
    ;

}