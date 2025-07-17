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
#include <zmbt/expr/expression.hpp>
#include <zmbt/expr/api.hpp>
#include <zmbt/expr/keyword.hpp>
#include <zmbt/expr/operator.hpp>
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
using Keyword = zmbt::lang::Keyword;

class InstanceTestRunner
{
    using ConditionPipe = std::list<ChannelHandle>;
    JsonNode model_;
    Environment env;

    std::vector<PipeHandle> inline_inputs_;
    std::vector<PipeHandle> inline_outputs_;
    std::vector<PipeHandle> tabular_inputs_;
    std::vector<PipeHandle> tabular_outputs_;



    bool run_test_procedure(boost::json::array const& test_vector, std::size_t const idx);

    void report_failure(TestDiagnostics const& report);

    bool exec_prerun_tasks(TestDiagnostics diagnostics);

    void exec_postrun_tasks(TestDiagnostics diagnostics);

    bool inject_inline_inputs(TestDiagnostics diagnostics);

    bool eval_inline_assertions(TestDiagnostics diagnostics);

    bool inject_tabular_inputs(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool execute_trigger(TestDiagnostics diagnostics);

    bool observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool eval_assertion(PipeHandle const& condition_pipe, lang::Expression expr, TestDiagnostics& diagnostics);


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
            env.RunAction(action);
        }
        catch (std::exception const& error) {
            report_failure(diagnostics.Error("pre-run", error.what()));
            return false;
        }
    }
    return true;
}

bool InstanceTestRunner::inject_inline_inputs(TestDiagnostics diagnostics)
{
    (void) diagnostics;
    for (auto const& pipe: inline_inputs_)
    {
        try
        {
            pipe.inject(pipe.expression());
        }
        catch(const std::exception& e)
        {
            report_failure(diagnostics
                .Error("inline input injection", e.what())
                .PipeId(pipe.index()));
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
            env.RunAction(action);
        }
        catch (std::exception const& error) {
            report_failure(diagnostics.Error("pre-run", error.what()));
        }
    }
}



bool InstanceTestRunner::inject_tabular_inputs(boost::json::array const& test_vector, TestDiagnostics diagnostics)
{
    bool no_exception_met{true};

    for (auto const& pipe: tabular_inputs_)
    {
        auto const& condition_idx = pipe.column();
        auto const& test_expr = test_vector.at(condition_idx);

        try {
            pipe.inject(test_expr);
        }
        catch (std::exception const& error) {
            report_failure(diagnostics
                .Error("test input injection", error.what())
                .PipeId(pipe.index())
            );
            no_exception_met = false;
        }
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

bool InstanceTestRunner::eval_assertion(PipeHandle const& condition_pipe, lang::Expression e, TestDiagnostics& diagnostics)
{

    if (e.is_noop())
    {
        return true;
    }


    boost::json::value observed;
    bool observe_success{true};

    try
    {
        observed = condition_pipe.observe();
    }
    catch (std::exception const& error) {
        diagnostics.Error("output observation",error.what());
        observe_success = false;
    }

    // testing observed value
    if (observe_success)
    {
        lang::Expression::to_predicate_if_const(e);
        condition_pipe.overload(e);

        try
        {
            auto const result = e.eval(observed);
            auto const passed = result.is_bool() && result.get_bool();
            if (lang::Expression(result).is_error())
            {
                diagnostics
                    .Error("output match evaluation", result);
                return false;
            }
            else if (not passed)
            {
                lang::EvalContext ctx {{}, lang::EvalLog::make(), 0};
                e.eval(observed, ctx);

                diagnostics
                    .Fail(e, observed)
                    .EvalStack(ctx.log);

                return false;
            }
        }
        catch(const std::exception& e)
        {
            diagnostics
                .Error("output match evaluation", e.what());
            return false;
        }
    }
    return true;
}


bool InstanceTestRunner::eval_inline_assertions(TestDiagnostics diagnostics)
{
    bool passed {true};

    for (auto const& pipe: inline_outputs_)
    {
        lang::Expression expect = pipe.expression();

        if (passed)
        {
            passed = eval_assertion(pipe, expect, diagnostics) && passed;
        }

        if (!passed)
        {
            report_failure(
                diagnostics
                .PipeId(pipe.index()));
        }
    }
    return passed;
}

bool InstanceTestRunner::observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics)
{
    bool test_case_passed {true};

    for (auto const& pipe: tabular_outputs_)
    {

        test_case_passed = eval_assertion(pipe, test_vector.at(pipe.column()), diagnostics) && test_case_passed;

        if (!test_case_passed)
        {
            report_failure(diagnostics
                .TabularConditionFailure(pipe.column()));
        }
    }

    return test_case_passed;
}


InstanceTestRunner::InstanceTestRunner(JsonNode const& model)
    : model_(model)
{

    auto const N = model_.at("/pipes").as_array().size();
    std::list<PipeHandle> pipes_list;
    for (std::size_t i = 0; i < N; i++)
    {
        pipes_list.emplace_back(model_, i);
    }



    // Step 2 - split inline and tabular
    while (!pipes_list.empty())
    {

        if (pipes_list.front().is_input())
        {
            if (pipes_list.front().has_expression())
            {
                inline_inputs_.push_back(std::move(pipes_list.front()));
            }
            else
            {
                tabular_inputs_.push_back(std::move(pipes_list.front()));
            }
        }
        else if (pipes_list.front().has_expression())
        {
            inline_outputs_.push_back(std::move(pipes_list.front()));
        }
        else
        {
            tabular_outputs_.push_back(std::move(pipes_list.front()));
        }
        pipes_list.pop_front();
    }
}


bool InstanceTestRunner::run_test_procedure(boost::json::array const& test_vector, std::size_t const idx)
{
    bool success {true};

    auto to_string = [](boost::json::value const& node) {
        return node.is_string() ? node.get_string().c_str() : boost::json::serialize(node);
    };

    auto diagnostics = TestDiagnostics(to_string(model_("/name")))
        .Description(to_string(model_("/description")))
        .Vector(test_vector)
        .TestRow(idx)
        .Comment(model_.get_or_default(format("/comments/%d", idx), "").as_string());
        ;

    env.ResetInterfaceData();
    success = success && exec_prerun_tasks(diagnostics);
    success = success && inject_inline_inputs(diagnostics);
    success = success && inject_tabular_inputs(test_vector, diagnostics);
    success = success && execute_trigger(diagnostics);
    success = success && eval_inline_assertions(diagnostics);
    success = success && observe_results(test_vector, diagnostics);

    exec_postrun_tasks(diagnostics);

    if (!success)
    {
        // TODO: handle
    }

    return success;
}


void InstanceTestRunner::Run()
{

    auto const N = tabular_inputs_.size() + tabular_outputs_.size();
    boost::json::array const& tests = model_("/tests").as_array();


    if (N == 0 && tests.empty())
    {
        run_test_procedure({}, 0); // no Test() clause
    }

    for (std::size_t i = 0; i < tests.size(); i++)
    {
        boost::json::array const& test_vector = tests.at(i).as_array();
        if (test_vector.size() != N)
        {
            throw model_error("inconsistent test vecor size at test case %d", i);
        }

        run_test_procedure(test_vector, i);
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

