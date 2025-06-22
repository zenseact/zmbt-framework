/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_HELPER_HPP_
#define ZMBT_MAPPING_DEFINITION_HELPER_HPP_


#include <stdint.h>
#include <boost/json.hpp>
#include <zmbt/core/interface_id.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/core/object_id.hpp>
#include <zmbt/core/parameter.hpp>
#include <zmbt/model/environment.hpp>
#include <zmbt/expr/expression.hpp>
#include <zmbt/model/traits.hpp>
#include <zmbt/model/param_transform.hpp>
#include <functional>
#include <utility>
#include <vector>

namespace zmbt {
namespace mapping {

namespace detail {
/// SignalMapping model definition helper
class DefinitionHelper {

  public:

    Environment env;
    JsonNode model;
    JsonNode params;


    void execute();

    std::size_t cur_cnl_idx() const;
    boost::json::string head_channel() const;
    boost::json::array const& pointers_for(Param const& p);

    void set_deferred_param(boost::json::string_view node_ptr, boost::json::value const& param);
    void set_description(boost::json::string_view comment);
    void set_comment(boost::json::string_view comment);
    void add_task(boost::json::string_view ref, bool const pre);
    void add_task(std::function<void()> fn, bool const pre);
    void add_param_values(Param const& p, boost::json::array const& values);


    template <class... A>
    void add_param_values_with_transform(Param const& p, A&&... args)
    {
        auto const& pointers = pointers_for(p);
        boost::json::array values {param_transform(p, pointers, std::forward<A>(args))...};
        add_param_values(p, values);
    }

    void init_zip();

    void init_prod();

    void init_pairwise();
    void init_parametrize();


    enum cnl_param : uint32_t {
        cnl_prm_none = 0,
        cnl_prm_cal = 1U << 0,
        cnl_prm_obj = 1U << 1,
        cnl_prm_key = 1U << 2,
        cnl_prm_defer_key = 1U << 3
    };

    boost::json::value handle_obj_p(object_id const& obj, uint32_t&)
    {
        return {obj};
    }

    boost::json::value handle_obj_p(Param const& obj, uint32_t& param_type)
    {
        param_type |= cnl_prm_obj;
        return obj;
    }

    boost::json::value handle_obj_p(boost::json::string_view obj, uint32_t&)
    {
        return {obj};
    }

    template <class P>
    require_cal<P, boost::json::value>
    handle_cal_p(P&& cal, uint32_t&)
    {
        env.RegisterPrototypes(cal);
        return {interface_id(cal)};
    }

    boost::json::value handle_cal_p(Param const& cal, uint32_t& param_type)
    {
        param_type |= cnl_prm_cal;
        return cal;
    }


    boost::json::value handle_key_p(boost::json::value const node, uint32_t& param_type)
    {
        if (Param::isParam(node))
        {
            param_type |= cnl_prm_key;
        }
        else if (!lang::Expression(node).is_literal())
        {
            param_type |= cnl_prm_defer_key;
        }
        return {node};
    }

    void combine_channels(boost::json::string_view combo);

    void add_channel_impl(boost::json::value const& ifc, uint32_t const param_type);

    template <class O, class C>
    void add_channel(O&& obj, C&& cal)
    {
        uint32_t param_type = cnl_prm_none;
        boost::json::value obj_node = handle_obj_p(obj, param_type);
        boost::json::value cal_node = handle_cal_p(cal, param_type);

        add_channel_impl({
            {"obj", obj_node},
            {"ifc", cal_node},
        }, param_type);

        if (cnl_prm_none == param_type) {
            env.RegisterAnonymousInterface(interface_id(cal_node), object_id(obj_node));
        }
    }

    template <class T>
    require_cal<T>
    add_channel(T&& cal)
    {
        uint32_t param_type = cnl_prm_none;
        boost::json::value cal_node = handle_cal_p(cal, param_type);

        add_channel_impl({
            {"obj", "$default"},
            {"ifc", cal_node},
        }, param_type);

        if (cnl_prm_none == param_type) {
            env.RegisterAnonymousInterface(interface_id(cal_node), ifc_host_nullptr<T>);
        }
    }

    template <class T>
    require_not_cal<T>
    add_channel(T&& key)
    {
        uint32_t param_type {cnl_prm_none};
        boost::json::value key_node = handle_key_p(std::forward<T>(key), param_type);
        add_channel_impl(key_node, param_type);
    }

    void set_channel_sp(boost::json::string_view kind, boost::json::value const& sp);

    void add_test_case(std::vector<lang::Expression> const& tv);

    void set_expr(lang::Expression const& expr);

    DefinitionHelper();

    DefinitionHelper(DefinitionHelper const&) = default;
    DefinitionHelper(DefinitionHelper &&) = default;
    DefinitionHelper(DefinitionHelper &) = default;
    DefinitionHelper& operator=(DefinitionHelper const&) = default;
    DefinitionHelper& operator=(DefinitionHelper &&) = default;
    ~DefinitionHelper();


};  // DefinitionHelper

}  // namespace detail
}  // namespace mapping
}  // namespace zmbt

#endif
