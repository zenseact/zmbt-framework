/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_TRANSITIONS_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_TRANSITIONS_HPP_

#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/expr/expression.hpp>
#include <zmbt/expr/api.hpp>
#include <zmbt/expr/operator.hpp>
#include <zmbt/model/parameter.hpp>
#include <zmbt/model/traits.hpp>
#include <cstddef>
#include <tuple>
#include <utility>

#include "model_definition_helper.hpp"
#include "zmbt/mapping/model_definition.hpp"
#include "zmbt/mapping/model_definition_node.hpp"



namespace zmbt {
namespace mapping {


template <class Target>
struct ModelDefinition::T_OnTrigger : protected virtual ModelDefinition::BaseTransition
{

    template <class... T>
    Target OnTrigger(boost::json::string_view key, T&&... fmtargs)
    {
        state().set_deferred_param("/trigger", key | expr::Fmt(fmtargs...));
        return transit_to<Target>();
    }

    Target OnTrigger(Param const& key)
    {
        state().params.get_or_create_array("/%s/pointers", key).push_back("/trigger");
        state().model("/trigger") = key;
        return transit_to<Target>();
    }

    Target OnTrigger(Param const& ifc, Param const& obj)
    {
        state().model("/trigger") = {
            {"obj", obj},
            {"ifc", ifc},
        };
        state().params.get_or_create_array("/%s/pointers", obj).push_back("/trigger/obj");
        state().params.get_or_create_array("/%s/pointers", ifc).push_back("/trigger/ifc");
        return transit_to<Target>();
    }


    template <class O, class Interface>
    require_literal<O, Interface, Target>
    OnTrigger(Interface&& ifc, O&& obj) {
        auto key = state().env.RegisterAnonymousTrigger(ifc, obj);
        return OnTrigger(key);
    }

    template <class Interface>
    require_cal<Interface, Target>
    OnTrigger(Interface&& ifc) {
        static_assert(!is_member_pointer<Interface>::value, "");
        return OnTrigger(std::forward<Interface>(ifc), ifc_host_nullptr<Interface>);
    }


    template <class Interface>
    require_cal<Interface, Target>
    OnTrigger(Interface&& ifc, Param const& obj)
    {
        state().model("/trigger") = {
            {"ifc", state().env.RegisterParametricTriggerIfc(ifc)}
        };
        state().params.get_or_create_array("/%s/pointers", obj).push_back("/trigger/obj");
        return transit_to<Target>();
    }

    template <class H>
    require_obj<H, Target>
    OnTrigger(Param const& ifc, H&& obj)
    {
        state().model("/trigger") = {
            {"obj", state().env.RegisterParametricTriggerObj(obj)}
        };
        state().params.get_or_create_array("/%s/pointers", ifc).push_back("/trigger/ifc");
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Repeat : protected virtual ModelDefinition::BaseTransition
{
    /// Run trigger N times
    Target Repeat(std::size_t N)
    {
        state().model("/repeat_trigger") = N;
        return transit_to<Target>();
    }

    /// Run trigger N times
    Target Repeat(Param const& N)
    {
        state().model("/repeat_trigger") = N;
        state().params.get_or_create_array("/%s/pointers", N).push_back("/repeat_trigger");
        return transit_to<Target>();
    }
};



template <class Target>
struct ModelDefinition::T_Filter : protected virtual ModelDefinition::BaseTransition
{

    /// Interface return clause
    /// Refers to the return subsignal at the given JSON Pointer
    Target Return(lang::Expression const& e = "")
    {
        state().set_channel_sp("return", e);
        return transit_to<Target>();
    }

    /// Interface return clause with printf-like format
    template <class T, class... Rest>
    Target Return(boost::json::string_view fmt, T&& arg1, Rest&&... args_rest)
    {
        return Return(fmt | expr::Fmt(arg1, args_rest...));
    }

    /// Interface argument clause
    /// Refers to the arguments subsignal at the given JSON Pointer
    Target Args(lang::Expression const& e = "$(default)")
    {
        state().set_channel_sp("args", e);
        return transit_to<Target>();
    }

    /// Interface argument clause with printf-like format
    template <class T, class... Rest>
    Target Args(boost::json::string_view fmt, T&& arg1, Rest&&... args_rest)
    {
        return Args(fmt | expr::Fmt(arg1, args_rest...));
    }

    /// Interface exception
    Target Exception()
    {
        state().cur_channel()["kind"] = "exception";
        state().cur_channel()["signal_path"] = "";
        return transit_to<Target>();
    }

    /// Output capture timestamp
    Target Timestamp()
    {
        state().cur_channel()["kind"] = "ts";
        state().cur_channel()["signal_path"] = "";
        return transit_to<Target>();
    }

    /// Output capture thread id
    Target ThreadId()
    {
        state().cur_channel()["kind"] = "tid";
        state().cur_channel()["signal_path"] = "";
        return transit_to<Target>();
    }

    Target CallCount()
    {
        state().cur_channel()["kind"] = "call_count";
        state().cur_channel()["signal_path"] = "";
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_As : protected virtual ModelDefinition::BaseTransition
{
    /// Apply Overload operator and optionally pipe with Cast
    /// if expression is non-predicate constant.
    Target As(boost::json::string_view ref)
    {
        state().cur_pipe()["overload"] = ref;
        return transit_to<Target>();
    }

    /// Apply Overload operator and optionally pipe with Cast
    /// if expression is non-predicate constant.
    Target As(lang::Operator const& op)
    {
        return As(op.annotation());
    }
};


template <class Target>
struct ModelDefinition::T_Take : protected virtual ModelDefinition::BaseTransition
{
    /// Set the pre/post transformation on channel signal
    Target Take(lang::Expression const& expr)
    {
        state().set_deferred_param(format("%s/transform", state().head_pointer()), expr);
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Tag : protected virtual ModelDefinition::BaseTransition
{
    /// Set channel tag
    Target Tag(boost::json::string_view alias)
    {
        state().cur_channel()["tag"] = alias;
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_PipeId : protected virtual ModelDefinition::BaseTransition
{
    /// Set pipe identifier for test reports
    Target operator[](lang::Expression const& expr)
    {
        state().set_deferred_param(format("/pipes/%d/id", (state().pipe_count_ - 1)), expr);
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_At : protected virtual ModelDefinition::BaseTransition
{
    /// Create input channel with an interface literal
    template <class Interface>
    require_not_str<Interface, Target>
    At(Interface&& cal, object_id const& obj)
    {
        state().add_channel(obj, std::forward<Interface>(cal));
        return transit_to<Target>();
    }

    template <class Interface>
    require_not_str<Interface, Target>
    At(Interface&& cal, Param const& obj)
    {
        state().add_channel(obj, std::forward<Interface>(cal));
        return transit_to<Target>();
    }


    /// Create input channel
    template <class Interface>
    require_not_str<Interface, Target>
    At(Interface&& cal)
    {
        state().add_channel(std::forward<Interface>(cal));
        return transit_to<Target>();
    }

    template <class... T>
    Target
    At(boost::json::string_view key, T&&... fmtargs)
    {
        state().add_channel(key | expr::Fmt(fmtargs...));
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_ContinuePipe : protected virtual ModelDefinition::BaseTransition
{
    /// Combine channel outputs in time series
    ///
    /// Combined channels produce a list of pairs [channel alias, requested signal],
    /// sorted by timestamp. Use it in combination with Saturate expr for testing strict or partial
    /// order on mock calls.
    Target Blend()
    {
        state().continue_pipe("blend");
        return transit_to<Target>();
    }

    /// Pack channel outputs into an array similarly to Fork keyword
    /// X with Y with Z -> [X, Y, Z]
    Target Group()
    {
        state().continue_pipe("group");
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_TerminatePipe : protected virtual ModelDefinition::BaseTransition
{
    Target Inject()
    {
        state().head_pipe_type_.emplace_null();
        state().cur_pipe()["column"] = state().test_column_count_++;
        state().cur_pipe()["role"] = "inject";
        return transit_to<Target>();
    }
    /// Set fixed input condition
    Target Inject(lang::Expression const& expr)
    {
        state().head_pipe_type_.emplace_null();
        state().cur_pipe()["role"] = "inject";
        state().set_expr(expr);
        return transit_to<Target>();
    }

    Target Expect()
    {
        state().head_pipe_type_.emplace_null();
        state().cur_pipe()["column"] = state().test_column_count_++;
        state().cur_pipe()["role"] = "expect";
        return transit_to<Target>();
    }
    /// Set fixed output assertion
    Target Expect(lang::Expression const& expr)
    {
        state().head_pipe_type_.emplace_null();
        state().cur_pipe()["role"] = "expect";
        state().set_expr(expr);
        return transit_to<Target>();
    }

    // TODO: add Assert* clauses
};


template <class Target>
struct ModelDefinition::T_Test : protected virtual ModelDefinition::BaseTransition
{
    Target Test()
    {
        return transit_to<Target>()();
    }
    Target Test(lang::Expression const& e0)
    {
        return transit_to<Target>()(e0);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1)
    {
        return transit_to<Target>()(e0, e1);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2)
    {
        return transit_to<Target>()(e0, e1, e2);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3)
    {
        return transit_to<Target>()(e0, e1, e2, e3);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                    lang::Expression const& e4)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                    lang::Expression const& e4, lang::Expression const& e5)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                    lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6);
    }
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                    lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6, lang::Expression const& e7)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6, e7);
    }

    template <class... Rest>
    Target Test(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                    lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6, lang::Expression const& e7,
                    Rest&&... rest)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6, e7, lang::Expression(rest)...);
    }
};


template <class Target>
struct ModelDefinition::T_TestRow : protected virtual ModelDefinition::BaseTransition
{
    /// test comment
    Target operator[](boost::json::string_view comment)
    {
        state().set_comment(comment);
    //     auto const idx = model.at("/tests").as_array().size() -1;
    // model("/comments/%d", idx) = comment;
    //     state().set_deferred_param(format("/pipes/%d/id", (pipe_count_ - 1)), expr);

        return transit_to<Target>();
    }

    Target operator()()
    {
        state().add_test_case({});
        return transit_to<Target>();
    }

    Target operator()(lang::Expression const& e0)
    {
        state().add_test_case({e0});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1)
    {
        state().add_test_case({e0, e1});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2)
    {
        state().add_test_case({e0, e1, e2});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3)
    {
        state().add_test_case({e0, e1, e2, e3});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                          lang::Expression const& e4)
    {
        state().add_test_case({e0, e1, e2, e3, e4});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                          lang::Expression const& e4, lang::Expression const& e5)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                          lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6});
        return transit_to<Target>();
    }
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                          lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6, lang::Expression const& e7)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6, e7});
        return transit_to<Target>();
    }

    template <class... Rest>
    Target operator()(lang::Expression const& e0, lang::Expression const& e1, lang::Expression const& e2, lang::Expression const& e3,
                          lang::Expression const& e4, lang::Expression const& e5, lang::Expression const& e6, lang::Expression const& e7,
                          Rest&&... rest)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6, e7, lang::Expression(rest)...});
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Description : protected virtual ModelDefinition::BaseTransition
{
    template <class... T>
    Target Description(boost::json::string_view comment, T&&... args)
    {
        state().set_deferred_param("/description", comment | expr::Fmt(args...));
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Param : protected virtual ModelDefinition::BaseTransition
{

    Target Zip(Param const& p, boost::json::value const& v0)
    {
        state().init_zip();
        return transit_to<Target>()(p, v0);
    }
    Target Zip(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1);
    }

    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7);
    }

    template <class... A>
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7,
        A&&... args
    )
    {
        state().init_zip();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
    }
    template <class... A>
    Target Zip(Param const& p, A&&... args)
    {
        state().init_zip();
        return transit_to<Target>()(p, std::forward<A>(args)...);
    }

    Target Prod(Param const& p, boost::json::value const& v0)
    {
        state().init_prod();
        return transit_to<Target>()(p, v0);
    }
    Target Prod(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1);
    }

    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7);
    }

    template <class... A>
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7,
        A&&... args
    )
    {
        state().init_prod();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
    }
    template <class... A>
    Target Prod(Param const& p, A&&... args)
    {
        state().init_prod();
        return transit_to<Target>()(p, std::forward<A>(args)...);
    }

    Target Pairwise(Param const& p, boost::json::value const& v0)
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0);
    }
    Target Pairwise(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1);
    }

    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2);
    }
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3);
    }
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4);
    }
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5);
    }
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6);
    }
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7);
    }

    template <class... A>
    Target Pairwise(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7,
        A&&... args
    )
    {
        state().init_pairwise();
        return transit_to<Target>()(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
    }
    template <class... A>
    Target Pairwise(Param const& p, A&&... args)
    {
        state().init_pairwise();
        return transit_to<Target>()(p, std::forward<A>(args)...);
    }
};


template <class Target>
struct ModelDefinition::T_CustomParam : protected virtual ModelDefinition::BaseTransition
{
    Target Parametrize(lang::Expression const&)
    {
        state().init_parametrize();
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_ParamRow : protected virtual ModelDefinition::BaseTransition
{
    template <class... A>
    Target operator()(Param const& p, A&&... args)
    {
        state().add_param_values_with_transform(p, args...);
        return transit_to<Target>();
    }

    Target operator()(Param const& p, boost::json::value const& v0)
    {
        state().add_param_values(p, boost::json::array{v0});
        return transit_to<Target>();
    }
    Target operator()(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        state().add_param_values(p, {v0, v1});
        return transit_to<Target>();
    }

    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        state().add_param_values(p, {v0, v1, v2});
        return transit_to<Target>();
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        state().add_param_values(p, {v0, v1, v2, v3});
        return transit_to<Target>();
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        state().add_param_values(p, {v0, v1, v2, v3, v4});
        return transit_to<Target>();
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5
    )
    {
        state().add_param_values(p, {v0, v1, v2, v3, v4, v5});
        return transit_to<Target>();
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6
    )
    {
        state().add_param_values(p, {v0, v1, v2, v3, v4, v5, v6});
        return transit_to<Target>();
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7
    )
    {
        state().add_param_values(p, {v0, v1, v2, v3, v4, v5, v6, v7});
        return transit_to<Target>();
    }
    template <class... A>
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4,
        boost::json::value const& v5,
        boost::json::value const& v6,
        boost::json::value const& v7,
        A&&... args
    )
    {
        state().add_param_values_with_transform(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_PreRun : protected virtual ModelDefinition::BaseTransition
{
    template <class... T>
    Target PreRun(T&&... tasks)
    {
        boost::mp11::tuple_for_each(std::forward_as_tuple(tasks...), [&](auto&& task) { state().add_task(task, true); });
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_PostRun : protected virtual ModelDefinition::BaseTransition
{
    template <class... T>
    Target PostRun(T&&... tasks)
    {
        boost::mp11::tuple_for_each(std::forward_as_tuple(tasks...), [&](auto&& task) { state().add_task(task, false); });
        return transit_to<Target>();
    }
};

}  // namespace mapping
}  // namespace zmbt


#endif
