/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_TRIGGER_HPP_
#define ZMBT_MODEL_TRIGGER_HPP_

#include <boost/type_index.hpp>

#include "zmbt/model/exceptions.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"

namespace zmbt {

namespace detail {

template <class T>
inline auto static_ptr_cast(std::shared_ptr<void> obj)
{
    return std::static_pointer_cast<T>(obj);
};

template <>
inline auto static_ptr_cast<nullptr_t>(std::shared_ptr<void> obj)
{
    return nullptr;
};

}



/// Object handler to be used with TriggerIfc
class TriggerObj
{
    object_id id_;
    bool is_unsafe_ptr_;
    std::shared_ptr<void> ptr_;

    static void stub_delete(void*) {}

public:

    template <class T>
    TriggerObj(std::shared_ptr<T> obj)
        : id_{(remove_cvref_t<T>*)(obj.get())}
        , is_unsafe_ptr_{false}
        , ptr_{obj}
    {
    }

    template <class T>
    TriggerObj(T* obj)
        : id_{obj}
        , is_unsafe_ptr_{true}
        , ptr_{std::shared_ptr<void>(obj, stub_delete)}
    {
    }

    template <class T>
    TriggerObj(T const* obj) : TriggerObj(const_cast<T*>(obj))
    {
    }

    template <class T>
    TriggerObj(T& obj) : TriggerObj(&obj)
    {
    }

    template <class T>
    TriggerObj(T const& obj) : TriggerObj(&obj)
    {
    }


    TriggerObj(nullptr_t)
        : id_{nullptr}
        , is_unsafe_ptr_{false}
        , ptr_{nullptr}
    {
    }

    virtual ~TriggerObj() = default;

    TriggerObj(TriggerObj const&) = default;
    TriggerObj(TriggerObj &&) = default;
    TriggerObj& operator=(TriggerObj const&) = default;
    TriggerObj& operator=(TriggerObj &&) = default;

    object_id id() const
    {
        return id_;
    }

    bool unsafe() const
    {
        return is_unsafe_ptr_;
    }

    std::shared_ptr<void> ptr() const
    {
        return ptr_;
    }
};



/// Function wrapper. Transforms `(T...) -> R` or `(O*)(T...) -> R` to `(shared_ptr<void>, JSON) -> JSON`
class TriggerIfc
{
    interface_id id_;
    std::function<boost::json::value(std::shared_ptr<void>, boost::json::value const&)> fn_;

    template <class R>
    static auto apply_fn(std::function<R()> fn) -> mp_if<is_void<R>, boost::json::value>
    {
        fn();
        return nullptr;
    }

    template <class R>
    static auto apply_fn(std::function<R()> fn) -> mp_if<mp_not<is_void<R>>, boost::json::value>
    {
        auto ret = fn();
        return json_from(ret);
    }

public:

    template <class I>
    TriggerIfc(I&& interface)
        : id_{std::forward<I>(interface)}
        , fn_{[ifc_ptr = get_ifc_pointer(std::forward<I>(interface))]
            (std::shared_ptr<void> obj, boost::json::value const& args_in) -> boost::json::value {
            using reflection = reflect::invocation<I const&>;
            using return_t = typename reflection::return_t;
            using args_t = typename reflection::args_t;
            using args_unqf_t = tuple_unqf_t<args_t>;
            using ifc_host_unref_t = remove_reference_t<typename reflection::host_t>;

            boost::json::value args_json_in_out;
            std::function<return_t()> fn;
            std::function<boost::json::value()> get_in_out_args = []{ return nullptr; };

            boost::json::value capture_args;
            boost::json::value capture_return;
            boost::json::value capture_ts;
            boost::json::value capture_tid;
            boost::json::value capture_exception;

            try
            {
                args_unqf_t args_typed_unqf_in = args_in.is_array()
                    ? dejsonize<args_unqf_t>(args_in)
                    : dejsonize<args_unqf_t>(boost::json::array{args_in});

                args_t args_typed_in_out = convert_tuple_to<args_t>(args_typed_unqf_in);

                fn = [obj, ifc_ptr, args_typed_in_out]() -> return_t {
                    if (is_member_function_pointer<I>::value && !obj) {
                        throw_exception(environment_error("invoking mfp trigger with null object"));
                    }
                    // WARN: is_unsafe_ptr cast
                    // TODO: check type_index by comp option
                    return reflection::apply(detail::static_ptr_cast<ifc_host_unref_t>(obj), ifc_ptr, args_typed_in_out);
                };

                capture_ts = get_ts();
                capture_tid = get_tid();
                try
                {
                    capture_return = apply_fn<return_t>(fn);
                }
                catch(const std::exception& e)
                {
                    auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
                    capture_exception = {
                        {"type"   , dynamic_exception_type   },
                        {"what"   , e.what()                 },
                        {"context", "trigger execution"}
                    };
                }
                catch(...)
                {
                    // TODO: try to gget type info from std::exception_ptr
                    capture_exception = {
                        {"type"   , "unknown"          },
                        {"what"   , "unknown"          },
                        {"context", "trigger execution"}
                    };
                }
                capture_args = json_from(convert_tuple_to<args_unqf_t>(args_typed_in_out));
            }
            catch(const std::exception& e)
            {
                auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
                capture_exception = {
                    {"type"   , dynamic_exception_type   },
                    {"what"   , e.what()                 },
                    {"context", "trigger args evaluation"}
                };
            }

            return {
                {"args"     , capture_args     },
                {"return"   , capture_return   },
                {"ts"       , capture_ts       },
                {"tid"      , capture_tid      },
                {"exception", capture_exception},
            };
        }}
    {
    }

    virtual ~TriggerIfc() = default;

    TriggerIfc(TriggerIfc const&) = default;
    TriggerIfc(TriggerIfc &) = default;
    TriggerIfc(TriggerIfc &&) = default;
    TriggerIfc& operator=(TriggerIfc const&) = default;
    TriggerIfc& operator=(TriggerIfc &&) = default;

    interface_id id() const
    {
        return id_;
    }

    boost::json::value operator()(std::shared_ptr<void> obj, boost::json::value const& args) const
    {
        return fn_(obj, args);
    }
};

/// Controlled environment interface executor.
class Trigger final {
    struct internal_ctor {};
    TriggerObj obj_;
    TriggerIfc ifc_;

    Trigger(internal_ctor, TriggerObj const& obj, TriggerIfc const& ifc) : obj_{obj}, ifc_{ifc}
    {
    }


  public:

    ~Trigger() = default;
    Trigger(Trigger const&) = default;
    Trigger(Trigger &&) = default;
    Trigger& operator=(Trigger const&) = default;
    Trigger& operator=(Trigger &&) = default;

    template <class T, class I>
    Trigger(T&& obj, I&& interface)
        : Trigger(internal_ctor{}, TriggerObj(std::forward<T>(obj)), TriggerIfc(std::forward<I>(interface)))
    {
    }

    template <class T, class I, class... A>
    Trigger(type_tag<T>, I&& interface, A&&... args)
        : Trigger(std::make_shared<T>(std::forward<A>(args)...), std::forward<I>(interface))
    {
    }


    boost::json::value execute(boost::json::value const& args_in = boost::json::array{}) const
    {
        return ifc_(obj_.ptr(), args_in);
    }

    boost::json::value operator()(boost::json::value const& args_in = boost::json::array{}) const
    {
        return execute(args_in);
    }

    bool operator==(Trigger const& o) const
    {
        return (obj_.id() == o.obj_.id()) && (ifc_.id() == o.ifc_.id());
    }

    bool operator!=(Trigger const& o) const
    {
        return !this->operator==(o);
    }

    object_id obj_id() const
    {
        return obj_.id();
    }

    interface_id ifc_id() const
    {
        return ifc_.id();
    }

};


}  // namespace zmbt

#endif