/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include "zmbt/model/environment_interface_record.hpp"


namespace zmbt {


Environment::InterfaceHandle::InterfaceHandle(Environment const& e, interface_id const& interface, object_id refobj)
    : refobj_{refobj}
    , interface_{interface}
    , env{e}
{
    auto lock = Env().Lock();
    auto& ifc_rec = Env().data_->json_data;
    captures = ifc_rec.branch(boost::json::kind::array, "/interface_records/%s/%s/captures", refobj, interface);
}

Environment::InterfaceHandle::InterfaceHandle(Environment const& e, boost::json::string_view ref)
    : Environment::InterfaceHandle::InterfaceHandle(
        e,
        e.InterfaceId(ref),
        e.ObjectId(ref)
    )
{
}

Environment::InterfaceHandle::InterfaceHandle(interface_id const& interface, object_id refobj)
    : Environment::InterfaceHandle::InterfaceHandle(Environment {}, interface, refobj)
{
}

Environment::InterfaceHandle::InterfaceHandle(interface_id const& interface, nullptr_t)
    : Environment::InterfaceHandle::InterfaceHandle(interface, object_id(nullptr))
{
}

Environment::InterfaceHandle::InterfaceHandle(boost::json::string_view ref)
    : Environment::InterfaceHandle::InterfaceHandle(Environment {}, ref)
{
}


boost::json::value const& Environment::InterfaceHandle::PrototypeReturn() const
{
    auto lock = Env().Lock();
    return env.data_->json_data.at("/prototypes/%s/return", interface());
}

boost::json::value const& Environment::InterfaceHandle::PrototypeArgs() const
{
    auto lock = Env().Lock();
    return env.data_->json_data.at("/prototypes/%s/args", interface());
}

void Environment::InterfaceHandle::Inject(std::shared_ptr<Generator> gen, lang::Expression const& tf, ChannelKind const kind, boost::json::string_view jp)
{
    auto lock = Env().Lock();
    env.data_->input_generators[refobj_][interface_][kind][jp] = {gen, tf};
}


boost::json::value Environment::InterfaceHandle::YieldInjection(ChannelKind const kind)
{
    boost::json::value result_value;

    if (ChannelKind::Return == kind)
    {
        result_value = PrototypeReturn();
    }
    else if (ChannelKind::Args == kind)
    {
        result_value = PrototypeArgs();
    }
    else
    {
        throw model_error("%s injection not implemented", json_from(kind));
    }

    // TODO: ensure no it is closed for updates
    // non-const - only generator atomic counters are incremented
    auto& inputs = [&]() -> EnvironmentData::GeneratorsTable& {
        // auto lock = env.Lock();
        // no lock here - it runs under assumption there are no updates during test execution.
        // TODO: split Env interface for managed and unmanaged,
        // and close modification of managed data on client-side during test execution.
        return env.data_->input_generators[refobj_][interface_][kind];
    }();

    for (auto& record: inputs)
    {
        boost::json::string_view const record_pointer = record.first;
        auto& generator = *record.second.first;
        auto const& tf = record.second.second;
        if (generator.is_noop()) continue;

        boost::json::value raw_v;
        // TODO: optimize recursive expr
        auto const iteration = generator(raw_v);
        auto const v = tf.is_noop() ? raw_v : tf.eval(raw_v); // transform can handle generator errors

        if (lang::Expression(v).is_error())
        {
            lang::Expression::EvalContext generator_ctx {{}, lang::Expression::EvalLog::make(), 0};
            generator.expression().eval(iteration, generator_ctx);

            lang::Expression::EvalContext transform_ctx {{}, lang::Expression::EvalLog::make(), 0};
            if (not tf.is_noop())
            {
                tf.eval(raw_v, transform_ctx);
            }

            ZMBT_LOG_JSON(FATAL) << boost::json::object{
                {"ZMBT_INJECTION", boost::json::object{
                    {"interface", key()},
                    {"group", json_from(kind)},
                    {"pointer", record_pointer},
                    {"Inject log", *generator_ctx.log.stack},
                    {"Take log", *transform_ctx.log.stack},
                }}
            };

            auto const failed_inject = raw_v == v;
            auto const error_context = failed_inject ? "Inject" : "Take";

            auto const origin = format("ZMBT_INJECTION(%s, %s%s, %s)",
                key(), json_from(kind), record_pointer, error_context);

            ZMBT_LOG_CERR(FATAL).WithSrcLoc(origin) << "\n"
            << (error_context ? generator_ctx.log.str(2) : transform_ctx.log.str(2));
            throw model_error("injection failure");
        }

        else
        {
            result_value.set_at_pointer(record_pointer, v);
        }

    }
    return result_value;
}



std::size_t Environment::InterfaceHandle::ObservedCalls() const
{
    auto lock = Env().Lock();
    return captures().as_array().size();
}


boost::json::array Environment::InterfaceHandle::ObservedArgs(int const nofcall)
{
    auto lock = Env().Lock();
    auto const N = captures().as_array().size();
    if (N == 0)
    {
        throw environment_error("ObservedArgs(%s) no captures found", interface());
    }
    auto const idx = nofcall < 0 ? N + nofcall : static_cast<std::size_t>(nofcall - 1);
    if (idx >= N)
    {
        throw environment_error("ObservedArgs(%s) index %d is out of range, N = %lu", interface(), nofcall, N);
    }
    return captures.get_or_create_array("/%d/args", idx);
}


boost::json::array Environment::InterfaceHandle::CaptureSlice(boost::json::string_view signal_path, int start, int stop, int const step) const
{
    auto lock = Env().Lock();
    return slice(captures().as_array(), signal_path, start, stop, step);
}

boost::json::array const& Environment::InterfaceHandle::Captures() const
{
    auto lock = Env().Lock();
    return captures().as_array();
}


boost::json::value Environment::InterfaceHandle::ObservedReturn(int const nofcall) const
{
    auto lock = Env().Lock();
    auto const N = captures().as_array().size();
    if (N == 0)
    {
        throw environment_error("ObservedReturn(%s) no captures found", interface());
    }
    auto const idx = nofcall < 0 ? N + nofcall : static_cast<std::size_t>(nofcall - 1);
    if (idx >= N)
    {
        throw environment_error("ObservedReturn(%s) index %lu is out of range, N = %lu", interface(), idx, N);
    }
    return captures.at("/%d/return", idx);
}

boost::json::string const& Environment::InterfaceHandle::key() const
{
    auto lock = Env().Lock();
    return Env().json_data().at("/refs/ids2key/%s/%s", refobj(), interface()).as_string();
}



Environment::InterfaceHandle& Environment::InterfaceHandle::RunAsAction()
{
    // TODO: handle actions as registered interfaces
    throw environment_error("RunAsAction not implemented!");
    boost::json::string_view ref = key();
    auto const& actions = Env().data_->callbacks;
    if (0 == actions.count(ref))
    {
        throw environment_error("Action %s is not registered!", ref);
    }

    try
    {
        actions.at(ref).operator()();
    }
    catch(const std::exception& e)
    {
        throw environment_error("Action %s error: `%s`", ref, e.what());
    }
    return *this;
}

Environment::InterfaceHandle& Environment::InterfaceHandle::RunAsTrigger(std::size_t const nofcall)
{
    // TODO: make it thread safe without recursive mutex
    boost::json::string_view ref = key();
    auto const& triggers = Env().data_->triggers;
    if (0 == triggers.count(ref))
    {
        throw environment_error("Trigger %s is not registered!", ref);
    }

    Trigger const& trigger = triggers.at(ref);
    boost::json::value args;
    boost::json::value capture;

    try
    {
        args = YieldInjection(ChannelKind::Args);
    }
    catch(const std::exception& e)
    {
        throw environment_error("Trigger[%s] #%d input evaluation error: `%s`", ref, nofcall, e.what());
    }
    try
    {
        capture = trigger(args);
        captures("/+") = capture;
    }
    catch(const model_error& e)
    {
        throw e;
    }
    catch(const std::exception& e)
    {
        throw environment_error("Trigger[%s] #%d execution error: `%s`, args: %s", ref, nofcall, e.what(), args);
    }

    return *this;
}

}  // namespace zmbt
