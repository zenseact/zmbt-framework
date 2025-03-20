/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/environment_interface_record.hpp"



namespace zmbt {

    Environment::InterfaceHandle::InterfaceHandle(Environment const& e, object_id refobj, interface_id const& interface)
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
            e.ObjectId(ref),
            e.InterfaceId(ref)
        )
    {
    }

    Environment::InterfaceHandle::InterfaceHandle(object_id refobj, interface_id const& interface)
        : Environment::InterfaceHandle::InterfaceHandle(Environment {}, refobj, interface)
    {
    }

    Environment::InterfaceHandle::InterfaceHandle(nullptr_t, interface_id const& interface)
        : Environment::InterfaceHandle::InterfaceHandle(object_id(nullptr), interface)
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

    boost::json::value Environment::InterfaceHandle::GetInjectionReturn(int const nofcall) const
    {
        auto lock = Env().Lock();
        auto ptr = injects.find_pointer("/%ld/return", nofcall);
        if (!ptr) ptr = injects.find_pointer("/-1/return");
        return ptr ? *ptr : PrototypeReturn();
    }

    boost::json::value Environment::InterfaceHandle::GetInjectionReturn(boost::json::string_view jp, int const nofcall) const
    {
        auto lock = Env().Lock();
        return GetInjectionReturn(nofcall).at_pointer(jp);
    }

    void Environment::InterfaceHandle::InjectReturn(boost::json::value const& value, boost::json::string_view jp, int const nofcall)
    {
        auto lock = Env().Lock();

        auto signal_node = GetInjectionReturn(nofcall);
        signal_node.set_at_pointer(jp, value);
        injects("/%ld/return", nofcall) = signal_node;
    }

    boost::json::value Environment::InterfaceHandle::GetInjectionArgs(int const nofcall) const
    {
        auto lock = Env().Lock();
        auto ptr = injects.find_pointer("/%ld/args", nofcall);
        if(!ptr) {
            ptr = injects.find_pointer("/-1/args");
        }
        return ptr ? *ptr : PrototypeArgs();
    }

    boost::json::value Environment::InterfaceHandle::GetInjectionArgs(boost::json::string_view jp, int const nofcall) const
    {
        auto lock = Env().Lock();
        return GetInjectionArgs(nofcall).at_pointer(jp);
    }


    void Environment::InterfaceHandle::InjectArgs(boost::json::value const& value, boost::json::string_view jp, int const nofcall)
    {
        auto lock = Env().Lock();
        auto signal_node = GetInjectionArgs(nofcall);
        signal_node.set_at_pointer(jp, value);
        injects("/%ld/args", nofcall) = signal_node;
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
        // handle 1-based indexation
        if (start > 0) { start -= 1; }
        if (stop  > 0) { stop  -= 1; }
        auto lock = Env().Lock();
        return slice(captures().as_array(), signal_path, start, stop, step);
    }

    boost::json::array const& Environment::InterfaceHandle::Captures()
    {
        auto lock = Env().Lock();
        return captures().as_array();
    }

    void Environment::InterfaceHandle::SetInjectsRange(boost::json::value const& values, boost::json::string_view signal_path)
    {
        boost::json::object inject_map {};
        if (values.is_object())
        {
            inject_map = values.get_object();
        }
        else if (values.is_array())
        {
            auto arr = values.get_array();
            for (std::size_t i = 0; i < arr.size(); i++)
            {
                inject_map.emplace(std::to_string(i+1), arr.at(i));
            }
        }
        else {
            throw environment_error("SetInjectsRange: invalid range values");
        }

        if (signal_path.empty())
        {
            throw environment_error("SetInjectsRange: empty signal_path (shall start with group name)");
        }
        auto const group = signal_path.substr(1, signal_path.find('/',2)-1);
        auto const path_in_group = signal_path.substr(group.size()+1);

        if (("args" != group) && ("return" != group) && ("exception" != group))
        {
            throw environment_error("SetInjectsRange: invalid injection group: %s", group);
        }
        else if ("exception" == group)
        {
            throw environment_error("SetInjectsRange: exception injection not implemented");
        }

        for (auto const& kv : inject_map) {
            auto const& idx  = kv.key();
            auto const& value =  kv.value();

            boost::json::value& injection_at_call = injects("/%ld/%s", idx, group);
            if (injection_at_call.is_null())
            {
                injection_at_call = env.data_->json_data.at("/prototypes/%s/%s", interface(), group);
            }

            injection_at_call.set_at_pointer(path_in_group, value);
        }
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

    Environment::InterfaceHandle& Environment::InterfaceHandle::RunAsTrigger()
    {
        // TODO: make it thread safe without recursive mutex
        boost::json::string_view ref = key();
        auto const& triggers = Env().data_->triggers;
        if (0 == triggers.count(ref))
        {
            throw environment_error("Trigger %s is not registered!", ref);
        }

        Trigger const& trigger = triggers.at(ref);

        try
        {
            boost::json::value capture = trigger(GetInjectionArgs(-1));
            captures("/+") = capture;
        }
        catch(const std::exception& e)
        {
            throw environment_error("Trigger %s error: `%s`", ref, e.what());
        }

        return *this;
    }

}  // namespace zmbt
