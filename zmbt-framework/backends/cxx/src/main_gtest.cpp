/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <sstream>

#include <gtest/gtest.h>

#include <zenseact-mbt.hpp>

bool init_unit_test(int argc, char* argv[])
{
    auto env = zmbt::Environment();

    env.SetFailureHandler([](boost::json::value const& sts){
        std::stringstream ss;
        zmbt::format_failure_report(ss, sts);
        ADD_FAILURE() << ss.str();
    });

    bool prettify = false;
#ifndef _MSC_VER
    if (std::getenv("ZMBT_LOG_PRETTIFY"))
    {
        prettify = true;
    }
#endif
    for (int i = 0; i < argc; ++i)
    {
        if (0 == std::strcmp(argv[i], "--zmbt_log_prettify"))
        {
            prettify = true;
        }
    }
    env.SetPrettyPrint(prettify);
    return true;
}

// entry point:
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    init_unit_test(argc, argv);
    return RUN_ALL_TESTS();
}
