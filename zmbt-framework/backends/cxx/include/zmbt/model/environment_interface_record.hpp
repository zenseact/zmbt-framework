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
#include "zmbt/expr/expression.hpp"




namespace zmbt {



/**
 * @brief Environment extension handling the interface data
 *
 * @tparam Interface
 */

class Environment::InterfaceHandle
{

private:
    object_id refobj_;
    interface_id interface_;

protected:

    mutable zmbt::Environment env;
    std::shared_ptr<OutputRecorder> output_recorder_;
public:


    interface_id interface() const
    {
        return interface_;
    }

    object_id refobj() const
    {
        return refobj_;
    }

    InterfaceHandle(Environment const& e, interface_id const& interface, object_id refobj);
    InterfaceHandle(Environment const& e, boost::json::string_view ref);

  public:

    InterfaceHandle(interface_id const& interface, object_id refobj);

    InterfaceHandle(interface_id const& interface, nullptr_t);

    InterfaceHandle(boost::json::string_view ref);


    template<class H, class E = mp_if<mp_not<is_pointer<H>>, void>>
    InterfaceHandle(interface_id const& interface, H const& obj)
        : InterfaceHandle(interface, std::addressof(obj))
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

    boost::json::array const& PrototypeArgs() const;

    void EnableOutputRecordFor(ChannelKind const kind);

    /// throw exception if set for current call
    void MaybeThrowException();

    /// \brief Yield input generator
    boost::json::value YieldInjection(ChannelKind const kind);

    /// \brief Injection at nofcall
    boost::json::value YieldInjectionArgs()
    {
        return YieldInjection(ChannelKind::Args);
    }

    boost::json::value YieldInjectionReturn()
    {
        return YieldInjection(ChannelKind::Return);
    }

    /// \brief Set generating function for injection
    /// \param e input generator
    /// \param group return | args | exception
    /// \param jp JSON Pointer
    void Inject(std::shared_ptr<Generator> gen, lang::Expression const& tf, ChannelKind const kind, boost::json::string_view jp = "");



    /// \brief Set generating function for injection return
    /// \details The intput conditions are evaluated from the corresponding expressions with
    /// 0-based call index as an argument.
    /// \param e generating funcion expression
    /// \param jp JSON Pointer
    void InjectReturn(lang::Expression const& e, boost::json::string_view jp = "")
    {
        return Inject(std::make_shared<Generator>(e), expr::Noop, ChannelKind::Return, jp);
    }

    /// \brief Set generating function for injection args
    /// \details The intput conditions are evaluated from the corresponding expressions with
    /// 0-based call index as an argument.
    /// \param e
    /// \param jp JSON Pointer
    void InjectArgs(lang::Expression const& e, boost::json::string_view jp = "")
    {
        return Inject(std::make_shared<Generator>(e), expr::Noop, ChannelKind::Args, jp);
    }


    /// Number of interface calls
    std::size_t ObservedCalls() const;



    boost::json::array CaptureSlice(boost::json::string_view signal_path) const;

    boost::json::array const& Captures() const;

    boost::json::string const& key() const;

    InterfaceHandle& RunAsAction();

    InterfaceHandle& RunAsTrigger(std::size_t const repeats = 1);
};

/// @brief Environment API handler for specific interface
template <class Interface>
class Environment::TypedInterfaceHandle : public Environment::InterfaceHandle
{

    using reflection  =  reflect::invocation<Interface const&>;
    using return_t    = typename reflection::return_t;
    using args_t      = typename reflection::args_t;
    using unqf_args_t = tuple_unqf_t<args_t>;

    using return_or_nullptr_t = reflect::invocation_ret_unqf_or_nullptr_t<Interface const&>;


    template <class T>
    using rvalue_reference_to_value = mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;

    using hookout_args_t = mp_transform<rvalue_reference_to_value, args_t>;

    void HookArgsImpl(hookout_args_t & args)
    try
    {
        output_recorder_->push(convert_tuple_to<unqf_args_t>(args), ErrorOr<return_or_nullptr_t>());


        auto const injection = YieldInjection(ChannelKind::Args).as_array();

        if (injection.size() != std::tuple_size<unqf_args_t>())
        {
            env.SetTestError({
                {"error"    , "invalid inject arguments arity"},
                {"injection", injection},
                {"interface", interface()                     },
                {"context"  , "Hook"                          },
                {"injection", injection                       },
            });
        }
        else
        {
            auto args_out = dejsonize<unqf_args_t>(injection);
            tuple_exchange(args, args_out);
        }
    }
    catch(const std::exception& e)
    {
        env.SetTestError({
            {"error"    , "exception thrown at args evaluation"},
            {"interface", interface()                          },
            {"context"  , "Hook"                               },
            {"what"     , e.what()                             },
        });
    }


    void HookReturnImpl(type_tag<void>)
    {
    }

    template <class T>
    auto HookReturnImpl(type_tag<T>) -> mp_if<mp_not<is_reference<T>>, T>
    try
    {
        boost::json::value result = YieldInjection(ChannelKind::Return);
        return dejsonize<T>(result);
    }
    catch(const std::exception& e)
    {
        env.SetTestError({
            {"error"    , "exception thrown at return evaluation"},
            {"interface", interface()                            },
            {"context"  , "Hook"                                 },
            {"what"     , e.what()                               },
        });
        return dejsonize<T>(PrototypeReturn());
    }

    template <class T>
    auto HookReturnImpl(type_tag<T>) -> mp_if<is_reference<T>, T>
    {
        using TT = remove_cvref_t<T>;

        TT value = HookReturnImpl(type<TT>);
        auto const key = format("$(ret-ref-%s-%s)", interface(), refobj());
        TT& ref = Env().template GetSharedRef<TT>(key, reflect::signal_traits<TT>::init());
        ref = value;
        return ref;
    }



    public:


    template <class H>
    TypedInterfaceHandle(interface_id const& interface, H const& refobj)
        : Environment::InterfaceHandle(interface, refobj)
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
        HookArgsImpl(args);
        MaybeThrowException();
        return HookReturnImpl(type<return_t>);
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
template <class I>
Environment::TypedInterfaceHandle<I> InterfaceRecord(I const& interface, object_id const& obj = {ifc_host_nullptr<I>})
{
    Environment env {};
    env.InitializeInterfaceHandlers(interface);
    return {interface, obj};
}


}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_INTERFACE_RECORD_HPP_
