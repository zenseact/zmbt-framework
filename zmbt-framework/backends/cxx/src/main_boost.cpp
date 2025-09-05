/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <sstream>

#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

#include <zenseact-mbt.hpp>

bool init_unit_test()
{
    auto const& master_suite = boost::unit_test::framework::master_test_suite();
    zmbt::InitZmbt(master_suite.argc, master_suite.argv);
    zmbt::Config().SetFailureHandler([](boost::json::value const& sts){
        std::stringstream ss;
        zmbt::format_failure_report(ss, sts);
        BOOST_ERROR(ss.str());
    });
    return true;
}

// entry point:
int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main(&init_unit_test, argc, argv );
}
