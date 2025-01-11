/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>
#include <zmbt/application/init_test.hpp>

bool init_unit_test()
{
    zmbt::application::InitZmbt(
        boost::unit_test::framework::master_test_suite().argc,
        boost::unit_test::framework::master_test_suite().argv
    );
    return true;
}

// entry point:
int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
