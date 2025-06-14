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
#include <zmbt/core/parameter.hpp>
#include <zmbt/expr/expression.hpp>
#include <zmbt/expr/expression_api.hpp>
#include <zmbt/expr/signal_operator_handler.hpp>
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
        state().set_deferred_param("/trigger", key | expr::Format(fmtargs...));
        return transit_to<Target>();
    }

    Target OnTrigger(Param const& key)
    {
        state().params("/%s/pointers/+", key) = "/trigger";
        state().model("/trigger") = key;
        return transit_to<Target>();
    }

    Target OnTrigger(Param const& ifc, Param const& obj)
    {
        state().model("/trigger") = {
            {"obj", obj},
            {"ifc", ifc},
        };
        state().params("/%s/pointers/+", obj) = "/trigger/obj";
        state().params("/%s/pointers/+", ifc) = "/trigger/ifc";
        return transit_to<Target>();
    }


    template <class O, class I>
    require_literal<O, I, Target>
    OnTrigger(I&& ifc, O&& obj) {
        auto key = state().env.RegisterAnonymousTrigger(obj, ifc);
        return OnTrigger(key);
    }

    template <class I>
    require_cal<I, Target>
    OnTrigger(I&& ifc) {
        static_assert(!is_member_pointer<I>::value, "");
        return OnTrigger(std::forward<I>(ifc), ifc_host_nullptr<I>);
    }


    template <class I>
    require_cal<I, Target>
    OnTrigger(I&& ifc, Param const& obj)
    {
        state().model("/trigger") = {
            {"ifc", state().env.RegisterParametricTriggerIfc(ifc)}
        };
        state().params("/%s/pointers/+", obj) = "/trigger/obj";
        return transit_to<Target>();
    }

    template <class H>
    require_obj<H, Target>
    OnTrigger(Param const& ifc, H&& obj)
    {
        state().model("/trigger") = {
            {"obj", state().env.RegisterParametricTriggerObj(obj)}
        };
        state().params("/%s/pointers/+", ifc) = "/trigger/ifc";
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
        state().params("/%s/pointers/+", N) = "/repeat_trigger";
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_InjectTo : protected virtual ModelDefinition::BaseTransition
{
    /// Create input channel with an interface literal
    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal, object_id const& obj)
    {
        state().add_channel(obj, std::forward<C>(cal), "inject");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal, Param const& obj)
    {
        state().add_channel(obj, std::forward<C>(cal), "inject");
        return transit_to<Target>();
    }


    /// Create input channel
    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal)
    {
        state().add_channel(std::forward<C>(cal), "inject");
        return transit_to<Target>();
    }

    template <class... T>
    Target
    InjectTo(boost::json::string_view key, T&&... fmtargs)
    {
        state().add_channel(key | expr::Fmt(fmtargs...), "inject");
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_ObserveOn : protected virtual ModelDefinition::BaseTransition
{
    /// Create input channel with an interface literal
    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal, object_id const& obj)
    {
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal, Param const& obj)
    {
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    /// Create input channel
    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal)
    {
        state().add_channel(std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class... T>
    Target
    ObserveOn(boost::json::string_view key, T&&... fmtargs)
    {
        state().add_channel(key | expr::Fmt(fmtargs...), "observe");
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_Union : protected virtual ModelDefinition::BaseTransition
{
    /// Combine channel outputs in time series
    ///
    /// Combined channels produce a list of pairs [channel alias, requested signal],
    /// sorted by timestamp. Use it in combination with Saturate expr for testing strict or partial
    /// order on mock calls.
    ///
    /// The output is affected by CallRange/Call/CallCount clauses:
    /// - CallRange: take all samples at specified range
    /// - Call (default clause with param = -1 for latest): take only one sample at specified call number
    /// - CallCount: take call count as a sample value and timestamp from the most recent call
    template <class C>
    require_not_str<C, Target>
    Union(C&& cal, object_id const& obj)
    {
        state().combine_channels("union");
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    Union(C&& cal, Param const& obj)
    {
        state().combine_channels("union");
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    Union(C&& cal)
    {
        state().combine_channels("union");
        state().add_channel(std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class... T>
    Target
    Union(boost::json::string_view key, T&&... fmtargs)
    {
        state().combine_channels("union");
        state().add_channel(key | expr::Format(fmtargs...), "observe");
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_With : protected virtual ModelDefinition::BaseTransition
{
    /// Combine channel outputs into an array similarly to Pack keyword
    /// X with Y with Z -> [X, Y, Z]
    template <class C>
    require_not_str<C, Target>
    With(C&& cal, object_id const& obj)
    {
        state().combine_channels("with");
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    With(C&& cal, Param const& obj)
    {
        state().combine_channels("with");
        state().add_channel(obj, std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class C>
    require_not_str<C, Target>
    With(C&& cal)
    {
        state().combine_channels("with");
        state().add_channel(std::forward<C>(cal), "observe");
        return transit_to<Target>();
    }

    template <class... T>
    Target
    With(boost::json::string_view key, T&&... fmtargs)
    {
        state().combine_channels("with");
        state().add_channel(key | expr::Format(fmtargs...), "observe");
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_SignalFilter : protected virtual ModelDefinition::BaseTransition
{

    /// Interface return clause
    /// Refers to the return subsignal at the given JSON Pointer
    Target Return(Expression const& e = "")
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
    Target Args(Expression const& e = "$default")
    {
        state().set_channel_sp("args", e);
        return transit_to<Target>();
    }

    /// Interface argument clause with printf-like format
    template <class T, class... Rest>
    Target Args(boost::json::string_view fmt, T&& arg1, Rest&&... args_rest)
    {
        return Args(fmt | expr::Format(arg1, args_rest...));
    }

    /// Interface exception
    Target Exception()
    {
        state().model("/channels/@/kind") = "exception";
        state().model("/channels/@/signal_path") = "";
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_SignalProperty : protected virtual ModelDefinition::BaseTransition
{
    /// Output capture timestamp
    Target Timestamp()
    {
        state().model("/channels/@/kind") = "ts";
        state().model("/channels/@/signal_path") = "";
        return transit_to<Target>();
    }

    /// Output capture thread id
    Target ThreadId()
    {
        state().model("/channels/@/kind") = "tid";
        state().model("/channels/@/signal_path") = "";
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_CallCount : protected virtual ModelDefinition::BaseTransition
{
    Target CallCount()
    {
        state().model("/channels/@/kind") = "call_count";
        state().model("/channels/@/signal_path") = "";
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Map : protected virtual ModelDefinition::BaseTransition
{
    /// Apply Overload operator
    Target As(boost::json::string_view ref)
    {
        state().model("/channels/@/overload") = ref;
        return transit_to<Target>();
    }

    /// Apply Overload operator
    Target As(SignalOperatorHandler const& op)
    {
        return As(op.annotation());
    }
};

template <class Target>
struct ModelDefinition::T_CallFilter : protected virtual ModelDefinition::BaseTransition
{
    /// Interface call number (0-based index). Negative value is resolved as a reverse index,
    /// with -1 referring to the last call (default)
    Target Call(int cnt)
    {
        state().model("/channels/@/call") = cnt;
        return transit_to<Target>();
    }

    Target Call(Param const& param)
    {
        state().params("/%s/pointers/+", param) = format("%s/call", state().head_channel());
        state().model("/channels/@/call") = param;
        return transit_to<Target>();
    }

    /// Access mock data as list using slice semantic (0-based, inclusive boundaries)
    Target CallRange(int start = 0, int stop = -1, int step = 1)
    {
        state().model("/channels/@/call") = {start, stop, step};
        return transit_to<Target>();
    }

    /// Access mock captures as list using slice semantic (0-based, inclusive boundaries)
    Target CallRange(Param const& param)
    {
        state().params("/%s/pointers/+", param) = format("%s/call", state().head_channel());
        state().model("/channels/@/call") = param;
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Alias : protected virtual ModelDefinition::BaseTransition
{
    Target Alias(boost::json::string_view alias)
    {
        state().model("/channels/@/alias") = alias;
        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Keep : protected virtual ModelDefinition::BaseTransition
{
    /// Set fixed input condition
    Target Keep(Expression const& expr)
    {
        state().model("/channels/@/keep") = expr;
        auto const curcnl = state().cur_cnl_idx();
        auto& params = state().params;

        JsonTraverse([&](boost::json::value const& v, std::string const jp){
            if (Param::isParam(v)) {
                params("/%s/pointers/+", v) = format(
                            "/channels/%d/keep%s", curcnl, jp);
            }
            return false;
        })(expr.underlying());

        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_Expect : protected virtual ModelDefinition::BaseTransition
{
    /// Set fixed output assertion
    Target Expect(Expression const& expr)
    {
        state().model("/channels/@/expect") = expr;
        auto const curcnl = state().cur_cnl_idx();
        auto& params = state().params;

        JsonTraverse([&](boost::json::value const& v, std::string const jp){
            if (Param::isParam(v)) {
                params("/%s/pointers/+", v) = format(
                            "/channels/%d/expect%s", curcnl, jp);
            }
            return false;
        })(expr.underlying());

        return transit_to<Target>();
    }
};


template <class Target>
struct ModelDefinition::T_Test : protected virtual ModelDefinition::BaseTransition
{
    Target Test()
    {
        return transit_to<Target>()();
    }
    Target Test(Expression const& e0)
    {
        return transit_to<Target>()(e0);
    }
    Target Test(Expression const& e0, Expression const& e1)
    {
        return transit_to<Target>()(e0, e1);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2)
    {
        return transit_to<Target>()(e0, e1, e2);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3)
    {
        return transit_to<Target>()(e0, e1, e2, e3);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6, e7);
    }

    template <class... Rest>
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7,
                    Rest&&... rest)
    {
        return transit_to<Target>()(e0, e1, e2, e3, e4, e5, e6, e7, Expression(rest)...);
    }
};

template <class Target>
struct ModelDefinition::T_TestRow : protected virtual ModelDefinition::BaseTransition
{
    Target operator()()
    {
        state().add_test_case({});
        return transit_to<Target>();
    }

    Target operator()(Expression const& e0)
    {
        state().add_test_case({e0});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1)
    {
        state().add_test_case({e0, e1});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2)
    {
        state().add_test_case({e0, e1, e2});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3)
    {
        state().add_test_case({e0, e1, e2, e3});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4)
    {
        state().add_test_case({e0, e1, e2, e3, e4});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6});
        return transit_to<Target>();
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6, e7});
        return transit_to<Target>();
    }

    template <class... Rest>
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7,
                          Rest&&... rest)
    {
        state().add_test_case({e0, e1, e2, e3, e4, e5, e6, e7, Expression(rest)...});
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_TestComment : protected virtual ModelDefinition::BaseTransition
{
    Target operator[](boost::json::string_view comment)
    {
        state().set_comment(comment);
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_Description : protected virtual ModelDefinition::BaseTransition
{
    template <class... T>
    Target Description(boost::json::string_view comment, T&&... args)
    {
        state().set_deferred_param("/description", comment | expr::Format(args...));
        return transit_to<Target>();
    }
};

template <class Target>
struct ModelDefinition::T_Zip : protected virtual ModelDefinition::BaseTransition
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
};

template <class Target>
struct ModelDefinition::T_Prod : protected virtual ModelDefinition::BaseTransition
{
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
