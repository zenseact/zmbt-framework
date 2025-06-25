/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
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
    return model("/pipes/@").as_object();
}

boost::json::object& DefinitionHelper::cur_channel()
{
    return model(head_pointer_).as_object();
}



void DefinitionHelper::set_deferred_param(boost::json::string_view node_ptr, boost::json::value const& param)
{
    if (param == "$default")
    {
        model(node_ptr) = param;
    }
    else
    {
        model(node_ptr) = param; // Expression
        JsonTraverse([&](boost::json::value const& v, std::string const jp){
            if (Param::isParam(v)) {
                params("/%s/pointers/+", v) = format("%s%s", node_ptr, jp);
            }
            return false;
        })(param);
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
    model("/param_groups").as_array().push_back("Zip");
}

void DefinitionHelper::init_prod()
{
    model("/param_groups").as_array().push_back("Prod");
}

void DefinitionHelper::init_pairwise()
{
    throw model_error("Pairwise clause not implemented");
}
void DefinitionHelper::init_parametrize()
{
    throw model_error("Parametrize clause not implemented");
}


void DefinitionHelper::continue_pipe(boost::json::string_view combo)
{
    if ((not head_pipe_type_.is_null()) and (head_pipe_type_ != combo)) // already a pipe continuation
    {
        throw model_error("can't chain different combination clauses, %s vs %s", head_pipe_type_, combo);
    }

    model("/pipes/@/type") = combo;
    head_pipe_type_ = combo;
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
        {"signal_path", "$default"},
        {"kind", "$default"},
        {"index_abs", channel_abs_count_},
        {"index_rel", channel_rel_count_},
        {"alias", channel_abs_count_},
    });

    channel_abs_count_++;
    channel_rel_count_++;
    head_pointer_ = format("/pipes/%d/channels/%d", (pipe_count_ - 1), (channel_rel_count_ - 1));
    auto const interface_ptr = format("%s/interface", head_pointer_);


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
        params("/%s/pointers/+", ifc) = interface_ptr;
    }

    if (param_type & cnl_prm_obj)
    {
        auto const& p = ifc.at_pointer("/obj");
        params("/%s/pointers/+", p) = interface_ptr + "/obj";
    }

    if (param_type & cnl_prm_cal)
    {
        auto const& p = ifc.at_pointer("/ifc");
        params("/%s/pointers/+", p) = interface_ptr + "/ifc";
    }
}


void DefinitionHelper::set_channel_sp(boost::json::string_view kind, boost::json::value const& sp)
{
    model("%s/kind", head_pointer_) = kind;
    set_deferred_param(format("%s/signal_path", head_pointer_), sp);
}

void DefinitionHelper::add_test_case(std::vector<lang::Expression> const& tv)
{
    auto const N = model.at("/tests").as_array().size();

    for (size_t i = 0; i < tv.size(); i++)
    {
        auto const& expr = tv.at(i);
        JsonTraverse([&](boost::json::value const& v, std::string const jp){
            if (Param::isParam(v)) {
                params("/%s/pointers/+", v) = format(
                            "/tests/%d/%d%s", N, i, jp);
            }
            return false;
        })(expr.underlying());
    }
    model("/tests/+") = json_from(tv);
}

void DefinitionHelper::set_expr(lang::Expression const& expr)
{
    cur_pipe()["expr"] = expr;

    JsonTraverse([&](boost::json::value const& v, std::string const jp){
        if (Param::isParam(v)) {
            params("/%s/pointers/+", v) = format(
                        "/pipes/%d/expr%s", (pipe_count_ - 1), jp);
        }
        return false;
    })(expr.underlying());
}

}  // namespace detail
}  // namespace mapping
}  // namespace zmbt
