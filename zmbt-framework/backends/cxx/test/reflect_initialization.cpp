/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/reflect.hpp"

namespace {

class NoDefaultConstructor {
  public:
    NoDefaultConstructor() = delete;
    NoDefaultConstructor(int) {}
};

}


template<>
struct zmbt::reflect::custom_signal_traits<NoDefaultConstructor>
{
    static NoDefaultConstructor init()
    {
        return {0};
    }
};

BOOST_AUTO_TEST_CASE(NoDefault)
{
    BOOST_CHECK_NO_THROW(zmbt::reflect::signal_traits<NoDefaultConstructor>::init());
}
