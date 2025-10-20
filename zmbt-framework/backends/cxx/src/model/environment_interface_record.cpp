/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <atomic>
#include <chrono>
#include <exception>
#include <thread>

#include "zmbt/model/global_stats.hpp"
#include "zmbt/model/environment_interface_record.hpp"


namespace zmbt {


Environment::InterfaceHandle::InterfaceHandle(Environment const& e, interface_id const& interface, object_id refobj)
    : refobj_{refobj}
    , interface_{interface}
    , env{e}
    , output_recorder_{env.GetRecorder(interface_, refobj_)}
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


boost::json::value Environment::InterfaceHandle::PrototypeReturn() const
{
    return env.GetPrototypes(interface()).ret();
}

boost::json::array Environment::InterfaceHandle::PrototypeArgs() const
{
    return env.GetPrototypes(interface()).args();
}

void Environment::InterfaceHandle::EnableOutputRecordFor(ChannelKind const ck)
{
    output_recorder_->enable_category(ck);
}

void Environment::InterfaceHandle::Inject(std::shared_ptr<Generator> gen, lang::Expression const& tf, ChannelKind const kind, boost::json::string_view jp)
{
    std::shared_ptr<InjectionTable> table;
    env.data_->injection_tables.try_emplace_or_visit(
        std::make_pair(interface_, refobj_),
        [&]() -> std::shared_ptr<InjectionTable> {
            table = InjectionTable::Make(interface_, refobj_);
            return table;
        },
        [&table](auto& record){
            table = record.second;
        }
    );
    table->add_record(kind, InjectionTable::Record{jp, gen, tf});
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
    auto const start = std::chrono::steady_clock::now();

    // TODO: ensure it is closed for updates
    // non-const - only generator atomic counters are incremented
        // auto lock = env.Lock();
        // no lock here - it runs under assumption there are no updates during test execution.
        // TODO: split Env interface for managed and unmanaged,
        // and close modification of managed data on client-side during test execution.


    std::shared_ptr<InjectionTable> table;
    env.data_->injection_tables.visit(std::make_pair(interface_, refobj_), [&table](auto& record){
        table = record.second;
    });

    boost::json::value result{};

    if (!table)
    {
        switch (kind)
        {
        case ChannelKind::Args:
            result = env.GetPrototypes(interface_).args();
            break;
        case ChannelKind::Return:
            result = env.GetPrototypes(interface_).ret();
            break;
        default:
            result = nullptr;
            break;
        }
    }
    else
    {
        boost::json::value maybe_error;
        result = table->yield(kind, maybe_error);
        if (!maybe_error.is_null())
        {
            Environment().SetTestError(std::move(maybe_error));
        }
    }

    flags::InjectionTime::add(std::chrono::steady_clock::now() - start);
    return result;
}



std::size_t Environment::InterfaceHandle::ObservedCalls() const
{
    output_recorder_->flush();
    return output_recorder_->count();
}


boost::json::array Environment::InterfaceHandle::CaptureSlice(boost::json::string_view signal_path) const
{
    output_recorder_->flush();
    return slice(output_recorder_->data_frames(), signal_path);
}

boost::json::array const& Environment::InterfaceHandle::Captures() const
{
    output_recorder_->flush();
    return output_recorder_->data_frames();
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
