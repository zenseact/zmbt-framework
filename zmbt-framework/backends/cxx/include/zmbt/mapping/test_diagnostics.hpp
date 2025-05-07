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

    // print_string("model");
    // print_string("message");
    // print_js_section("expected");
    // print_js_section("observed");

    // os << prefix << "condition: [" << sts.at("test") << "," << sts.at("channel") << "]";

    // print_string("description");
    // print_string("comment");

    // print_js_section("test vector");

    // auto const& eval_stack = sts.at("eval_stack").as_array();


    boost::json::string model_name;
    boost::json::string message    {};
    boost::json::string description{};
    boost::json::string comment    {};

    boost::json::value expected    {};
    boost::json::value observed    {};
    boost::json::value vector      {};
    size_t tr  {};
    size_t tc  {};

    boost::json::value channel_id  {};

    boost::json::array eval_stack  {};
    Result result                  {};


    TestDiagnostics(boost::json::string_view model_name) : model_name{model_name} {}

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
    TestDiagnostics& TestCol(std::size_t const c)
    {
        this->tc = c; return *this;
    }
    TestDiagnostics& TestRow(std::size_t const r)
    {
        this->tr = r; return *this;
    }
    TestDiagnostics& ChannelId(boost::json::value const& id)
    {
        this->channel_id = id; return *this;
    }
    TestDiagnostics& EvalStack(Expression::EvalLog const& log)
    {
        if (log.stack)
        {
            this->eval_stack = *log.stack;
        }
        return *this;
    }

    /// report test setup or execution error
    TestDiagnostics& Error(boost::json::string_view origin, boost::json::string_view msg);

    /// report test expectation failure
    TestDiagnostics& Fail(Expression expected, boost::json::value observed);

    boost::json::value to_json() const;
};

} // namespace mapping
} // namespace zmbt

#endif // ZMBT_MAPPING_EXPECT_VARIANTS_HPP_
