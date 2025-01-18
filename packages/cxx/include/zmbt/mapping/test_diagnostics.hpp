/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MAPPING_TEST_DIAGNOSTICS_HPP_
#define ZMBT_MAPPING_TEST_DIAGNOSTICS_HPP_

#include <boost/json.hpp>
#include <zmbt/model/expression.hpp>
#include <zmbt/model/signal_operator_handler.hpp>

namespace zmbt {
namespace mapping {

/// Test diagnostics for test case execution
struct TestDiagnostics
{
    enum class Result
    {
        Success,
        Fail,
        Error
    };


    boost::json::string model;
    boost::json::string description{};
    boost::json::string comment {};
    boost::json::string message    {};
    boost::json::value expected    {};
    boost::json::value observed    {};
    boost::json::value decorator   {};
    boost::json::value vector      {};
    boost::json::value nof_channel {};
    boost::json::value nof_vector  {};
    boost::json::value trigger     {};
    boost::json::array channels    {};
    Result kind           {};


    TestDiagnostics(boost::json::string_view model) : model{model} {}

    TestDiagnostics(TestDiagnostics const&) = default;
    TestDiagnostics(TestDiagnostics &&) = default;

    TestDiagnostics& operator=(TestDiagnostics const&) = default;
    TestDiagnostics& operator=(TestDiagnostics &&) = default;
    ~TestDiagnostics() = default;

    TestDiagnostics& Description(boost::json::string_view msg)
    {
        this->description = msg; return *this;
    }

    TestDiagnostics& Comment(boost::json::string_view msg)
    {
        this->comment = msg; return *this;
    }

    TestDiagnostics& Vector(boost::json::value val)
    {
        this->vector = val; return *this;
    }
    TestDiagnostics& ChannelIdx(int idx)
    {
        this->nof_channel = idx; return *this;
    }
    TestDiagnostics& VectorIdx(int idx)
    {
        this->nof_vector  = idx; return *this;
    }


    TestDiagnostics& Channels(boost::json::value const& cnls)
    {
        this->channels = cnls.as_array(); return *this;
    }
    TestDiagnostics& Trigger(boost::json::value const& trig)
    {
        this->trigger = trig; return *this;
    }

    /// report test setup or execution error
    TestDiagnostics& Error(boost::json::string_view origin, boost::json::string_view msg);

    /// report test expectation failure
    TestDiagnostics& Fail(Expression expected, boost::json::value observed, SignalOperatorHandler op);

    boost::json::value to_json() const;
};

} // namespace mapping
} // namespace zmbt

#endif // ZMBT_MAPPING_EXPECT_VARIANTS_HPP_
