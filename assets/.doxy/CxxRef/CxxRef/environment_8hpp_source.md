

# File environment.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**environment.hpp**](environment_8hpp.md)

[Go to the documentation of this file](environment_8hpp.md)


```C++

#ifndef ZMBT_MODEL_ENVIRONMENT_HPP_
#define ZMBT_MODEL_ENVIRONMENT_HPP_


#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/interface_id.hpp>
#include <zmbt/core/interface_traits.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/core/object_id.hpp>
#include <zmbt/reflect/signal_traits.hpp>
#include <zmbt/reflect/invocation.hpp>
#include <zmbt/reflect/prototypes.hpp>
#include <zmbt/reflect/serialization.hpp>
#include <zmbt/expr.hpp>
#include <exception>
#include <functional>
#include <iosfwd>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>

#include "environment_data.hpp"
#include "exceptions.hpp"
#include "trigger.hpp"

#include "permanent_data.hpp"

namespace zmbt {


class Environment {

    friend class OutputRecorder;
  protected:

    using lock_t = typename EnvironmentData::lock_t;

    template <class Interface>
    using return_t = reflect::invocation_ret_t<ifc_pointer_t<Interface>>;

    template <class Interface>
    using unqf_args_t = reflect::invocation_args_unqf_t<ifc_pointer_t<Interface>>;


    template <class Interface>
    using argsref_t = reflect::invocation_args_t<ifc_pointer_t<Interface>>;

    template <class T>
    using rvref_to_val = mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;

    template <class Interface>
    using hookout_args_t = mp_transform<rvref_to_val, argsref_t<Interface>>;

    std::shared_ptr<EnvironmentData> data_;
    std::shared_ptr<PermanentEnvData> permanent_data_;

    void SetTestError(boost::json::value&& msg);

  public:
    class InterfaceHandle;

    template <class Interface>
    class TypedInterfaceHandle;

    boost::json::value const& TestError();

    bool HasTestError();

    JsonNode& json_data()
    {
        return data_->json_data;
    }

    reflect::Prototypes GetPrototypes(interface_id const& id) const
    {
        auto const maybe_proto = permanent_data_->get_prototypes(id);
        if (!maybe_proto.has_value())
        {
            throw_exception(environment_error("accessing unregistered prototypes for `%s`", id));
        }
        return maybe_proto.value();
    }

    JsonNode const& json_data() const
    {
        return data_->json_data;
    }

    lock_t Lock() const;

    lock_t TryLock() const;

    lock_t DeferLock() const;

    Environment();

    Environment(Environment &&) = default;

    Environment(Environment const&) = default;

    Environment& operator=(Environment &&) = default;

    Environment& operator=(Environment const&) = default;


    virtual ~Environment()
    {
    }

    void DumpToJsonLog();


    void SetVar(lang::Expression const& key_expr, boost::json::value var);

    template <class T>
    void SetVar(lang::Expression const& key_expr, T var)
    {
        return SetVar(key_expr, json_from(var));
    }


    boost::json::value GetVarOrUpdate(lang::Expression const& key_expr, boost::json::value update_value);

    template <class T>
    T GetVarOrUpdate(lang::Expression const& key_expr, T update_value)
    {
        return dejsonize<T>(GetVarOrUpdate(key_expr, json_from(update_value)));
    }


    boost::json::value GetVarOrDefault(lang::Expression const& key_expr, boost::json::value default_value = {});

    template <class T>
    T GetVarOrDefault(lang::Expression const& key_expr, T default_value = reflect::signal_traits<T>::init())
    {
        return dejsonize<T>(GetVarOrDefault(key_expr, json_from(default_value)));
    }


    boost::json::value GetVar(lang::Expression const& key_expr);

    template <class T>
    T GetVar(lang::Expression const& key_expr)
    {
        return dejsonize<T>(GetVar(key_expr));
    }


    template <class T>
    void SetShared(lang::Expression const& key_expr, std::shared_ptr<T> data)
    {
        boost::json::string const key = key_expr.eval().as_string();
        EnvironmentData::shared_data_record const record { typeid(T), data };

        auto lock = Lock();

        if (data_->shared.count(key)){
            data_->shared.at(key) = record;
        }
        else {
            data_->shared.emplace(key, record);
        }
    }


    template <class T>
    std::shared_ptr<T> GetShared(lang::Expression const& key_expr) const
    {
        boost::json::string const key = key_expr.eval().as_string();
        auto found = data_->shared.cend();
        {
            auto lock = Lock();
            found = data_->shared.find(key);
        }
        if (found == data_->shared.cend())
        {
            return nullptr;
        }

        auto const record = found->second;
        if (std::type_index(typeid(T)) != record.first)
        {
            throw_exception(environment_error("GetShared invoked with incompatible type for `%s`", key));
        }

        return std::static_pointer_cast<T>(record.second);
    }

    template <class T, class... A>
    T& GetSharedRef(lang::Expression const& key_expr, A&&... args)
    {
        boost::json::string const key = key_expr.eval().as_string();
        auto lock = Lock();
        auto found = data_->shared.find(key);
        if (found == data_->shared.cend())
        {
            auto const shared = std::make_shared<T>(std::forward<A>(args)...);
            EnvironmentData::shared_data_record const record { typeid(T), shared };

            if (not data_->shared.emplace(key, record).second)
            {
                throw_exception(environment_error("GetSharedRef failed to create shared object at `%s`", key));
            }
            return *shared;
        }

        auto const record = found->second;
        if (std::type_index(typeid(T)) != record.first)
        {
            throw_exception(environment_error("GetSharedRef invoked with incompatible type for `%s`", key));
        }

        return *std::static_pointer_cast<T>(record.second);
    }

    bool ContainsShared(lang::Expression const& key_expr) const;


    void ResetInterfaceData();


    void ResetAll();


    Environment& RegisterAction(lang::Expression const& key_expr, std::function<void()> action);


    Environment& RunAction(lang::Expression const& key_expr);
    Environment& RunActionNoCatch(lang::Expression const& key_expr);


    template <class Interface>
    interface_id RegisterParametricTriggerIfc(Interface&& interface)
    {
        InitializeInterfaceHandlers(std::forward<Interface>(interface));
        TriggerIfc trigger_ifc{std::forward<Interface>(interface)};
        interface_id ifc_id{trigger_ifc.id()};
        auto key = format("/trigger_ifcs/%s", ifc_id);
        auto lock = Lock();
        data_->trigger_ifcs.emplace(ifc_id, std::move(trigger_ifc));
        data_->json_data(key) = 0; // TODO: timestamp
        return ifc_id;
    }

    template <class T>
    object_id RegisterParametricTriggerObj(T&& obj)
    {
        TriggerObj trigger_obj{std::forward<T>(obj)};
        object_id obj_id{trigger_obj.id()};
        auto key = format("/trigger_objs/%s", obj_id);

        auto lock = Lock();
        data_->trigger_objs.emplace(obj_id, std::move(trigger_obj));
        data_->json_data(key) = 0; // TODO: timestamp
        return obj_id;
    }

    static boost::json::string autokey(object_id const& obj_id, interface_id const& ifc_id)
    {
        return {format("%s:%s", obj_id.key(), ifc_id.key())};
    }


    template <class Interface>
    std::shared_ptr<OutputRecorder> GetRecorder(Interface&& interface)
    {
        return GetRecorder(std::forward<Interface>(interface), ifc_host_nullptr<Interface>);
    }

    std::shared_ptr<OutputRecorder> GetRecorder(interface_id const& ifc_id, object_id const& obj_id)
    {
        std::shared_ptr<OutputRecorder> recorder;

        data_->output_recorders.try_emplace_or_visit(std::make_pair(ifc_id, obj_id),
            [&recorder, &ifc_id, &obj_id]() -> std::shared_ptr<OutputRecorder> {
                recorder = std::make_shared<OutputRecorder>(ifc_id, obj_id);
                return recorder;
            },
            [&recorder](auto& record){
                recorder = record.second;
            }
        );
        return recorder;
    }

    boost::json::string GetOrRegisterParametricTrigger(object_id const& obj_id, interface_id const& ifc_id);

    template <class Interface, class H>
    Environment& RegisterTrigger(boost::json::string_view key, Interface&& interface, H&& host)
    {

        interface_id const ifc_id{std::forward<Interface>(interface)};
        object_id const obj_id{host};

        auto recorder = GetRecorder(ifc_id, obj_id);

        Trigger trigger{std::forward<H>(host), interface, recorder};
        auto const json_data_key = format("/triggers/%s", key);
        auto const err_msg = format("Trigger registering failed: key \"%s\" is taken", key);
        auto lock = Lock();
        auto const trigger_found =  data_->triggers.find(key);
        if (trigger_found != data_->triggers.cend())
        {
            if (trigger_found->second != trigger)
            {
                throw_exception(environment_error(err_msg));
            }
            return *this;
        }
        data_->triggers.emplace(key, trigger);
        data_->json_data(json_data_key) = 0; // TODO: timestamp
        RegisterInterface(key, interface, trigger.obj_id());
        return *this;
    }


    template <class Interface>
    Environment& RegisterTrigger(boost::json::string_view key, Interface&& interface)
    {
        return RegisterTrigger(key, std::forward<Interface>(interface), ifc_host_nullptr<Interface>);
    }

    template <class Interface, class H>
    boost::json::string RegisterAnonymousTrigger(Interface&& interface, H&& host)
    {
        auto key = autokey(host, interface);
        RegisterTrigger(key, interface, host);
        return key;
    }


    bool HasTrigger(boost::json::string_view key) const;

    bool HasAction(boost::json::string_view key) const;


    template <class Interface>
    Environment& InitializeInterfaceHandlers(Interface&& interface)
    {
        static_assert(is_ifc_handle<Interface>::value, "");
        auto const ifc_id = interface_id(std::forward<Interface>(interface));

        permanent_data_->prototypes.emplace(ifc_id, interface);
        permanent_data_->default_objects.emplace(ifc_id, ifc_host_nullptr<Interface>);
        permanent_data_->output_recorder_factories.emplace(ifc_id, [](OutputRecorder& rec){
            rec.setup_handlers<Interface>();
        });
        return *this;
    }

    Environment& RegisterInterface(boost::json::string_view key, interface_id const& ifc_id, object_id const& obj_id);

    Environment& RegisterAnonymousInterface(interface_id const& ifc_id, object_id const& obj_id)
    {
        return RegisterInterface(autokey(obj_id, ifc_id), ifc_id, obj_id);
    }


    template <class Interface>
    enable_if_t<is_ifc_handle<Interface>::value, Environment&>
    RegisterInterface(boost::json::string_view key, Interface&& interface, object_id const& obj_id = object_id{ifc_host_nullptr<Interface>})
    {
        InitializeInterfaceHandlers(std::forward<Interface>(interface));
        return RegisterInterface(key, interface_id(std::forward<Interface>(interface)), obj_id);
    }

    template <class Interface>
    enable_if_t<is_ifc_handle<Interface>::value, Environment&>
    RegisterAnonymousInterface(Interface&& interface, object_id const& obj_id = object_id{ifc_host_nullptr<Interface>})
    {
        return RegisterInterface(autokey(obj_id, std::forward<Interface>(interface)), std::forward<Interface>(interface), obj_id);
    }



    object_id ObjectId(boost::json::string_view interface_key) const;

    interface_id InterfaceId(boost::json::string_view interface_key) const;

    boost::json::string GetOrRegisterInterface(object_id const& obj_id, interface_id const& ifc_id);

    object_id DefaultObjectId(interface_id const& ifc_id) const;

};

}  // namespace zmbt

#endif  // ZMBT_FIXTURE_ENVIRONMENT_STORE_HPP_
```


