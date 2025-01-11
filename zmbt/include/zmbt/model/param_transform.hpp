/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_PARAM_TRANSFORM_HPP_
#define ZMBT_MODEL_PARAM_TRANSFORM_HPP_


#include <boost/json.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/generic_traits.hpp>
#include <zmbt/core/interface_id.hpp>
#include <zmbt/core/interface_traits.hpp>
#include <zmbt/core/object_id.hpp>
#include <zmbt/core/parameter.hpp>
#include <zmbt/model/exceptions.hpp>
#include <zmbt/reflect/serialization_policy.hpp>
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
struct param_transform<T, traits::require_json_from<T>>
{
    param_transform(Param const&, boost::json::array const&) {}

    boost::json::value operator()(T&& arg)
    {
        return zmbt::reflect::json_from(arg);
    }
};



template <class T, class = void>
struct param_transform_interface;

template <class T>
struct param_transform_interface<T, traits::require_cal<T>> : public param_transform_base
{
    using param_transform_base::param_transform_base;

    boost::json::value operator()(T&& arg)
    {
        Environment env {};
        env.RegisterPrototypes(arg);

        boost::json::value pvalue {};
        if ((Kind::trig_ifc & pkind) && (Kind::ifc_node & pkind) )
        {
            throw model_error("Ambiguous param %s (missing object on channel nodes?)", param);
        }
        if (Kind::trig_node & pkind)
        {
            pvalue = env.RegisterTriggerLiteral(ifc_host_nullptr<T>, arg);
        }
        else if (Kind::trig_ifc & pkind)
        {
            pvalue = env.RegisterParametricTriggerIfc(arg);
        }
        else if (Kind::ifc_node & pkind)
        {
            pvalue = {
                {"obj", "$default"},
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
            throw model_error("Ambiguous param %s (should it be a part of an interface?) `%s`", param, pointers);
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
struct param_transform_interface<T, traits::require_not_cal<T>> : public param_transform_base
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
            throw model_error("Ambiguous param %s (should it be a trigger or channel object?)", param);
        }
        return nullptr;
    }
};

template <class T>
struct param_transform<T, traits::require_no_json_from<T>> : public param_transform_interface<T>
{
    using param_transform_interface<T>::param_transform_interface;
};

} // namespace detail


/// Transform Param value of type T to json value and handle type-specific environment effects
template <class T>
boost::json::value param_transform(Param const& param, boost::json::array const& pointers, T&& value)
{
    return detail::param_transform<T>(param, pointers)(std::forward<T>(value));
}

}


#endif
