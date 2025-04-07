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
#include "expression.hpp"




namespace zmbt {



/**
 * @brief Environment extension handling the interface data
 *
 * @tparam Interface
 */

class Environment::InterfaceHandle
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

    InterfaceHandle(Environment const& e, object_id refobj, interface_id const& interface);
    InterfaceHandle(Environment const& e, boost::json::string_view ref);

  public:

    InterfaceHandle(object_id refobj, interface_id const& interface);

    InterfaceHandle(nullptr_t, interface_id const& interface);

    InterfaceHandle(boost::json::string_view ref);


    template<class H, class E = mp_if<mp_not<is_pointer<H>>, void>>
    InterfaceHandle(H const& obj, interface_id const& interface)
        : InterfaceHandle(std::addressof(obj), interface)
    {
    }

    InterfaceHandle(InterfaceHandle const&) = default;
    InterfaceHandle(InterfaceHandle && o) = default;
    InterfaceHandle& operator=(InterfaceHandle const&) = default;
    InterfaceHandle& operator=(InterfaceHandle &&) = default;
    ~InterfaceHandle() = default;

    /**
     * @brief Environment of the interface handle
     *
     * @return zmbt::Environment&
     */
    Environment& Env() const
    {
        return env;
    }

    boost::json::value const& PrototypeReturn() const;

    boost::json::value const& PrototypeArgs() const;

    /// \brief Injection at nofcall
    /// \param group return | args | exception
    /// \param jp JSON pointer
    /// \param nofcall number of call
    boost::json::value GetInjection(boost::json::string_view group, boost::json::string_view jp, int const nofcall);

    /// \brief Injection return signal at nofcall
    /// \param jp JSON Pointer
    /// \param nofcall
    boost::json::value GetInjectionReturn(boost::json::string_view jp, std::size_t const nofcall = 0)
    {
        return GetInjection("return", jp, nofcall);
    }

    /// \brief Injection return signal at nofcall
    /// \param nofcall
    boost::json::value GetInjectionReturn(std::size_t const nofcall = 0)
    {
        return GetInjectionReturn("", nofcall);
    }

    /// \brief Injection args subsignal at nofcall
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall
    boost::json::value GetInjectionArgs(boost::json::string_view jp, std::size_t const nofcall = 0)
    {
        return GetInjection("args", jp, nofcall);
    }


    /// \brief Injection args at nofcall
    /// \param value
    /// \param nofcall
    boost::json::value GetInjectionArgs(std::size_t const nofcall = 0)
    {
        return GetInjectionArgs("", nofcall);
    }


    /// \brief Set injection return subsignal at nofcall
    /// \param e expression
    /// \param op operator handler id
    /// \param group return | args | exception
    /// \param jp JSON Pointer
    void Inject(Expression const& e, boost::json::string_view op, boost::json::string_view group, boost::json::string_view jp = "");

    /// \brief Set injection return subsignal at nofcall
    /// \param e
    /// \param jp JSON Pointer
    void InjectReturn(Expression const& e, SignalOperatorHandler const& op, boost::json::string_view jp = "")
    {
        return Inject(e, op.annotation(), "return", jp);
    }

    /// \brief Set injection return subsignal at nofcall
    /// \param e
    /// \param jp JSON Pointer
    void InjectReturn(Expression const& e, boost::json::string_view jp = "")
    {
        return InjectReturn(e, {}, jp);
    }


    /// \brief Set injection args subsignal at nofcall
    /// \param e
    /// \param jp JSON Pointer
    void InjectArgs(Expression const& e, SignalOperatorHandler const& op, boost::json::string_view jp = "")
    {
        return Inject(e, op.annotation(), "args", jp);
    }

    /// \brief Set injection args subsignal at nofcall
    /// \param e
    /// \param jp JSON Pointer
    void InjectArgs(Expression const& e, boost::json::string_view jp = "")
    {
        return InjectArgs(e, {}, jp);
    }


    /// Number of interface calls
    std::size_t ObservedCalls() const;


    /// Input arguments observed at nofcall
    boost::json::array ObservedArgs(int const nofcall = -1);


    boost::json::array CaptureSlice(boost::json::string_view signal_path, int start = 0, int stop = -1, int const step = 1);

    boost::json::array const& Captures();

    /// Observed return value at nofcall
    boost::json::value ObservedReturn(int const nofcall = -1);

    boost::json::string const& key() const;

    InterfaceHandle& RunAsAction();

    InterfaceHandle& RunAsTrigger(std::size_t const nofcall = 0);
};

/// @brief Environment API handler for specific interface
template <class Interface>
class Environment::TypedInterfaceHandle : public Environment::InterfaceHandle
{

    using reflection  =  reflect::invocation<Interface const&>;
    using return_t    = typename reflection::return_t;
    using args_t      = typename reflection::args_t;
    using unqf_args_t = tuple_unqf_t<args_t>;

    template <class T>
    using rvalue_reference_to_value = mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;

    using hookout_args_t = mp_transform<rvalue_reference_to_value, args_t>;

    std::size_t HookImpl(hookout_args_t & args)
    {
        auto const ts = get_ts();
        std::string const tid = get_tid();
        auto lock = Env().Lock();

        captures("/+") = {
            {"ts", ts},
            {"tid", tid },
            {"args", json_from(convert_tuple_to<unqf_args_t>(args))}
        };

        auto nofcall = captures().as_array().size() - 1;
        auto const injection = GetInjectionArgs(nofcall).as_array();
        if (injection.size() != std::tuple_size<unqf_args_t>())
        {
            throw model_error("invalid inject arguments arity");
        }

        auto args_out = dejsonize<unqf_args_t>(injection);
        tuple_exchange(args, args_out);

        // Produce
        return nofcall;
    }

    public:


    template <class H>
    TypedInterfaceHandle(H const& refobj, interface_id const& interface)
        : Environment::InterfaceHandle(refobj, interface)
    {
    }

    TypedInterfaceHandle(TypedInterfaceHandle const&) = default;
    TypedInterfaceHandle(TypedInterfaceHandle &&) = default;
    ~TypedInterfaceHandle() = default;

    /**
     * @brief Hook the producing interface call to the environment
     *
     * @details Register input arguments, update the output reference arguments,
     * increment call count and return corresponding env value
     *
     * @param args
     * @return return_t
     */
    return_t Hook(hookout_args_t args)
    {
        std::size_t nofcall;
        boost::json::value result;

        try
        {
            nofcall = HookImpl(args);
        }
        catch(const std::exception& e)
        {
            throw model_error("Hook #%d %s capture error: `%s`, args: %s"
                , nofcall, interface()
                , e.what(), json_from(args));
            return dejsonize<return_t>(nullptr);
        }

        try
        {
            result = GetInjectionReturn(nofcall);
        }
        catch(const std::exception& e)
        {
            throw model_error("Hook #%d %s return evaluation error: `%s`", nofcall, interface(), e.what());
            return dejsonize<return_t>(nullptr);
        }

        try
        {
            return dejsonize<return_t>(result);
        }
        catch(const std::exception& e)
        {
            throw model_error("Hook #%d %s return evaluation error, can't deserialize %s as %s",
                nofcall, interface(), boost::json::serialize(result), type_name<return_t>());
            return dejsonize<return_t>(nullptr);
        }
    }

    /**
     * @brief Hook the producing interface call to the environment
     *
     * @details Register input arguments, update the output reference arguments,
     * increment call count and return corresponding env value
     *
     * @tparam A... compatible arg types
     * @param arg
     * @return return_t
     */
    template <class... A>
    return_t Hook(A&&... arg)
    {
        hookout_args_t args {arg...};
        return Hook(args);
    }
};


/**
 * @brief Make TypedInterfaceHandle instance
 *
 * @tparam I
 * @param obj
 * @param interface
 * @return TypedInterfaceHandle<I>
 */
template <class H, class I>
Environment::TypedInterfaceHandle<I> InterfaceRecord(H const& obj, I const& interface)
{
    Environment env {};
    env.RegisterPrototypes(interface);
    return {obj, interface};
}


/**
 * @brief Make TypedInterfaceHandle instance
 *
 * @tparam I
 * @param interface
 * @return TypedInterfaceHandle<I>
 */
template <class I>
Environment::TypedInterfaceHandle<I> InterfaceRecord(I const& interface) {
    Environment env {};
    env.RegisterPrototypes(interface);
    return {ifc_host_nullptr<I>, interface};
}



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_INTERFACE_RECORD_HPP_
