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

    /// \brief Injection return signal at nofcall
    /// \param nofcall number of call
    /// \param jp JSON Pointer
    /// \return
    boost::json::value GetInjectionReturn(boost::json::string_view jp, int const nofcall = -1) const;

    /// \brief Injection return signal at nofcall
    /// \param nofcall number of call
    /// \return
    boost::json::value GetInjectionReturn(int const nofcall = -1) const;


    /// \brief Set injection return subsignal at nofcall
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall number of call
    void InjectReturn(boost::json::value const& value, boost::json::string_view jp = "", int const nofcall = -1);

    /// \brief Set injection return subsignal at nofcall
    /// \tparam T
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall number of call
    template <class T>
    void InjectReturn(T value, boost::json::string_view jp = "", int const nofcall = -1)
    {
        // TODO: handle conversion error
        InjectReturn(json_from(value), jp, nofcall);
    }


    /// \brief Injection args subsignal at nofcall
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall number of call
    boost::json::value GetInjectionArgs(boost::json::string_view jp, int const nofcall = -1) const;

    /// \brief Injection args subsignal at nofcall
    /// \param value
    /// \param nofcall number of call
    boost::json::value GetInjectionArgs(int const nofcall = -1) const;

    /// \brief Set injection args subsignal at nofcall
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall number of call
    void InjectArgs(boost::json::value const& value, boost::json::string_view jp = "", int const nofcall = -1);

    /// \brief Set injection args subsignal at nofcall
    /// \tparam T
    /// \param value
    /// \param jp JSON Pointer
    /// \param nofcall number of call
    template <class T>
    void InjectArgs(T value, boost::json::string_view jp = "", int const nofcall = -1)
    {
        // TODO: handle conversion error
        InjectArgs(json_from(value), jp, nofcall);
    }

    /// Number of interface calls
    std::size_t ObservedCalls() const;


    /// Input arguments observed at nofcall
    boost::json::array ObservedArgs(int const nofcall = -1);


    boost::json::array CaptureSlice(boost::json::string_view signal_path, int start = 0, int stop = -1, int const step = 1);

    boost::json::array const& Captures();

    /// @brief Set injects range
    /// values should be either a mapping {numof_call -> value} or an array.
    /// @param values values to inject
    /// @param signal_path signal path to update at each injection
    void SetInjectsRange(boost::json::value const& values, boost::json::string_view signal_path);


    /// Observed return value at nofcall
    boost::json::value ObservedReturn(int const nofcall = -1);

    boost::json::string const& key() const;

    InterfaceHandle& RunAsAction();

    InterfaceHandle& RunAsTrigger();
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

        auto nofcall = captures().as_array().size();
        auto const injection = GetInjectionArgs(nofcall).as_array();
        if (injection.size() != std::tuple_size<unqf_args_t>())
        {
        }
        try
        {
            auto args_out = dejsonize<unqf_args_t>(injection);
            tuple_exchange(args, args_out);
        }
        catch(const std::exception& e)
        {
            throw environment_error("InterfaceRecord<%s>::Hook failed with error `%s`",
                interface().annotation(),
                e.what()
            );
        }

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
        auto const nofcall = HookImpl(args);
        return dejsonize<return_t>(GetInjectionReturn(nofcall));
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
        auto const nofcall = HookImpl(args);
        return dejsonize<return_t>(GetInjectionReturn(nofcall));
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
