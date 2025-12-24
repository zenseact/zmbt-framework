

# File environment\_interface\_record.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**environment\_interface\_record.hpp**](environment__interface__record_8hpp.md)

[Go to the documentation of this file](environment__interface__record_8hpp.md)


```C++

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

    Environment& Env() const
    {
        return env;
    }

    boost::json::value PrototypeReturn() const;

    boost::json::array PrototypeArgs() const;

    void EnableOutputRecordFor(ChannelKind const kind);

    void MaybeThrowException();

    boost::json::value YieldInjection(ChannelKind const kind);

    boost::json::value YieldInjectionArgs()
    {
        return YieldInjection(ChannelKind::Args);
    }

    boost::json::value YieldInjectionReturn()
    {
        return YieldInjection(ChannelKind::Return);
    }

    void Inject(std::shared_ptr<Generator> gen, lang::Expression const& tf, ChannelKind const kind, boost::json::string_view jp = "");



    void InjectReturn(lang::Expression const& e, boost::json::string_view jp = "")
    {
        return Inject(std::make_shared<Generator>(e), expr::Id, ChannelKind::Return, jp);
    }

    void InjectArgs(lang::Expression const& e, boost::json::string_view jp = "")
    {
        return Inject(std::make_shared<Generator>(e), expr::Id, ChannelKind::Args, jp);
    }


    std::size_t ObservedCalls() const;



    boost::json::array CaptureSlice(boost::json::string_view signal_path) const;

    boost::json::array const& Captures() const;

    boost::json::string const& key() const;

    InterfaceHandle& RunAsAction();

    InterfaceHandle& RunAsTrigger(std::size_t const repeats = 1);
};

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
                {"interface", interface().str()               },
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
            {"interface", interface().str()                    },
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
        boost::json::value result(YieldInjection(ChannelKind::Return));
        return dejsonize<T>(result);
    }
    catch(const std::exception& e)
    {
        env.SetTestError({
            {"error"    , "exception thrown at return evaluation"},
            {"interface", interface().str()                      },
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
        auto const key = format("$(ret-ref-%s-%s)", interface().key(), refobj().key());
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

    return_t Hook(hookout_args_t args)
    {
        std::size_t nofcall;
        boost::json::value result;
        HookArgsImpl(args);
        MaybeThrowException();
        return HookReturnImpl(type<return_t>);
    }

    template <class... A>
    return_t Hook(A&&... arg)
    {
        hookout_args_t args {arg...};
        return Hook(args);
    }
};


template <class Interface>
Environment::TypedInterfaceHandle<Interface> InterfaceRecord(Interface const& interface, object_id const& obj = {ifc_host_nullptr<Interface>})
{
    Environment env {};
    env.InitializeInterfaceHandlers(interface);
    return {interface, obj};
}


}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_INTERFACE_RECORD_HPP_
```


