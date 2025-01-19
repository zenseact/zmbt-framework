/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_ENVIRONMENT_INTERFACE_RECORD_HPP_
#define ZMBT_MODEL_ENVIRONMENT_INTERFACE_RECORD_HPP_


#include <chrono>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>

#include "environment.hpp"



namespace zmbt {



/**
 * @brief Environment extension handling the interface data
 *
 * @tparam Interface
 */

class Environment::IfcRec
{

    mutable zmbt::Environment env;
    object_id refobj_;
    interface_id interface_;

protected:

    JsonNode captures;
    JsonNode injects;

public:

    interface_id interface() const
    {
        return interface_;
    }

    object_id refobj() const
    {
        return refobj_;
    }

    IfcRec(Environment const& e, object_id refobj, interface_id const& interface)
        : env{e}
        , refobj_{refobj}
        , interface_{interface}
    {
        auto lock = Env().Lock();
        auto& ifc_rec = Env().data_->json_data;
        captures = ifc_rec.branch(boost::json::kind::array, "/interface_records/%s/%s/captures", refobj, interface);
        injects = ifc_rec.branch(boost::json::kind::object, "/interface_records/%s/%s/injects", refobj, interface);
    }

    IfcRec(Environment const& e, boost::json::string_view ref) : IfcRec(
        e,
        e.ObjectId(ref),
        e.InterfaceId(ref)
    )
    {
    }

  public:

    IfcRec(object_id refobj, interface_id const& interface)
        : IfcRec(Environment {}, refobj, interface)
    {
    }

    IfcRec(nullptr_t, interface_id const& interface)
        : IfcRec(object_id(nullptr), interface)
    {
    }

    IfcRec(boost::json::string_view ref) : IfcRec(Environment {}, ref)
    {
    }


    template<class H, class E = mp_if<mp_not<is_pointer<H>>, void>>
    IfcRec(H const& obj, interface_id const& interface)
        : IfcRec(std::addressof(obj), interface)
    {
    }

    IfcRec(IfcRec const&) = default;
    IfcRec(IfcRec && o) = default;
    IfcRec& operator=(IfcRec const&) = default;
    IfcRec& operator=(IfcRec &&) = default;
    ~IfcRec() = default;

    /**
     * @brief Environment of the interface handle
     *
     * @return zmbt::Environment&
     */
    Environment& Env() const
    {
        return env;
    }

    boost::json::value PrototypeReturn() const
    {
        auto lock = Env().Lock();
        return env.data_->json_data.at("/prototypes/%s/return", interface());
    }

    boost::json::array PrototypeArgs() const
    {
        auto lock = Env().Lock();
        return env.data_->json_data.at("/prototypes/%s/args", interface()).as_array();
    }

    /**
     * @brief Get the registered interface return value
     *
     * @return return_t
     */
    boost::json::value GetInjectionReturn(int const nofcall = -1) const
    {
        auto lock = Env().Lock();
        auto ptr = injects.find_pointer("/%ld/return", nofcall);
        if (!ptr) ptr = injects.find_pointer("/-1/return");
        return ptr ? *ptr : PrototypeReturn();
    }


    /**
     * @brief Register the interface return value
     *
     * @param value
     */
    //@{
    void InjectReturn(boost::json::value value, int const nofcall = -1)
    {
        auto lock = Env().Lock();
        injects("/%ld/return", nofcall) = value;
    }

    template <class T>
    void InjectReturn(T value, int const nofcall = -1)
    {
        // TODO: handle conversion error
        InjectReturn(json_from(value), nofcall);
    }
    //@}


    /**
     * @brief Get the registered interface arguments tuple
     *
     * @return unqf_args_t
     */
    boost::json::array GetInjectionArgs(int const nofcall = -1) const
    {
        auto lock = Env().Lock();
        auto ptr = injects.find_pointer("/%ld/args", nofcall);
        if(!ptr) {
            ptr = injects.find_pointer("/-1/args");
        }
        return ptr ? ptr->as_array() : PrototypeArgs();
    }


    /**
     * @brief Register the interface arguments
     *
     * @param args
     */
    //@{
    void InjectArgs(boost::json::array args, int const nofcall = -1)
    {
        auto lock = Env().Lock();
        injects("/%ld/args", nofcall) = args;
    }

    /**
     * @brief Number of interface calls
     *
     * @return std::size_t
     */
    std::size_t ObservedCalls() const
    {
        auto lock = Env().Lock();
        return captures().as_array().size();
    }


    boost::json::array ObservedArgs(int const nofcall = -1)
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


    boost::json::array CaptureSlice(boost::json::string_view signal_path, int start = 0, int stop = -1, int const step = 1)
    {
        // handle 1-based indexation
        if (start > 0) { start -= 1; }
        if (stop  > 0) { stop  -= 1; }
        auto lock = Env().Lock();
        return slice(captures().as_array(), signal_path, start, stop, step);
    }

    boost::json::array const& Captures()
    {
        auto lock = Env().Lock();
        return captures().as_array();
    }

    /// @brief Set injects range
    /// values should be either a mapping {numof_call -> value} or an array.
    /// @param values values to inject
    /// @param signal_path signal path to update at each injection
    void SetInjectsRange(boost::json::value const& values, boost::json::string_view signal_path)
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


    boost::json::value ObservedReturn(int const nofcall = -1)
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

    boost::json::string const& key() const
    {
        auto lock = Env().Lock();
        return Env().json_data().at("/refs/ids2key/%s/%s", refobj(), interface()).as_string();
    }



    IfcRec& RunAsAction()
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

    IfcRec& RunAsTrigger()
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
};

/// @brief Environment API handler for specific interface
template <class Interface>
class TypedInterfaceRecord : public Environment::IfcRec
{

    using reflection  =  reflect::invocation<Interface const&>;
    using return_t    = typename reflection::return_t;
    using args_t      = typename reflection::args_t;
    using unqf_args_t = tuple_unqf_t<args_t>;

    template <class T>
    using rvalue_reference_to_value = mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;

    using hookout_args_t = mp_transform<rvalue_reference_to_value, args_t>;

    template <class A>
    std::size_t HookImpl(A & args)
    {
        auto const ts = get_ts();
        std::string const tid = get_tid();
        auto lock = Env().Lock();

        captures("/+") = {
            {"ts", ts},
            {"tid", tid },
            {"args", json_from(convert_tuple_to<unqf_args_t>(args))}
        };

        auto nofcall = captures().as_array().size();

        auto args_out = dejsonize<unqf_args_t>(GetInjectionArgs(nofcall));

        // Produce
        auto args_hook = convert_tuple_to<hookout_args_t>(args);
        tuple_exchange(args_hook, args_out);
        return nofcall;
    }

    public:


    template <class H>
    TypedInterfaceRecord(H const& refobj, interface_id const& interface)
        : Environment::IfcRec(refobj, interface)
    {
    }

    TypedInterfaceRecord(TypedInterfaceRecord const&) = default;
    TypedInterfaceRecord(TypedInterfaceRecord &&) = default;
    ~TypedInterfaceRecord() = default;

    /**
     * @brief Hook the producing interface call to the environment
     *
     * @details Register input arguments, update the output reference arguments,
     * increment call count and return corresponding env value
     *
     * @param args
     * @return return_t
     */
    //@{
    return_t Hook(hookout_args_t args)
    {
        auto const nofcall = HookImpl(args);
        return dejsonize<return_t>(GetInjectionReturn(nofcall));
    }

    template <class... A>
    return_t Hook(A&&... arg)
    {
        hookout_args_t args {arg...};
        auto const nofcall = HookImpl(args);
        return dejsonize<return_t>(GetInjectionReturn(nofcall));
    }
    //@}
};


/**
 * @brief Make TypedInterfaceRecord instance
 *
 * @tparam I
 * @param obj
 * @param interface
 * @return TypedInterfaceRecord<I>
 */
template <class H, class I>
TypedInterfaceRecord<I> InterfaceRecord(H const& obj, I const& interface)
{
    Environment env {};
    env.RegisterPrototypes(interface);
    return {obj, interface};
}


/**
 * @brief Make TypedInterfaceRecord instance
 *
 * @tparam I
 * @param interface
 * @return TypedInterfaceRecord<I>
 */
template <class I>
TypedInterfaceRecord<I> InterfaceRecord(I const& interface) {
    Environment env {};
    env.RegisterPrototypes(interface);
    return {ifc_host_nullptr<I>, interface};
}



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_INTERFACE_RECORD_HPP_
