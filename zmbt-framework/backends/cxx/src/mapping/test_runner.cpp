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
    using ChannelCombo = std::list<ChannelHandle>;
    JsonNode model_;
    Environment env;

    std::vector<ChannelHandle> static_inputs_;
    std::vector<ChannelCombo> static_outputs_;
    std::vector<std::pair<std::size_t, ChannelHandle>> dynamic_inputs_;
    std::vector<std::pair<std::size_t, ChannelCombo>> dynamic_outputs_;



    void report_failure(TestDiagnostics const& report);


    bool exec_prerun_tasks(TestDiagnostics diagnostics);

    void exec_postrun_tasks(TestDiagnostics diagnostics);

    bool inject_fixed_inputs(TestDiagnostics diagnostics);

    bool eval_fixed_assertions(TestDiagnostics diagnostics);

    bool inject_dynamic_inputs(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool execute_trigger(TestDiagnostics diagnostics);

    bool observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool eval_assertion(std::list<ChannelHandle> const& channel_group, Expression const& expr, TestDiagnostics& diagnostics);


public:
    InstanceTestRunner(JsonNode const& model);
    void Run();
};


void InstanceTestRunner::report_failure(TestDiagnostics const& report)
{
    env.HandleTestFailure(report.to_json());
}


bool InstanceTestRunner::exec_prerun_tasks(TestDiagnostics diagnostics)
{
    if (!model_.contains("/pre-run")) return true;
    for (const auto& action : model_.at("/pre-run").as_array()) {
        try {
            env.RunAction(action.as_string());
        }
        catch (std::exception const& error) {
            report_failure(diagnostics.Error("pre-run", error.what()));
            return false;
        }
    }
    return true;
}

bool InstanceTestRunner::inject_fixed_inputs(TestDiagnostics diagnostics)
{
    (void) diagnostics;
    for (auto const& static_cnl: static_inputs_)
    {
        try
        {
            static_cnl.inject_fixed();
        }
        catch(const std::exception& e)
        {
            report_failure(diagnostics
                .Error("fixed input injection", e.what())
                .ChannelId(static_cnl.alias()));
            return false;
        }
    }
    return true;
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



bool InstanceTestRunner::inject_dynamic_inputs(boost::json::array const& test_vector, TestDiagnostics diagnostics)
{
    bool no_exception_met{true};

    std::size_t col_idx {};
    for (auto const& input: dynamic_inputs_)
    {
        auto const& condition_idx = input.first;
        auto const& channel = input.second;
        auto const& test_expr = test_vector.at(condition_idx);

        try {
            channel.inject(test_expr);
        }
        catch (std::exception const& error) {
            report_failure(diagnostics
                .Error("test input injection", error.what())
                .TestCol(col_idx)
            );
            no_exception_met = false;
        }
        ++col_idx;
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

bool InstanceTestRunner::eval_assertion(std::list<ChannelHandle> const& channel_group, Expression const& expr, TestDiagnostics& diagnostics)
{
    bool assertion_passed{true};

    if (expr.is_noop())
    {
        return true;
    }

    boost::json::value observed {};
    SignalOperatorHandler op {};

    // getting observed value
    if (assertion_passed)
    {
        auto const& combo = channel_group.cbegin()->combine();
        try {
            if (combo == "union")
            {
                observed = ChannelHandle::observe_union(channel_group);
            }
            else if (combo == "with")
            {
                observed = ChannelHandle::observe_with(channel_group);
            }
            else
            {
                observed = channel_group.cbegin()->observe();
                op = channel_group.cbegin()->op();
            }
        }
        catch (std::exception const& error) {
            diagnostics.Error("output observation",error.what());
            assertion_passed = false;
        }
    }

    // testing observed value
    if (assertion_passed)
    {
        try
        {
            if (not (expr.match(observed, op)))
            {
                Expression::EvalContext ctx {op, Expression::EvalLog::make(), 0};
                expr.eval(observed, ctx);

                diagnostics
                    .Fail(expr, observed)
                    .EvalStack(ctx.log);

                assertion_passed = false;
            }
        }
        catch(const std::exception& e)
        {
            diagnostics
                .Error("output match evaluation", e.what());

            assertion_passed = false;
        }
    }

    return assertion_passed;
}


bool InstanceTestRunner::eval_fixed_assertions(TestDiagnostics diagnostics)
{
    bool passed {true};

    for (auto const& group: static_outputs_)
    {
        Expression expect = group.back().expect();
        try
        {
            expect = Expression::asPredicate(expect);
        }
        catch(const std::exception& e)
        {
            diagnostics.Error("fixed expectation eval", e.what());
            passed = false;
        }

        if (passed)
        {
            passed = eval_assertion(group, expect, diagnostics) && passed;
        }

        if (!passed)
        {
            boost::json::value ids;
            if (group.size() > 1)
            {
                ids.emplace_array();
                for (auto const& cnl: group)
                {
                    ids.get_array().push_back(cnl.alias());
                }
            }
            else
            {
                ids = group.back().alias();
            }
            report_failure(diagnostics.ChannelId(ids));
        }
    }
    return passed;
}

bool InstanceTestRunner::observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics)
{
    bool test_case_passed {true};

    for (auto const& output: dynamic_outputs_)
    {
        auto const  condition_idx = output.first;
        auto const& channel_group = output.second;

        if (!channel_group.cbegin()->is_output())
        {
            continue;
        }

        test_case_passed = eval_assertion(channel_group, Expression::asPredicate(test_vector.at(condition_idx)), diagnostics) && test_case_passed;

        if (!test_case_passed)
        {
            report_failure(diagnostics
                .TestCol(condition_idx));
        }
    }

    return test_case_passed;
}


InstanceTestRunner::InstanceTestRunner(JsonNode const& model)
    : model_(model)
{

    std::list<std::list<ChannelHandle>> channel_groups;

    // Step 1 - fill the group list
    channel_groups.push_back({});
    auto group_it = channel_groups.rbegin();
    auto const N = model_.at("/channels").as_array().size();
    for (std::size_t i = 0; i < N; i++)
    {
        ChannelHandle channel{model_, format("/channels/%d", i)};

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
            channel_groups.push_back({});
            group_it = channel_groups.rbegin();
        }

        group_it->push_back(channel);
    }

    // Step 2 - split static and dynamic
    std::size_t condition_index{0};
    while (!channel_groups.empty())
    {
        auto const& group_head = *channel_groups.front().cbegin();
        auto const& group_tail = *channel_groups.front().crbegin();


        if (group_head.is_input())
        {
            if (group_head.is_fixed_input())
            {
                static_inputs_.push_back(group_head);
            }
            else
            {
                dynamic_inputs_.push_back({condition_index++, group_head});
            }
        }
        else if (group_tail.is_fixed_output())
        {
            static_outputs_.push_back(std::move(channel_groups.front()));
        }
        else
        {
            dynamic_outputs_.push_back({condition_index++, std::move(channel_groups.front())});
        }
        channel_groups.pop_front();
    }
}


void InstanceTestRunner::Run()
{
    auto const N = dynamic_inputs_.size() + dynamic_outputs_.size();
    boost::json::array const& tests = model_("/tests").as_array();

    for (std::size_t i = 0; i < tests.size(); i++)
    {
        boost::json::array const& test_vector = tests.at(i).as_array();
        if (test_vector.size() != N)
        {
            throw model_error("inconsistent test vecor size at test case %d", i);
        }

        bool success {true};

        auto to_string = [](boost::json::value const& node) {
            return node.is_string() ? node.get_string().c_str() : boost::json::serialize(node);
        };

        auto diagnostics = TestDiagnostics(to_string(model_("/name")))
            .Description(to_string(model_("/description")))
            .Vector(test_vector)
            .TestRow(i)
            .Comment(model_.get_or_default(format("/comments/%d", i), "").as_string());
            ;

        env.ResetInterfaceData();
        success = success && exec_prerun_tasks(diagnostics);
        success = success && inject_fixed_inputs(diagnostics);
        success = success && inject_dynamic_inputs(test_vector, diagnostics);
        success = success && execute_trigger(diagnostics);
        success = success && eval_fixed_assertions(diagnostics);
        success = success && observe_results(test_vector, diagnostics);

        exec_postrun_tasks(diagnostics);

        if (!success)
        {
            // TODO: handle
        }
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

