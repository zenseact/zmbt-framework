/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/environment_interface_record.hpp"



namespace zmbt {

Environment::InterfaceHandle::InterfaceHandle(Environment const& e, interface_id const& interface, object_id refobj)
    : env{e}
    , refobj_{refobj}
    , interface_{interface}
{
    auto lock = Env().Lock();
    auto& ifc_rec = Env().data_->json_data;
    captures = ifc_rec.branch(boost::json::kind::array, "/interface_records/%s/%s/captures", refobj, interface);
    injects = ifc_rec.branch(boost::json::kind::object, "/interface_records/%s/%s/injects", refobj, interface);
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

void Environment::InterfaceHandle::Inject(lang::Expression const& e, boost::json::string_view group, boost::json::string_view jp)
{
    auto lock = Env().Lock();
    auto& expr_map = injects.get_or_create_object("/%s/expr", group);
    expr_map[jp] = e; // boost::json::object holds insertion order

    // invalidate cache
    injects("/%s/cache", group).emplace_array();
}


boost::json::value Environment::InterfaceHandle::GetInjection(boost::json::string_view group, boost::json::string_view jp, int const nofcall)
{
    auto lock = Env().Lock();
    auto& injection_cache = injects.get_or_create_array("/%s/cache", group);

    if (injection_cache.empty())
    {
        injection_cache.push_back(nullptr);
        injection_cache.push_back(nullptr);
    }
    auto& cache_nofcall = injection_cache.at(0);
    auto& cache_value = injection_cache.at(1);

    if (cache_nofcall != nofcall) // compute and store inject at n
    {
        boost::json::value temp_node;

        if ("return" == group)
        {
            temp_node = PrototypeReturn();
        }
        else if ("args" == group)
        {
            temp_node = PrototypeArgs();
        }
        else
        {
            throw model_error("%s injection not implemented", group);
        }

        if (auto expr_map = injects("/%s/expr",group).if_object())
        {
            for (auto const& record: *expr_map)
            {
                boost::json::string_view const record_pointer = record.key();
                lang::Expression const expr (record.value());
                if (expr.is_noop()) continue;

                // TODO: optimize recursive expr
                auto v = expr.eval(nofcall); // TODO: handle errors
                temp_node.set_at_pointer(record_pointer, v);
            }
        }
        cache_value = temp_node;
        cache_nofcall = nofcall;
    }

    boost::json::error_code ec;
    auto p = cache_value.find_pointer(jp, ec);

    return p ? *p : nullptr;
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
        // TODO: format err msg
        throw environment_error("ObservedArgs(%s) index %d is out of range, N = %lu", interface(), nofcall, N);
    }
    return captures.get_or_create_array("/%d/args", idx);
}


boost::json::array Environment::InterfaceHandle::CaptureSlice(boost::json::string_view signal_path, int start, int stop, int const step)
{
    auto lock = Env().Lock();
    return slice(captures().as_array(), signal_path, start, stop, step);
}

boost::json::array const& Environment::InterfaceHandle::Captures()
{
    auto lock = Env().Lock();
    return captures().as_array();
}


boost::json::value Environment::InterfaceHandle::ObservedReturn(int const nofcall)
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
        // TODO: format err msg
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
        args = GetInjectionArgs(nofcall);
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
