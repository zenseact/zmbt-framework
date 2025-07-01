/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

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
#include <zmbt/model/test_failure.hpp>
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

namespace zmbt {


/**
 * @brief Controlled environment data storage
 *
 * @details Handles signal values and calls counters to/from environment, using interface pointers as keys
 * @see <A HREF="/user-guide/environment/">Environment API</A>
 *
 */
class Environment {

  protected:

    using lock_t = typename EnvironmentData::lock_t;

    template <class T>
    using return_t = reflect::invocation_ret_t<ifc_pointer_t<T>>;

    template <class T>
    using unqf_args_t = reflect::invocation_args_unqf_t<ifc_pointer_t<T>>;


    template <class T>
    using argsref_t = reflect::invocation_args_t<ifc_pointer_t<T>>;

    template <class T>
    using rvref_to_val = mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;

    template <class I>
    using hookout_args_t = mp_transform<rvref_to_val, argsref_t<I>>;

    std::shared_ptr<EnvironmentData> data_;

    // TODO: move to persistent config
    using FailureHandler = std::function<void(boost::json::value const&)>;
    struct PersistentConfig
    {
        FailureHandler failure_handler {&zmbt::default_test_failure};
        bool pretty_print {false};
    };

    std::shared_ptr<PersistentConfig> config_;


  public:
    class InterfaceHandle;

    template <class Interface>
    class TypedInterfaceHandle;


    JsonNode& json_data()
    {
        return data_->json_data;
    }

    JsonNode const& json_data() const
    {
        return data_->json_data;
    }

    /// Get the Environment thread lock
    lock_t Lock() const;

    /// Get the Environment thread lock with std::try_to_lock tag
    lock_t TryLock() const;

    /// Get the Environment thread lock with std::defer_lock tag
    lock_t DeferLock() const;

    Environment();

    Environment(Environment &&) = default;

    Environment(Environment const&) = default;

    Environment& operator=(Environment &&) = default;

    Environment& operator=(Environment const&) = default;


    virtual ~Environment()
    {
    }

    /// @brief Dump current environment state in prettified JSON
    /// @param os output stream
    void DumpJsonData(std::ostream &os);


    /**
     * @brief Set the environment variable
     *
     * @tparam T
     * @param key_expr key constant expression
     * @param var
     */
    //@{
    void SetVar(lang::Expression const& key_expr, boost::json::value var);

    template <class T>
    void SetVar(lang::Expression const& key_expr, T var)
    {
        return SetVar(key_expr, json_from(var));
    }
    //@}


    /**
     * @brief Get or update the environment variable
     *
     * @tparam T
     * @param obj
     * @param key
     * @param update_value
     * @return T
     */
    //@{
    boost::json::value GetVarOrUpdate(lang::Expression const& key_expr, boost::json::value update_value);

    template <class T>
    T GetVarOrUpdate(lang::Expression const& key_expr, T update_value)
    {
        return dejsonize<T>(GetVarOrUpdate(key_expr, json_from(update_value)));
    }
    //@}


    /**
     * @brief Get the environment variable or use default
     *
     * @tparam T
     * @param obj
     * @param key
     * @param default_value
     * @return T
     */
    //@{
    boost::json::value GetVarOrDefault(lang::Expression const& key_expr, boost::json::value default_value = {});

    template <class T>
    T GetVarOrDefault(lang::Expression const& key_expr, T default_value = reflect::signal_traits<T>::init())
    {
        return dejsonize<T>(GetVarOrDefault(key_expr, json_from(default_value)));
    }
    //@}


    /**
     * @brief Get the environment variable
     *
     * @tparam T
     * @param obj
     * @param key
     * @return T
     */
    //@{
    boost::json::value GetVar(lang::Expression const& key_expr);

    template <class T>
    T GetVar(lang::Expression const& key_expr)
    {
        return dejsonize<T>(GetVar(key_expr));
    }
    //@}


    /**
     * @brief Set the shared data associated with key
     *
     * @tparam T
     * @tparam A
     * @param key_expr
     * @param data
     */
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


    /**
     * @brief Get the shared data associated with object and string key
     *
     * @details Dynamic polymorphism is not supported, as the type safety is ensured
     * by comparing typeid(T). If the type T is not exactly the same as was used on
     * corresponding SetShared call, the method will throw environment_error.
     *
     * If corresponding data was not set, the metod return nullptr.
     *
     * @tparam T
     * @tparam A
     * @param fmtstr key format string
     * @param arg key format arguments
     * @return std::shared_ptr<T>
     */
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
            throw environment_error("GetShared invoked with incompatible type for `%s`", key);
        }

        return std::static_pointer_cast<T>(record.second);
    }

    /// @brief Get reference to shared var, creating it if necessary
    /// @tparam T
    /// @tparam ...A Constructor args for initial value
    /// @param key_expr
    /// @return
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
                throw environment_error("GetSharedRef failed to create shared object at `%s`", key);
            }
            return *shared;
        }

        auto const record = found->second;
        if (std::type_index(typeid(T)) != record.first)
        {
            throw environment_error("GetSharedRef invoked with incompatible type for `%s`", key);
        }

        return *std::static_pointer_cast<T>(record.second);
    }

    /// Check if shared variable exists
    bool ContainsShared(lang::Expression const& key_expr) const;


    /**
     * @brief Clear all interface data (accessed via ArgsFor/ReturnFor)
     *
     * @warning this will invalidate interface records
     *
     */
    void ResetInterfaceData();


    /**
     * @brief Clear interface data associated with the object (accessed via ArgsFor/ReturnFor)
     *
     * @warning this will invalidate interface records
     * @param obj
     */
    void ResetInterfaceDataFor(object_id obj);


    /**
     * @brief Clear all data
     *
     */
    void ResetAll();


    /**
     * @brief Clear data associated with the object
     *
     * @param obj
     */
    void ResetAllFor(object_id obj);


    Environment& RegisterAction(lang::Expression const& key_expr, std::function<void()> action);


    Environment& RunAction(lang::Expression const& key_expr);


    template <class I>
    interface_id RegisterParametricTriggerIfc(I&& interface)
    {
        RegisterPrototypes(interface);
        TriggerIfc trigger_ifc{std::forward<I>(interface)};
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

    boost::json::string GetOrRegisterParametricTrigger(object_id const& obj_id, interface_id const& ifc_id);

    /**
     * @brief Register test trigger to enable FFI in the test model runners.
     *
     * @tparam H
     * @tparam I
     * @param host  callable host object
     * @param interface callable interface handle
     * @param key string key, unique per environment
     * @return
     */
    //@{
    template <class I, class H>
    Environment& RegisterTrigger(boost::json::string_view key, I&& interface, H&& host)
    {
        Trigger trigger{std::forward<H>(host), interface};
        auto const json_data_key = format("/triggers/%s", key);
        auto const err_msg = format("Trigger registering failed: key \"%s\" is taken", key);
        auto lock = Lock();
        auto const trigger_found =  data_->triggers.find(key);
        if (trigger_found != data_->triggers.cend())
        {
            if (trigger_found->second != trigger)
            {
                throw environment_error(err_msg);
            }
            return *this;
        }
        data_->triggers.emplace(key, trigger);
        data_->json_data(json_data_key) = 0; // TODO: timestamp
        RegisterInterface(key, interface, trigger.obj_id());
        return *this;
    }


    template <class I>
    Environment& RegisterTrigger(boost::json::string_view key, I&& interface)
    {
        return RegisterTrigger(key, std::forward<I>(interface), ifc_host_nullptr<I>);
    }
    //@}

    template <class H, class I>
    boost::json::string RegisterAnonymousTrigger(I&& interface, H&& host)
    {
        auto key = autokey(host, interface);
        RegisterTrigger(key, interface, host);
        return key;
    }


    bool HasTrigger(boost::json::string_view key) const;

    bool HasAction(boost::json::string_view key) const;


    template <class I>
    Environment& RegisterPrototypes(I&& interface)
    {
        static_assert(is_ifc_handle<I>::value, "");
        auto const ifc_id = interface_id(interface);
        auto const obj_id = object_id{ifc_host_nullptr<I>};
        auto const proto_key = format("/prototypes/%s", ifc_id);
        auto const defobj_key = format("/refs/defobj/%s", ifc_id);
        auto const prototypes = reflect::prototypes<I>();

        auto lock = Lock();
        if (!data_->json_data.contains(proto_key))
        {
            data_->json_data(proto_key) = prototypes;
            data_->json_data(defobj_key) = obj_id;
        }
        return *this;
    }

    /**
     * @brief Register interface to enable FFI in the test model runners.
     *
     * @param obj_id object associated with callable
     * @param ifc_id callable handle
     * @param key string key, unique per environment
     * @return
     */
    Environment& RegisterInterface(boost::json::string_view key, interface_id const& ifc_id, object_id const& obj_id);

    Environment& RegisterAnonymousInterface(interface_id const& ifc_id, object_id const& obj_id)
    {
        return RegisterInterface(autokey(obj_id, ifc_id), ifc_id, obj_id);
    }


    template <class I>
    enable_if_t<is_ifc_handle<I>::value, Environment&>
    RegisterInterface(boost::json::string_view key, I&& interface, object_id const& obj_id = object_id{ifc_host_nullptr<I>})
    {
        RegisterPrototypes(std::forward<I>(interface));
        return RegisterInterface(key, interface_id{std::forward<I>(interface)}, obj_id);
    }

    template <class I>
    enable_if_t<is_ifc_handle<I>::value, Environment&>
    RegisterAnonymousInterface(I&& interface, object_id const& obj_id = object_id{ifc_host_nullptr<I>})
    {
        RegisterPrototypes(std::forward<I>(interface));
        return RegisterInterface(autokey(obj_id, interface_id{interface}), interface_id(interface), obj_id);
    }


    PersistentConfig Config() const;


    /// Set pretty print JSON values
    Environment& SetPrettyPrint(bool const pretty_print = true);

    /// Set custom test failure handler
    Environment& SetFailureHandler(std::function<void(boost::json::value const&)> const& fn);

    /// Reset the test handler to default
    Environment& ResetFailureHandler();

    Environment& HandleTestFailure(boost::json::value const& diagnostics);

    object_id ObjectId(boost::json::string_view interface_key) const;

    interface_id InterfaceId(boost::json::string_view interface_key) const;

    boost::json::string GetOrRegisterInterface(object_id const& obj_id, interface_id const& ifc_id);

    object_id DefaultObjectId(interface_id const& ifc_id) const;

};

}  // namespace zmbt

#endif  // ZMBT_FIXTURE_ENVIRONMENT_STORE_HPP_
