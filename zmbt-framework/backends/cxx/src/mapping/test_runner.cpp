/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>
#include <zmbt/application.hpp>
#include <zmbt/core.hpp>
#include <zmbt/model.hpp>
#include <zmbt/expr.hpp>
#include <cstddef>
#include <chrono>
#include <cstdint>
#include <exception>
#include <memory>
#include <stdexcept>
#include <tuple>

#include "zmbt/mapping/test_runner.hpp"
#include "zmbt/mapping/test_diagnostics.hpp"
#include "zmbt/mapping/test_parameter_resolver.hpp"
#include "zmbt/mapping/channel_handle.hpp"
#include "zmbt/mapping/pipe_handle.hpp"
#include "zmbt/model/global_flags.hpp"
#include "zmbt/model/global_stats.hpp"


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

    void set_recorder_filters();

    bool eval_inline_assertions(TestDiagnostics diagnostics);

    bool inject_tabular_inputs(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool execute_trigger(TestDiagnostics diagnostics);

    bool observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics);

    bool eval_assertion(PipeHandle const& condition_pipe, lang::Expression expr, TestDiagnostics& diagnostics);

    static void reset_performance_counters();

    static boost::json::object collect_performance_report();


public:
    InstanceTestRunner(JsonNode const& model);
    void Run();
};


void InstanceTestRunner::report_failure(TestDiagnostics const& report)
{
    boost::json::value diagnostics(report.to_json());
    Config().HandleTestFailure(diagnostics);
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
                .PipeId(pipe.id()));
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
                .PipeId(pipe.id())
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

        flags::TestIsRunning::set();
        ifc_rec.RunAsTrigger(runs);
        flags::TestIsRunning::clear();

        if (env.HasTestError())
        {
            report_failure(diagnostics.Error("test error", env.TestError()));
            return false;
        }
        return true;
    }
    catch (std::exception const& error) {
        flags::TestIsRunning::clear();
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

    try
    {
        observed = condition_pipe.observe();
    }
    catch (std::exception const& error) {
        diagnostics.Error("output observation",error.what());
        return false;
    }

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
            auto ctx = lang::EvalContext::make();
            e.eval(observed, ctx);

            diagnostics
                .Fail(e, observed)
                .Traces(ctx.traces)
                .EvalStack(ctx.log);

            return false;
        }
        else
        {
            return true;
        }
    }
    catch(const std::exception& e)
    {
        diagnostics
            .Error("output match evaluation", e.what());
        return false;
    }
}

void InstanceTestRunner::set_recorder_filters()
{
    for (auto const& pipe: inline_outputs_)
    {
        for (auto const& channel: pipe.channels())
        {
            channel.inerface_handle().EnableOutputRecordFor(channel.kind());
        }
    }

    for (auto const& pipe: tabular_outputs_)
    {
        for (auto const& channel: pipe.channels())
        {
            channel.inerface_handle().EnableOutputRecordFor(channel.kind());
        }
    }
}

bool InstanceTestRunner::eval_inline_assertions(TestDiagnostics diagnostics)
{
    bool passed {true};

    for (auto const& pipe: inline_outputs_)
    {
        lang::Expression expect = pipe.expression();

        // FIXME: diagnostics is shared
        if (!eval_assertion(pipe, expect, diagnostics))
        {
            passed = false;
            report_failure(diagnostics
                .PipeId(pipe.id()));
        }
    }
    return passed;
}

bool InstanceTestRunner::observe_results(boost::json::array const& test_vector, TestDiagnostics diagnostics)
{
    bool passed {true};

    for (auto const& pipe: tabular_outputs_)
    {
        lang::Expression expect = test_vector.at(pipe.column());
        // FIXME: diagnostics is shared
        if (!eval_assertion(pipe, expect, diagnostics))
        {
            passed = false;
            report_failure(diagnostics
                .TabularConditionFailure(pipe.column()));
        }
    }

    return passed;
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

    zmbt::Config().ResetRng();
    zmbt::lang::GlobalEnv().Reset();
    reset_performance_counters();

    auto to_string = [](boost::json::value const& node) {
        return node.is_string() ? node.get_string().c_str() : boost::json::serialize(node);
    };


    auto const comment = to_string(lang::Expression(model_.get_or_default(format("/comments/%d", idx), "")).eval());

    auto diagnostics = TestDiagnostics(to_string(model_("/name")))
        .Description(to_string(model_("/description")))
        .Vector(test_vector)
        .TestRow(idx)
        .Comment(comment)
    ;

    env.ResetInterfaceData();
    success = success && exec_prerun_tasks(diagnostics);
    success = success && inject_inline_inputs(diagnostics);
    success = success && inject_tabular_inputs(test_vector, diagnostics);
    set_recorder_filters();
    success = success && execute_trigger(diagnostics);
    success = success && eval_inline_assertions(diagnostics);
    success = success && observe_results(test_vector, diagnostics);

    exec_postrun_tasks(diagnostics);

    ZMBT_LOG(DEVMODE) << collect_performance_report();

    if (!success)
    {
        // TODO: handle
    }

    // reset_performance_counters();
    return success;
}


void InstanceTestRunner::reset_performance_counters()
{
    flags::InjectionTime::reset();
    flags::RecordingTime::reset();
    flags::ConversionTime::reset();
}

boost::json::object InstanceTestRunner::collect_performance_report()
{
    using namespace std::chrono;

    auto const to_milliseconds = [](std::uint64_t const ns_value) -> double {
        return duration<double, std::milli>(nanoseconds(ns_value)).count();
    };

    return boost::json::object{
        {"injection time", to_milliseconds(flags::InjectionTime::value())},
        {"recording time", to_milliseconds(flags::RecordingTime::value())},
        {"conversion time", to_milliseconds(flags::ConversionTime::value())},
    };
}


void InstanceTestRunner::Run()
{

    auto const N = tabular_inputs_.size() + tabular_outputs_.size();
    boost::json::array const& tests = model_("/tests").as_array();

    if (N == 0 && tests.empty())
    {
        run_test_procedure({}, 0); // no Test() clause
    }
    else if(!tests.empty())
    {
        for (std::size_t i = 0; i < tests.size(); i++)
        {
            boost::json::array const& test_vector = tests.at(i).as_array();
            if (test_vector.size() != N)
            {
                throw_exception(model_error("inconsistent test vecor size at test case %d", i));
            }
            run_test_procedure(test_vector, i);
        }
    }
    else
    {
        throw_exception(model_error("missing the Test clause or inline conditions"));
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
