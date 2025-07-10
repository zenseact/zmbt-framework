/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zenseact-mbt.hpp"

using namespace zmbt::api;
using namespace zmbt;

namespace sut {
struct Foo {};
struct Bar : public Foo {};

// This function depends on ADL for tag_invoke.
// Depending on how the SUT is built and linked,
// it may be unintentionally affected by test-only adapters.
template <class T>
boost::json::value test_conversion(T&& t)
{
    return boost::json::value_from(std::forward<T>(t));
}

// SUT defines template serialization
template <class T>
std::enable_if_t<std::is_base_of<Foo, T>::value>
tag_invoke(boost::json::value_from_tag, boost::json::value& v, T const&)
{
    v = zmbt::type_name<T>();
}
}

#ifdef TEST_ADL_POLLUTION
    // Example of test-specific serialization for Bar,
    // likely not intended not be visible in SUT lookup
    namespace my_test {
    void tag_invoke(boost::json::value_from_tag, boost::json::value& v, sut::Bar const& t)
    {
        v = "Lol";
    }
    }
    // Inject the test-specific serialization into the SUT namespace
    namespace sut { using my_test::tag_invoke; }
#else
    // Hermetic test-only serialization not affecting the SUT
    template <>
    struct zmbt::reflect::custom_serialization<sut::Bar>
    {
        static boost::json::value json_from(sut::Bar const&)
        {
            return "Lol";
        }
    };
#endif

BOOST_AUTO_TEST_CASE(AdlPollution)
{
    BOOST_CHECK_EQUAL(sut::test_conversion(sut::Bar{}).as_string(), "sut::Bar");
    BOOST_CHECK_EQUAL(zmbt::json_from(sut::Bar{}), "Lol");
}
