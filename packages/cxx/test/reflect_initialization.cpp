/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
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
struct zmbt::reflect::custom_initialization<NoDefaultConstructor>
{
    static NoDefaultConstructor init()
    {
        return {0};
    }
};

BOOST_AUTO_TEST_CASE(NoDefault)
{
    BOOST_CHECK_NO_THROW(zmbt::reflect::initialization<NoDefaultConstructor>::init());
}
