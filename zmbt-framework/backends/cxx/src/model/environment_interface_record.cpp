/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <atomic>
#include <exception>
#include <thread>

#include "zmbt/model/environment_interface_record.hpp"


namespace zmbt {


Environment::InterfaceHandle::InterfaceHandle(Environment const& e, interface_id const& interface, object_id refobj)
    : refobj_{refobj}
    , interface_{interface}
    , env{e}
    , capture_{env.GetCapture(interface_, refobj_)}
{
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

void Environment::InterfaceHandle::AddCaptureCategory(ChannelKind const ck)
{
    capture_->enable_category(ck);
};

void Environment::InterfaceHandle::Inject(std::shared_ptr<Generator> gen, lang::Expression const& tf, ChannelKind const kind, boost::json::string_view jp)
{
    auto const key = std::make_pair(interface_, refobj_);
    auto const kind_idx = static_cast<unsigned>(kind);
    EnvironmentData::InjectionTable table{};
    table.at(kind_idx).push_back({jp, gen, tf});

    using value_type = decltype(env.data_->injection_tables)::value_type;
    env.data_->injection_tables.try_emplace_or_visit(key, table, [kind_idx, &table](value_type& record){
        record.second.at(kind_idx).push_back(std::move(table.at(kind_idx).back()));
    });
}


void Environment::InterfaceHandle::MaybeThrowException()
{
    auto const maybe_exception = YieldInjection(ChannelKind::Exception);
    if (maybe_exception.is_null())
    {
        return;
    }

    lang::Expression const e(maybe_exception);
    auto const key = e.error_id();
    if (env.HasAction(key))
    {
        env.RunActionNoCatch(key);
    }
    else
    {
        throw_exception(std::runtime_error(e.data().at("message").as_string().c_str()));
    }
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
    else if (ChannelKind::Exception == kind)
    {
        result_value = nullptr;
    }
    else
    {
        throw_exception(model_error("%s injection not implemented", json_from(kind)));
    }

    // TODO: ensure it is closed for updates
    // non-const - only generator atomic counters are incremented
        // auto lock = env.Lock();
        // no lock here - it runs under assumption there are no updates during test execution.
        // TODO: split Env interface for managed and unmanaged,
        // and close modification of managed data on client-side during test execution.

    using value_type = decltype(env.data_->injection_tables)::value_type;
    auto const kind_idx = static_cast<unsigned>(kind);
    boost::optional<EnvironmentData::InputRecordList&> maybe_inputs;
    env.data_->injection_tables.visit(std::make_pair(interface_, refobj_), [kind_idx, &maybe_inputs](value_type& v){
        maybe_inputs = v.second.at(kind_idx);
    });

    if (!maybe_inputs.has_value()) return result_value;


    for (auto& record: *maybe_inputs)
    {
        boost::json::string_view const record_pointer = record.jptr;
        auto& generator = *record.generator;
        auto const& tf = record.transform;
        if (generator.is_noop()) continue;

        boost::json::value raw_v;
        // TODO: optimize recursive expr
        auto const iteration = generator(raw_v);
        auto const v = tf.is_noop() ? raw_v : tf.eval(raw_v); // transform can handle generator errors

        lang::Expression const v_as_expr(v);
        if (v_as_expr.is_error())
        {
            if (kind == ChannelKind::Exception)
            {
                result_value = v_as_expr.to_json();
                break;
            }
            // reevaluate to collect log

            auto generator_ctx = lang::EvalContext::make();
            generator.expression().eval(iteration, generator_ctx);

            auto transform_ctx = lang::EvalContext::make();
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
            // throw_exception(model_error("injection failure"));
            return {v_as_expr.prettify().c_str()};
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
    capture_->flush();
    return capture_->count();
}


boost::json::array Environment::InterfaceHandle::CaptureSlice(boost::json::string_view signal_path) const
{
    capture_->flush();
    return slice(capture_->data_frames(), signal_path);
}

boost::json::array const& Environment::InterfaceHandle::Captures() const
{
    capture_->flush();
    return capture_->data_frames();
}


boost::json::string const& Environment::InterfaceHandle::key() const
{
    auto lock = Env().Lock();
    return Env().json_data().at("/refs/ids2key/%s/%s", refobj(), interface()).as_string();
}



Environment::InterfaceHandle& Environment::InterfaceHandle::RunAsAction()
{
    // TODO: handle actions as registered interfaces
    throw_exception(environment_error("RunAsAction not implemented!"));
    boost::json::string_view ref = key();
    auto const& actions = Env().data_->callbacks;
    if (0 == actions.count(ref))
    {
        throw_exception(environment_error("Action %s is not registered!", ref));
    }

    try
    {
        actions.at(ref).operator()();
    }
    catch(const std::exception& e)
    {
        throw_exception(environment_error("Action %s error: `%s`", ref, e.what()));
    }
    return *this;
}

Environment::InterfaceHandle& Environment::InterfaceHandle::RunAsTrigger(std::size_t const repeats)
try
{
    boost::json::string_view ref = key();
    auto const& triggers = Env().data_->triggers;
    if (0 == triggers.count(ref))
    {
        env.SetTestError({
            {"error"    , "no registered trigger found"},
            {"reference", ref},
        });
        return *this;
    }

    Trigger const& trigger = triggers.at(ref);

    for (size_t frame = 0; frame < repeats; frame++)
    {
        trigger(YieldInjection(ChannelKind::Args));
        if (env.data_->has_test_error.load(std::memory_order_acquire))
        {
            break;
        }
    }

    return *this;
}
catch(const std::exception& e)
{
    auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
    env.SetTestError({
        {"error"    , "exception thrown at trigger evaluation"},
        {"interface", interface()                             },
        {"context"  , "Trigger"                               },
        {"what"     , e.what()                                },
        {"type"     , dynamic_exception_type                  }
    });
    return *this;
}

}  // namespace zmbt
