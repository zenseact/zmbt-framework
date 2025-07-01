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

    auto task = [&](){
        auto handle = InterfaceRecord(test_fun);

        for(int i = 0; i < kIterations; ++i)
        {
            auto lock = handle.Env().Lock();

            int arg_x = handle.YieldInjectionArgs().as_array()[0].as_int64();
            int ret_x = handle.YieldInjectionReturn().as_int64();

            handle.InjectArgs(boost::json::array{++arg_x});
            handle.InjectReturn(++ret_x);
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < kWorkers; ++i) threads.emplace_back(task);
    for(auto& thread: threads) thread.join();

    EXPECT_EQ(kWorkers * kIterations, InterfaceRecord(test_fun).YieldInjectionArgs().as_array()[0].as_int64());
    EXPECT_EQ(kWorkers * kIterations, InterfaceRecord(test_fun).YieldInjectionReturn().as_int64());
}


TEST(RunInGtest, SignalMapping)
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("SignalMapping test")
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        (2|Add(2)    , 4                                 ) ["Compose on input"]
        ({42, 42, 42}, 42|Repeat(3)                      ) ["Compose on output"]
        ("[42,42,42]", Parse & (42|Repeat(3)) | Eq       ) ["Nested Compose"]
        ("1:5"|Arange, Reduce(Add) & Size | Div | Eq(2.5)) ["Complex example (computing average)"]
    ;


    Param const X {"X"};
    Param const Y {"Y"};

    using List = boost::json::array;
    SignalMapping("Parametric SignalMapping with X = %d, Y = %d", X, Y)
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        (List{X, X, X}         , X|Repeat(3)                )
        ("[%s,%s]"|Format(X, Y), Parse | Any({13,0}, {19,1}))
    .Zip
        (X, 13, 19)
        (Y,  0,  1)
    ;

}