/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_TRIGGER_HPP_
#define ZMBT_MODEL_TRIGGER_HPP_

#include <boost/type_index.hpp>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "output_capture.hpp"

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



/// Function wrapper. Transforms `(T...) -> R` or `(O*)(T...) -> R` to `(shared_ptr<void>, JSON) -> void`,
/// capturing R with OutputCapture
class TriggerIfc
{
    interface_id id_;
    std::function<void(OutputCapture&)> setup_capture_;
    std::function<void(std::shared_ptr<void>, boost::json::value const&, OutputCapture&)> fn_;

    template <class R>
    static auto apply_fn(std::function<R()> fn) -> mp_if<is_void<R>, nullptr_t>
    {
        fn();
        return nullptr;
    }

    template <class R>
    static auto apply_fn(std::function<R()> fn) -> mp_if<mp_not<is_void<R>>, R>
    {
        return fn();
    }

public:

    template <class I>
    TriggerIfc(I&& interface)
        : id_{std::forward<I>(interface)}
        , setup_capture_{[&interface](OutputCapture& capture){ capture.setup_handlers<I>();}}
        , fn_{[ifc_ptr = get_ifc_pointer(std::forward<I>(interface))]
            (std::shared_ptr<void> obj, boost::json::value const& args_in, OutputCapture& capture) {
            using reflection = reflect::invocation<I const&>;
            using return_t = typename reflection::return_t;
            using args_t = typename reflection::args_t;
            using args_unqf_t = tuple_unqf_t<args_t>;
            using ifc_host_unref_t = remove_reference_t<typename reflection::host_t>;

            using result_t = mp_if<is_void<return_t>, nullptr_t, remove_cvref_t<return_t>>;

            boost::json::value args_json_in_out;
            std::function<return_t()> fn;
            std::function<boost::json::value()> get_in_out_args = []{ return nullptr; };

            args_unqf_t args_to_capture = reflect::signal_traits<args_unqf_t>::init();
            ErrorOr<result_t> return_or_error {};

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

                try
                {
                    return_or_error = ErrorOr<result_t>::MakeValue(apply_fn<return_t>(fn));
                }
                catch(const std::exception& e)
                {
                    auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
                    ErrorInfo err;
                    err.type = dynamic_exception_type.c_str();
                    err.what = e.what();
                    err.context = "trigger execution";
                    return_or_error = ErrorOr<result_t>::MakeError(err);

                }
                catch(...)
                {
                    // TODO: try to get type info from std::exception_ptr

                    ErrorInfo err;
                    err.type = "unknown";
                    err.what = "unknown";
                    err.context = "trigger execution";
                    return_or_error = ErrorOr<result_t>::MakeError(err);
                }

                args_to_capture = convert_tuple_to<args_unqf_t>(args_typed_in_out);
            }
            catch(const std::exception& e)
            {
                auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
                    ErrorInfo err;
                    err.type = dynamic_exception_type.c_str();
                    err.what = e.what();
                    err.context = "trigger args evaluation";
                    return_or_error = ErrorOr<result_t>::MakeError(err);
            }

            capture.push(args_to_capture, return_or_error);
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

    void setup_capture(OutputCapture& capture)
    {
        setup_capture_(capture);
    }

    void execute(std::shared_ptr<void> obj, boost::json::value const& args_in, OutputCapture& capture) const
    {
        return fn_(obj, args_in, capture);
    }
};

/// Controlled environment interface executor.
class Trigger final {
    struct internal_ctor {};
    TriggerObj obj_;
    TriggerIfc ifc_;
    std::shared_ptr<OutputCapture> output_capture_;

    Trigger(internal_ctor, TriggerObj const& obj, TriggerIfc const& ifc, std::shared_ptr<OutputCapture> capture)
        : obj_{obj}
        , ifc_{ifc}
        , output_capture_{capture}
    {
        if(output_capture_)
        {
            ifc_.setup_capture(*output_capture_);
        }
    }


  public:

    ~Trigger() = default;
    Trigger(Trigger const&) = default;
    Trigger(Trigger &&) = default;
    Trigger& operator=(Trigger const&) = default;
    Trigger& operator=(Trigger &&) = default;

    template <class T, class I>
    Trigger(T&& obj, I&& interface, std::shared_ptr<OutputCapture> capture)
        : Trigger(internal_ctor{}, TriggerObj(std::forward<T>(obj)), TriggerIfc(std::forward<I>(interface)), capture)
    {
    }


    void operator()(boost::json::value args_in) const
    {
        ifc_.execute(obj_.ptr(), std::move(args_in), *output_capture_);
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