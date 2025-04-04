/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>
#include <zmbt/core/exceptions.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/model/environment.hpp>
#include <zmbt/model/environment_interface_record.hpp>
#include <zmbt/model/exceptions.hpp>
#include <zmbt/model/expression.hpp>
#include <zmbt/model/keyword.hpp>
#include <zmbt/model/signal_operator_handler.hpp>
#include <zmbt/core.hpp>
#include <cstddef>
#include <exception>
#include <memory>
#include <stdexcept>
#include <tuple>

#include "zmbt/logging.hpp"
#include "zmbt/mapping/test_runner.hpp"
#include "zmbt/mapping/test_diagnostics.hpp"
#include "zmbt/mapping/test_parameter_resolver.hpp"
#include "zmbt/mapping/channel_handle.hpp"



namespace
{
using namespace zmbt;
using namespace zmbt::mapping;
using Keyword = zmbt::dsl::Keyword;

class InstanceTestRunner
{
    JsonNode model_;
    Environment env;
    std::vector<std::list<ChannelHandle>> dynamic_channel_groups_;
    std::list<ChannelHandle> static_channels_; // keep


    void report_failure(TestDiagnostics report);

    TestDiagnostics test_case_diagnostics(std::size_t n);

    void exec_prerun_tasks(TestDiagnostics diagnostics);

    void setup_static_channels(TestDiagnostics diagnostics);

    void exec_postrun_tasks(TestDiagnostics diagnostics);

    bool prepare_test(std::size_t const n, TestDiagnostics diagnostics);

    bool execute_trigger(TestDiagnostics diagnostics);

    bool observe_results(std::size_t n, TestDiagnostics diagnostics);

public:
    InstanceTestRunner(JsonNode const& model);
    void Run();
};


void InstanceTestRunner::report_failure(TestDiagnostics report)
{
    env.HandleTestFailure(report.to_json());
}


TestDiagnostics InstanceTestRunner::test_case_diagnostics(std::size_t n)
{

    std::string name =  model_("/name").is_string() ? model_("/name").get_string().c_str() : boost::json::serialize(model_("/name"));
    std::string descr =  model_("/description").is_string() ? model_("/description").get_string().c_str() : boost::json::serialize(model_("/description"));

    auto diagnostics = TestDiagnostics(name)
    .Description(descr)
    .Comment(model_.get_or_default(format("/comments/%d", n), "").as_string())
    .VectorIdx(n)
    .Vector(model_.at("/tests/%d", n).as_array())
    .Channels(model_.at("/channels").as_array())
    .Trigger(model_.at("/trigger"));


    return diagnostics;
}


void InstanceTestRunner::exec_prerun_tasks(TestDiagnostics diagnostics)
{
    if (!model_.contains("/pre-run")) return;
    for (const auto& action : model_.at("/pre-run").as_array()) {
        try {
            env.RunAction(action.as_string());
        }
        catch (std::exception const& error) {
            report_failure(diagnostics.Error("pre-run", error.what()));
        }
    }
}

void InstanceTestRunner::setup_static_channels(TestDiagnostics diagnostics)
{
    (void) diagnostics;
    for (auto const& static_cnl: static_channels_)
    {
        static_cnl.inject_yield();
    }
}

void InstanceTestRunner::exec_postrun_tasks(TestDiagnostics diagnostics)
{
    if (!model_.contains("/post-run")) return;
    for (const auto& action : model_.at("/post-run").as_array()) {
        try {
            env.RunAction(action.as_string());
        }
        catch (std::exception const& error) {
            report_failure(diagnostics.Error("pre-run", error.what()));
        }
    }
}



bool InstanceTestRunner::prepare_test(std::size_t const n, TestDiagnostics diagnostics)
{
    // RESET CAPTURED ARGS/RETURNS
    env.ResetInterfaceData();

    exec_prerun_tasks(diagnostics);

    setup_static_channels(diagnostics);

    auto const& test_vector = model_.at("/tests/%d", n).as_array();
    if (test_vector.size() != dynamic_channel_groups_.size())
    {
        throw model_error("inconsistent test vecor size at test case %d", n);
    }
    auto test_value_it = test_vector.cbegin();
    auto channel_group_it = dynamic_channel_groups_.cbegin();
    bool no_exception_met{true};
    std::size_t column_idx {0};
    while (test_value_it != test_vector.cend())
    {
        auto const& test_expr = *test_value_it++;
        auto const& channel_group = *channel_group_it++;

        try {
            if (channel_group.cbegin()->is_input())
            {
                channel_group.cbegin()->inject(test_expr);
            }
            else if ((channel_group.cbegin()->is_keep()))
            {

            }
            else // output
            {
                continue;
            }
        }
        catch (std::exception const& error) {
            report_failure(diagnostics
                .Error("sample injection", error.what())
                .ChannelIdx(column_idx)
            );
            no_exception_met = false;
        }
        column_idx++;
    }

    return no_exception_met;
}


bool InstanceTestRunner::execute_trigger(TestDiagnostics diagnostics)
{
    try {
        Environment::InterfaceHandle ifc_rec{model_.at("/trigger").as_string()};
        auto const& runs =  boost::json::value_to<std::uint64_t>(model_.get_or_default("/repeat_trigger", 1U));
        for (std::uint64_t i = 0; i < runs; i++)
        {
            ifc_rec.RunAsTrigger(i);
        }
        return true;
    }
    catch (std::exception const& error) {
        report_failure(diagnostics.Error("trigger", error.what()));
        return false;
    }
}


bool InstanceTestRunner::observe_results(std::size_t n, TestDiagnostics diagnostics)
{
    auto const& test_vector = model_.at("/tests/%d", n).as_array();
    if (test_vector.size() != dynamic_channel_groups_.size())
    {
        throw model_error("inconsistent test vecor size at test case %d", n);
    }
    auto test_value_it = test_vector.cbegin();
    auto channel_group_it = dynamic_channel_groups_.cbegin();
    std::size_t column_idx {0};
    bool test_case_passed{true};

    while (test_value_it != test_vector.cend())
    {
        bool assertion_passed{true};
        auto const& test_value = *test_value_it++;
        auto const& channel_group = *channel_group_it++;

        if (!channel_group.cbegin()->is_output())
        {
            ++column_idx;
            continue;
        }
        auto expr = Expression(test_value);
        if (expr.is(Keyword::Noop))
        {
            ++column_idx;
            continue;
        }
        else if (expr.is(Keyword::Apply))
        {
            try
            {
                expr = expr.eval();
            }
            catch(std::exception const& e)
            {
                assertion_passed = false;
                diagnostics
                    .Error("apply match evaluation", e.what())
                    .ChannelIdx(column_idx);
            }
        }

        boost::json::value observed {};
        SignalOperatorHandler op {};

        if (assertion_passed)
        {
            auto const& combo = channel_group.cbegin()->combine();
            try {
                if (combo == "union")
                {
                    observed = ChannelHandle::observe_series(channel_group);
                }
                else if (combo == "with")
                {
                    observed = ChannelHandle::observe_join(channel_group);
                }
                else
                {
                    observed = channel_group.cbegin()->observe();
                    op = channel_group.cbegin()->op();
                }
            }
            catch (std::exception const& error) {
                diagnostics
                    .Error("output observation",error.what())
                    .ChannelIdx(column_idx);
                assertion_passed = false;
            }
        }

        if (assertion_passed)
        {
            try
            {
                if (not (expr.match(observed, op)))
                {
                    Expression::EvalContext ctx {op, Expression::EvalLog::make(), 0};
                    expr.eval(observed, ctx);

                    diagnostics
                        .Fail(expr, observed, op)
                        .ChannelIdx(column_idx)
                        .EvalStack(ctx.log);

                    assertion_passed = false;
                }
            }
            catch(const std::exception& e)
            {
                diagnostics
                    .Error("output match evaluation", e.what())
                    .ChannelIdx(column_idx);

                assertion_passed = false;
            }
        }

        if (!assertion_passed)
        {
            test_case_passed = false;
            report_failure(diagnostics);
        }
        column_idx++;

    }

    return test_case_passed;
}


InstanceTestRunner::InstanceTestRunner(JsonNode const& model)
    : model_(model)
    , dynamic_channel_groups_{}
    , static_channels_{}
{
    dynamic_channel_groups_.push_back({});
    auto group_it = dynamic_channel_groups_.rbegin();
    auto const N = model_.at("/channels").as_array().size();
    for (std::size_t i = 0; i < N; i++)
    {
        ChannelHandle channel{model_, format("/channels/%d", i)};
        if (channel.is_keep())
        {
            static_channels_.push_back(channel);
            continue;
        }

        bool switch_group {true};

        if (group_it->empty())
        {
            switch_group = false;
        }
        else if (channel.combine().is_null())
        {
            switch_group = group_it->cbegin()->combine().is_null();
        }
        else
        {
            switch_group = channel.combine() != group_it->cbegin()->combine();
        }

        if (switch_group)
        {
            dynamic_channel_groups_.push_back({});
            group_it = dynamic_channel_groups_.rbegin();
        }

        group_it->push_back(channel);
    }
}


void InstanceTestRunner::Run()
{
    boost::json::array const& tests = model_("/tests").as_array();
    for (std::size_t i = 0; i < tests.size(); i++)
    {
        auto diagnostics     = test_case_diagnostics(i);
        bool success         = prepare_test(i, diagnostics);
        if (success) success = execute_trigger(diagnostics);
        if (success) observe_results(i, diagnostics);

        exec_postrun_tasks(diagnostics);
    }
}

}


namespace zmbt {
namespace mapping {


class TestRunner::Impl
{
    TestParameterResolver resolver_;

  public:

    Impl(JsonNode const& model) : resolver_{model} {}

    void Run()
    {

        JsonNode instance = resolver_.Next();
        while (!instance.is_null())
        {
            InstanceTestRunner(instance).Run();
            instance = resolver_.Next();
        }
    }

};

TestRunner::TestRunner(JsonNode const& model) : impl_{std::make_unique<TestRunner::Impl>(model)} {}

void TestRunner::Run()
{
    impl_->Run();
}

TestRunner::~TestRunner() {}

} // namespace mapping
} // namespace zmbt

