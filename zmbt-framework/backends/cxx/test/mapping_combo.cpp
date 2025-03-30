/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt::expr;
using namespace zmbt;


BOOST_AUTO_TEST_CASE(TestWith)
{
    auto test_pair = [](int x, int y){ return std::make_pair(x, y); };

    SignalMapping("With clause test")
    .OnTrigger(test_pair)

        .InjectTo  (test_pair).Args(0)
        .InjectTo  (test_pair).Args(1)

        .ObserveOn (test_pair) .Return()

        .ObserveOn (test_pair) .Return(1)
            .With()
        .ObserveOn (test_pair) .Return(0)

    .Test
        (  _ ,  _ ,  Size|2  ,  Size|2  )
        (  0 ,  0 , { 0,  0} , { 0,  0} )
        ( 42 , 13 , {42, 13} , {13, 42} )
        ( 42 , 13 , Gt       , Lt       )
    ;


    using test_map_t = std::map<int, int>;
    auto test_map = [](test_map_t const& map){ return map; };

    SignalMapping("With clause test")
    .OnTrigger(test_map)

        .InjectTo  (test_map)
        .ObserveOn (test_map).Return("/0/1")
            .With()
        .ObserveOn (test_map).Return("/1/1")

    .Test
        (_             ,  _   )
        ({{1,2}, {3,4}}, {2,4})
    ;
}


BOOST_AUTO_TEST_CASE(TestUnion)
{
    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    };

    auto test = [](std::list<int> nums){
        Mock mock{};
        bool flip {true};
        while (!nums.empty())
        {
            int x = nums.front();
            nums.pop_front();
            flip ? mock.foo(x) : mock.bar(x);
            flip = !flip;
        }
    };

    using boost::json::array;

    SignalMapping("Test series with default clause")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).Alias("f")
            .Union()
        .ObserveOn (&Mock::bar).Alias("b")

    .Test
        ({1,2,3,4}, array{{"f", 3}, {"b", 4}})
    ;

    SignalMapping("Test series with range")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).CallRange().Alias("f")
            .Union()
        .ObserveOn (&Mock::bar).CallRange().Alias("b")

    .Test
        ({1,2,3,4}, array{{"f", 1}, {"b", 2}, {"f", 3}, {"b", 4}})
        ({1,2,3,4}, Saturate({"f", 1}, {"b", 2}, {"f", 3}, {"b", 4}))
    ;

    SignalMapping("Test series with CallCount clause")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).CallCount().Alias("f")
            .Union()
        .ObserveOn (&Mock::bar).CallCount().Alias("b")

    .Test
        ({1,2,3,4,5,6,7}, array{{"b", 3}, {"f", 4}}) ["f is called last"]
    ;
}
