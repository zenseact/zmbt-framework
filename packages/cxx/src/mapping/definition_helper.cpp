/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0

 */
#include "zmbt/mapping/definition_helper.hpp"

#include <zmbt/core/aliases.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/model/expression_keyword.hpp>
#include <zmbt/reflect/serialization.hpp>
#include <cstddef>

#include "zmbt/mapping/test_runner.hpp"


namespace zmbt {
namespace mapping {

DefinitionHelper::DefinitionHelper()
    : env{}
    , model{{
        {"name"        , nullptr     },
        {"description" , ""          },
        {"channels"    , boost::json::array() },
        {"tests"       , boost::json::array() },
        {"param_groups", boost::json::array() },
        {"comments"    , boost::json::object()},
    }}
{
    params = model.branch(boost::json::kind::object, "/params");
}

DefinitionHelper::~DefinitionHelper()
{
}

std::size_t DefinitionHelper::cur_cnl_idx() const
{
    return model.at("/channels").as_array().size() - 1;
}


boost::json::string DefinitionHelper::head_channel() const
{
    return format("/channels/%d", cur_cnl_idx()).c_str();
}

void DefinitionHelper::set_deferred_param(boost::json::string_view node_ptr, boost::json::value const& param)
{
    if (param == "$default")
    {
        model(node_ptr) = param;
    }
    else
    {
        DeferredFormat df{param};
        model(node_ptr) = df;
        for (auto const& kv : df.deferred_params()) {
            auto const& param_key  = kv.key();
            auto const& df_pointer =  kv.value();
            params("/%s/pointers/+", param_key) = format("%s%s", node_ptr, df_pointer);
        }
    }
}

void DefinitionHelper::add_task(boost::json::string_view ref, bool const pre)
{
    boost::json::string_view node = pre ? "/pre-run/+" : "/post-run/+";
    model(node) = ref;
}

void DefinitionHelper::add_task(std::function<void()> fn, bool const pre)
{
    boost::json::string_view pref = pre ? "pre" : "post";
    boost::json::string ref{
        format("%s:%s#%s", object_id(this).key(), pref, model.get_or_create_array("/%s-run", pref).size())};
    env.RegisterAction(fn, ref);
    add_task(ref, pre);
}


void DefinitionHelper::execute()
{
    TestRunner(model).Run();
}

void DefinitionHelper::set_description(boost::json::string_view comment)
{
    model("/description") = comment;
}


void DefinitionHelper::set_comment(boost::json::string_view comment)
{
    auto const idx = model.at("/tests").as_array().size() -1;
    model("/comments/%d", idx) = comment;
}

boost::json::array const& DefinitionHelper::pointers_for(Param const& p)
{
    return params.at("/%s/pointers", p).as_array();
}

void DefinitionHelper::add_param_values(Param const& p, boost::json::array const& values)
{
    size_t const param_group = model("/param_groups").as_array().size() - 1;
    boost::json::array& value_group = params.get_or_create_array("/%s/value_groups/%d", p, param_group);
    for (auto const& val : values) {
        value_group.push_back(val);
    }
}

void DefinitionHelper::init_zip()
{
    model.at("/param_groups").as_array().push_back("Zip");
}

void DefinitionHelper::init_prod()
{
    model.at("/param_groups").as_array().push_back("Prod");
}

void DefinitionHelper::add_channel_impl(boost::json::value const& ifc, boost::json::string_view role, uint32_t const param_type)
{
    auto const idx = cur_cnl_idx() + 1;
    model("/channels/+") = {
        {"interface", ifc},
        {"role", role},
        {"signal_path", "$default"},
        {"kind", "$default"},
        {"call", -1},
        {"alias", idx},
    };

    if (param_type & cnl_prm_key & cnl_prm_obj & cnl_prm_cal)
    {
        // THROW ambiguous
    }

    if (param_type & cnl_prm_defer_key)
    {
        set_deferred_param(format("%s/interface", head_channel()), ifc);
    }

    if (param_type & cnl_prm_key)
    {
        params("/%s/pointers/+", ifc) = format("%s/interface", head_channel());
    }

    if (param_type & cnl_prm_obj)
    {
        auto const& p = ifc.at_pointer("/obj");
        params("/%s/pointers/+", p) = format("%s/interface/obj", head_channel());
    }

    if (param_type & cnl_prm_cal)
    {
        auto const& p = ifc.at_pointer("/ifc");
        params("/%s/pointers/+", p) = format("%s/interface/ifc", head_channel());
    }
}


void DefinitionHelper::set_channel_sp(boost::json::string_view kind, boost::json::value const& sp)
{
    auto cnl_ptr = head_channel();
    model("%s/kind", cnl_ptr) = kind;
    set_deferred_param(format("%s/signal_path", cnl_ptr), sp);
}

void DefinitionHelper::add_test_case(std::vector<Expression> const& tv)
{
    // TODO: implement expression parametrization
    // This is a draft guess-based logic, the correct implementation
    // would require Keyword::Param + traversing the expr value to get nested params
    auto const N = model.at("/tests").as_array().size();
    for (size_t i = 0; i < tv.size(); i++)
    {
        auto const& expr = tv.at(i);
        if (expr.is(Expression::Keyword::Eq) && Param::isParam(expr.subexpr()))
        {
            params("/%s/pointers/+", expr.subexpr()) = format(
            "/tests/%d/%d", N, i);
        }
    }
    model("/tests/+") = json_from(tv);
}

}  // namespace mapping
}  // namespace zmbt
