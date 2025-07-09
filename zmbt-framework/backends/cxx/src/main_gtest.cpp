/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <sstream>

#include <gtest/gtest.h>

#include <zenseact-mbt.hpp>


// entry point:
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    zmbt::InitZmbt(argc, argv);
    zmbt::Environment().SetFailureHandler([](boost::json::value const& sts){
        std::stringstream ss;
        zmbt::format_failure_report(ss, sts);
        ADD_FAILURE() << ss.str();
    });
    return RUN_ALL_TESTS();
}
