/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0

 */
#include "zmbt/mapping/model_definition_helper.hpp"

#include <zmbt/core/aliases.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/expr/keyword.hpp>
#include <zmbt/reflect/serialization.hpp>
#include <cstddef>

#include "zmbt/mapping/test_runner.hpp"


namespace zmbt {
namespace mapping {
namespace detail {

DefinitionHelper::DefinitionHelper()
    : env{}
    , model{{
        {"name"        , nullptr     },
        {"description" , ""          },
        {"pipes"       , boost::json::array() },
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

boost::json::object& DefinitionHelper::cur_pipe()
{
    return model("/pipes").as_array().back().as_object();
}

boost::json::object& DefinitionHelper::cur_channel()
{
    return model(channel_pointer_).as_object();
}



void DefinitionHelper::set_deferred_param(boost::json::string_view node_ptr, lang::Expression const& expr)
{
    if (expr == "$(default)")
    {
        model(node_ptr) = "$(default)";
    }
    else
    {
        auto const preproc_params = expr.preprocessing_parameters();
        for (auto const& pp: preproc_params)
        {
            params.get_or_create_array("/%s/pointers", pp.first).push_back(format("%s%s", node_ptr, pp.second).c_str());
        }
        model(node_ptr) = expr.to_json(); // Expression
    }
}

void DefinitionHelper::add_task(boost::json::string_view ref, bool const pre)
{
    boost::json::string_view node = pre ? "/pre-run" : "/post-run";
    model.get_or_create_array(node).push_back(ref);
}

void DefinitionHelper::add_task(std::function<void()> fn, bool const pre)
{
    boost::json::string_view pref = pre ? "pre" : "post";
    boost::json::string ref{
        format("%s:%s#%s", object_id(this).key(), pref, model.get_or_create_array("/%s-run", pref).size())};
    env.RegisterAction(ref, fn);
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
    model("/param_groups").as_array().push_back("Zip");
}

void DefinitionHelper::init_prod()
{
    model("/param_groups").as_array().push_back("Prod");
}

void DefinitionHelper::init_pairwise()
{
    throw_exception(model_error("Pairwise clause not implemented"));
}
void DefinitionHelper::init_parametrize()
{
    throw_exception(model_error("Parametrize clause not implemented"));
}


void DefinitionHelper::continue_pipe(boost::json::string_view combo)
{
    if ((not head_pipe_type_.is_null()) and (head_pipe_type_ != combo)) // already a pipe continuation
    {
        throw_exception(model_error("can't chain different combination clauses, %s vs %s", head_pipe_type_, combo));
    }

    head_pipe_type_ = combo;
    auto& pipe = model("/pipes").as_array().back();
    auto& pipe_obj = pipe.is_null() ? pipe.emplace_object() : pipe.as_object();
    pipe_obj.insert_or_assign("type", combo);
}

void DefinitionHelper::add_channel_impl(boost::json::value const& ifc, uint32_t const param_type)
{

    auto& pipes = model("/pipes").as_array();

    if (head_pipe_type_.is_null())
    {
        channel_rel_count_ = 0;
        pipes.push_back({
            {"channels", boost::json::array{}},
            {"index", pipe_count_++},
        });
    }

    auto& channels = pipes.back().as_object().at("channels").as_array();
    channels.push_back({
        {"interface", ifc},
        {"role", nullptr},
        {"signal_path", "$(default)"},
        {"kind", "$(default)"},
        {"index_abs", channel_abs_count_},
        {"index_rel", channel_rel_count_},
        {"tag", channel_abs_count_},
    });

    channel_abs_count_++;
    channel_rel_count_++;
    channel_pointer_ = format("/pipes/%d/channels/%d", (pipe_count_ - 1), (channel_rel_count_ - 1));
    auto const interface_ptr = format("%s/interface", channel_pointer_);


    if (param_type & cnl_prm_key & cnl_prm_obj & cnl_prm_cal)
    {
        // THROW ambiguous
    }

    if (param_type & cnl_prm_defer_key)
    {
        set_deferred_param(interface_ptr, ifc);
    }

    if (param_type & cnl_prm_key)
    {
       params.get_or_create_array("/%s/pointers", ifc).push_back(interface_ptr.c_str());
    }

    if (param_type & cnl_prm_obj)
    {
        auto const& p = ifc.at_pointer("/obj");
       params.get_or_create_array("/%s/pointers", p).push_back((interface_ptr + "/obj").c_str());
    }

    if (param_type & cnl_prm_cal)
    {
        auto const& p = ifc.at_pointer("/ifc");
       params.get_or_create_array("/%s/pointers", p).push_back((interface_ptr + "/ifc").c_str());
    }
}


void DefinitionHelper::set_channel_sp(boost::json::string_view kind, lang::Expression const& sp)
{
    model("%s/kind", channel_pointer_) = kind;
    set_deferred_param(format("%s/signal_path", channel_pointer_), sp);
}

void DefinitionHelper::add_test_case(std::vector<lang::Expression> const& tv)
{
    auto const N = model.at("/tests").as_array().size();

    for (size_t i = 0; i < tv.size(); i++)
    {
        auto const& expr = tv.at(i);
        auto const preproc_params = expr.preprocessing_parameters();
        for (auto const& pp: preproc_params)
        {
            params.get_or_create_array("/%s/pointers", pp.first).push_back(format(
                    "/tests/%d/%d%s", N, i, pp.second).c_str());
        }
    }
    model.get_or_create_array("/tests").push_back(json_from(tv));
    test_row_count_++;
}

void DefinitionHelper::set_expr(lang::Expression const& expr)
{
    cur_pipe()["expr"] = expr.to_json();
    auto const preproc_params = expr.preprocessing_parameters();
    for (auto const& pp: preproc_params)
    {
        params.get_or_create_array("/%s/pointers", pp.first).push_back(format(
            "/pipes/%d/expr%s", (pipe_count_ - 1), pp.second).c_str());
    }
}

}  // namespace detail
}  // namespace mapping
}  // namespace zmbt
