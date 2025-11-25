/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <zenseact-mbt.hpp>

// Called by boost::unit_test::unit_test_main before running tests.
boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    zmbt::InitZmbt(argc, argv);
    zmbt::Config().SetFailureHandler([](boost::json::value const& sts){
        std::stringstream ss;
        zmbt::format_failure_report(ss, sts);
        BOOST_ERROR(ss.str());
    });
    return nullptr;  // use the default master test suite
}

// entry point:
int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);
}
