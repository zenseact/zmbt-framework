

# File param\_transform.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**param\_transform.hpp**](param__transform_8hpp.md)

[Go to the documentation of this file](param__transform_8hpp.md)


```C++

#ifndef ZMBT_MODEL_PARAM_TRANSFORM_HPP_
#define ZMBT_MODEL_PARAM_TRANSFORM_HPP_


#include <boost/json.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/generic_traits.hpp>
#include <zmbt/core/interface_id.hpp>
#include <zmbt/core/interface_traits.hpp>
#include <zmbt/core/object_id.hpp>
#include <zmbt/model/parameter.hpp>
#include <zmbt/model/exceptions.hpp>
#include <zmbt/reflect/serialization.hpp>
#include <stdint.h>
#include <utility>

#include "environment.hpp"
#include "traits.hpp"


namespace zmbt {


namespace detail {

template <class T, class = void>
struct param_transform;


struct param_transform_base
{
    Param const& param;
    boost::json::array const& pointers;

    struct Kind {
        constexpr static uint32_t none      = 0;
        constexpr static uint32_t trig_node = 0x1 << 0U;
        constexpr static uint32_t trig_obj  = 0x1 << 1U;
        constexpr static uint32_t trig_ifc  = 0x1 << 2U;
        constexpr static uint32_t ifc_node  = 0x1 << 3U;
        constexpr static uint32_t ifc_obj   = 0x1 << 4U;
        constexpr static uint32_t ifc_ifc   = 0x1 << 5U;
    };

    uint32_t pkind {};

    param_transform_base(Param const& p, boost::json::array const& ptrs);
};




template <class T>
struct param_transform<T, require_json_from<T>>
{
    param_transform(Param const&, boost::json::array const&) {}

    boost::json::value operator()(T&& arg)
    {
        return zmbt::json_from(arg);
    }
};



template <class T, class = void>
struct param_transform_interface;

template <class T>
struct param_transform_interface<T, require_cal<T>> : public param_transform_base
{
    using param_transform_base::param_transform_base;

    boost::json::value operator()(T&& arg)
    {
        Environment env {};
        env.InitializeInterfaceHandlers(std::forward<T>(arg));

        boost::json::value pvalue {};
        if ((Kind::trig_ifc & pkind) && (Kind::ifc_node & pkind) )
        {
            throw_exception(model_error("Ambiguous param %s (missing object on channel nodes?)", param));
        }
        if (Kind::trig_node & pkind)
        {
            pvalue = env.RegisterAnonymousTrigger(arg, ifc_host_nullptr<T>);
        }
        else if (Kind::trig_ifc & pkind)
        {
            pvalue = env.RegisterParametricTriggerIfc(arg);
        }
        else if (Kind::ifc_node & pkind)
        {
            pvalue = {
                {"obj", "$(default)"},
                {"ifc", interface_id{arg} },
            };
        }
        else if (Kind::ifc_ifc & pkind)
        {
            pvalue = interface_id{arg};
        }
        else if (Kind::trig_obj & pkind)
        {
            pvalue = register_trig_obj(arg);
        }
        else if (Kind::ifc_obj & pkind) // it could be am mfp on an object with call operator
        {
            pvalue = construct_or_default<object_id, T>(std::forward<T>(arg));
        }
        else
        {
            throw_exception(model_error("Ambiguous param %s (should it be a part of an interface?) `%s`", param, pointers));
        }
        return pvalue;
    }

    template <class O>
    enable_if_t<not ifc_is_fn_handle<O>::value, object_id>
    register_trig_obj(O&& obj)
    {
        return Environment().RegisterParametricTriggerObj(obj);
    }

    template <class O>
    enable_if_t<ifc_is_fn_handle<O>::value, object_id>
    register_trig_obj(O&&)
    {
        return object_id{};
    }
};

template <class T>
struct param_transform_interface<T, require_not_cal<T>> : public param_transform_base
{
    using param_transform_base::param_transform_base;

    boost::json::value operator()(T&& arg)
    {

        if (Kind::trig_obj & pkind)
        {
            return boost::json::value{Environment().RegisterParametricTriggerObj(arg)};
        }
        else if (Kind::ifc_obj & pkind)
        {
            return object_id{arg};
        }
        else
        {
            throw_exception(model_error("Ambiguous param %s (should it be a trigger or channel object?)", param));
        }
        return nullptr;
    }
};

template <class T>
struct param_transform<T, require_no_json_from<T>> : public param_transform_interface<T>
{
    using param_transform_interface<T>::param_transform_interface;
};

} // namespace detail


template <class T>
boost::json::value param_transform(Param const& param, boost::json::array const& pointers, T&& value)
{
    return detail::param_transform<T>(param, pointers)(std::forward<T>(value));
}

}


#endif
```


