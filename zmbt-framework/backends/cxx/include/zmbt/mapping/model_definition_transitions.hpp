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
#include <zmbt/core/deferred_format.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/core/parameter.hpp>
#include <zmbt/model/expression.hpp>
#include <zmbt/model/expression_api.hpp>
#include <zmbt/model/signal_operator_handler.hpp>
#include <zmbt/model/traits.hpp>
#include <cstddef>
#include <tuple>
#include <utility>

#include "model_definition_helper.hpp"
#include "zmbt/mapping/model_definition.hpp"


#define N_STATE static_cast<Source*>(this)->state()


namespace zmbt {
namespace mapping {


template <class Source, class Target>
struct ModelDefinition::T_OnTrigger
{

    template <class... T>
    Target OnTrigger(boost::json::string_view key, T&&... fmtargs)
    {
        N_STATE.set_deferred_param("/trigger", boost::json::array{key, std::forward<T>(fmtargs)...});
        return Target(N_STATE);
    }

    Target OnTrigger(Param const& key)
    {
        N_STATE.params("/%s/pointers/+", key) = "/trigger";
        N_STATE.model("/trigger") = key;
        return Target(N_STATE);
    }

    Target OnTrigger(Param const& ifc, Param const& obj)
    {
        N_STATE.model("/trigger") = {
            {"obj", obj},
            {"ifc", ifc},
        };
        N_STATE.params("/%s/pointers/+", obj) = "/trigger/obj";
        N_STATE.params("/%s/pointers/+", ifc) = "/trigger/ifc";
        return Target(N_STATE);
    }


    template <class O, class I>
    require_literal<O, I, Target>
    OnTrigger(I&& ifc, O&& obj) {
        auto key = N_STATE.env.RegisterAnonymousTrigger(obj, ifc);
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
        N_STATE.model("/trigger") = {
            {"ifc", N_STATE.env.RegisterParametricTriggerIfc(ifc)}
        };
        N_STATE.params("/%s/pointers/+", obj) = "/trigger/obj";
        return Target(N_STATE);
    }

    template <class H>
    require_obj<H, Target>
    OnTrigger(Param const& ifc, H&& obj)
    {
        N_STATE.model("/trigger") = {
            {"obj", N_STATE.env.RegisterParametricTriggerObj(obj)}
        };
        N_STATE.params("/%s/pointers/+", ifc) = "/trigger/ifc";
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_Repeat
{
    /// Run trigger N times
    Target Repeat(std::size_t N)
    {
        N_STATE.model("/repeat_trigger") = N;
        return Target(N_STATE);
    }

    /// Run trigger N times
    Target Repeat(Param const& N)
    {
        N_STATE.model("/repeat_trigger") = N;
        N_STATE.params("/%s/pointers/+", N) = "/repeat_trigger";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_InjectTo
{
    /// Create input channel with an interface literal
    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal, object_id const& obj)
    {
        N_STATE.add_channel(obj, std::forward<C>(cal), "inject");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal, Param const& obj)
    {
        N_STATE.add_channel(obj, std::forward<C>(cal), "inject");
        return Target(N_STATE);
    }


    /// Create input channel
    template <class C>
    require_not_str<C, Target>
    InjectTo(C&& cal)
    {
        N_STATE.add_channel(std::forward<C>(cal), "inject");
        return Target(N_STATE);
    }

    template <class... T>
    Target
    InjectTo(boost::json::string_view key, T&&... fmtargs)
    {
        N_STATE.add_channel(DeferredFormat(key, std::forward<T>(fmtargs)...), "inject");
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_ObserveOn
{
    /// Create input channel with an interface literal
    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal, object_id const& obj)
    {
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal, Param const& obj)
    {
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    /// Create input channel
    template <class C>
    require_not_str<C, Target>
    ObserveOn(C&& cal)
    {
        N_STATE.add_channel(std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class... T>
    Target
    ObserveOn(boost::json::string_view key, T&&... fmtargs)
    {
        N_STATE.add_channel(DeferredFormat(key, std::forward<T>(fmtargs)...), "observe");
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_Union
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
        N_STATE.combine_channels("union");
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    Union(C&& cal, Param const& obj)
    {
        N_STATE.combine_channels("union");
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    Union(C&& cal)
    {
        N_STATE.combine_channels("union");
        N_STATE.add_channel(std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class... T>
    Target
    Union(boost::json::string_view key, T&&... fmtargs)
    {
        N_STATE.combine_channels("union");
        N_STATE.add_channel(DeferredFormat(key, std::forward<T>(fmtargs)...), "observe");
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_With
{
    /// Combine channel outputs into an array similarly to Pack keyword
    /// X with Y with Z -> [X, Y, Z]
    template <class C>
    require_not_str<C, Target>
    With(C&& cal, object_id const& obj)
    {
        N_STATE.combine_channels("with");
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    With(C&& cal, Param const& obj)
    {
        N_STATE.combine_channels("with");
        N_STATE.add_channel(obj, std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class C>
    require_not_str<C, Target>
    With(C&& cal)
    {
        N_STATE.combine_channels("with");
        N_STATE.add_channel(std::forward<C>(cal), "observe");
        return Target(N_STATE);
    }

    template <class... T>
    Target
    With(boost::json::string_view key, T&&... fmtargs)
    {
        N_STATE.combine_channels("with");
        N_STATE.add_channel(DeferredFormat(key, std::forward<T>(fmtargs)...), "observe");
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_SignalFilter
{
    /// Interface return clause
    /// Refers to the return subsignal at the given JSON Pointer
    template <class... T>
    Target Return(boost::json::string_view base, T&&... tokens)
    {
        N_STATE.set_channel_sp("return", boost::json::array{base, std::forward<T>(tokens)...});
        return Target(N_STATE);
    }

    /// Interface return clause
    Target Return()
    {
        return Return("");
    }

    /// Refers to the arguments subsignal at the given idx
    Target Return(std::size_t idx)
    {
        return Return("/%lu", idx);
    }

    /// Interface argument clause
    /// Refers to the arguments subsignal at the given JSON Pointer
    template <class... T>
    Target Args(boost::json::string_view base, T&&... tokens)
    {
        N_STATE.set_channel_sp("args", boost::json::array{base, std::forward<T>(tokens)...});
        return Target(N_STATE);
    }

    /// Interface default argument clause
    /// Refers to the argument value on unary interfaces or to the arguments tuple otherwise.
    Target Args()
    {
        N_STATE.set_channel_sp("args", "$default");
        return Target(N_STATE);
    }

    /// Interface argument clause
    /// Refers to the arguments subsignal at the given idx
    Target Args(std::size_t idx)
    {
        return Args("/%lu", idx);
    }

    /// Interface exception
    Target Exception()
    {
        N_STATE.model("/channels/@/kind") = "exception";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_SignalProperty
{
    /// Output capture timestamp
    Target Timestamp()
    {
        N_STATE.model("/channels/@/kind") = "ts";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }

    /// Output capture thread id
    Target ThreadId()
    {
        N_STATE.model("/channels/@/kind") = "tid";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_CallCount
{
    Target CallCount()
    {
        N_STATE.model("/channels/@/kind") = "call_count";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_As
{
    /// Apply SignalOperatorHandler for comparisons and transform
    Target As(boost::json::string_view ref)
    {
        N_STATE.model("/channels/@/operator") = ref;
        return Target(N_STATE);
    }

    /// Apply SignalOperatorHandler for comparisons and transform
    Target As(SignalOperatorHandler const& op)
    {
        N_STATE.env.RegisterOperator(op);
        return As(op.annotation());
    }
};

template <class Source, class Target>
struct ModelDefinition::T_CallFilter
{
    /// Interface call number (0-based index). Negative value is resolved as a reverse index,
    /// with -1 referring to the last call (default)
    Target Call(int cnt)
    {
        N_STATE.model("/channels/@/call") = cnt;
        return Target(N_STATE);
    }

    Target Call(Param const& param)
    {
        N_STATE.params("/%s/pointers/+", param) = format("%s/call", N_STATE.head_channel());
        N_STATE.model("/channels/@/call") = param;
        return Target(N_STATE);
    }

    /// Access mock data as list using slice semantic (0-based, inclusive boundaries)
    Target CallRange(int start = 0, int stop = -1, int step = 1)
    {
        N_STATE.model("/channels/@/call") = {start, stop, step};
        return Target(N_STATE);
    }

    /// Access mock captures as list using slice semantic (0-based, inclusive boundaries)
    Target CallRange(Param const& param)
    {
        N_STATE.params("/%s/pointers/+", param) = format("%s/call", N_STATE.head_channel());
        N_STATE.model("/channels/@/call") = param;
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_Alias
{
    Target Alias(boost::json::string_view alias)
    {
        N_STATE.model("/channels/@/alias") = alias;
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_Keep
{
    /// Set input condition
    Target Keep(Expression const& expr)
    {
        N_STATE.model("/channels/@/keep") = expr;
        auto const curcnl = N_STATE.cur_cnl_idx();
        auto& params = N_STATE.params;

        JsonTraverse([&](boost::json::value const& v, std::string const jp){
            if (Param::isParam(v)) {
                params("/%s/pointers/+", v) = format(
                            "/channels/%d/keep%s", curcnl, jp);
            }
            return false;
        })(expr.underlying());

        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct ModelDefinition::T_Test
{
    Target Test(Expression const& e0)
    {
        return Target(N_STATE)(e0);
    }
    Target Test(Expression const& e0, Expression const& e1)
    {
        return Target(N_STATE)(e0, e1);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2)
    {
        return Target(N_STATE)(e0, e1, e2);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3)
    {
        return Target(N_STATE)(e0, e1, e2, e3);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4)
    {
        return Target(N_STATE)(e0, e1, e2, e3, e4);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5)
    {
        return Target(N_STATE)(e0, e1, e2, e3, e4, e5);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6)
    {
        return Target(N_STATE)(e0, e1, e2, e3, e4, e5, e6);
    }
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7)
    {
        return Target(N_STATE)(e0, e1, e2, e3, e4, e5, e6, e7);
    }

    template <class... Rest>
    Target Test(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                    Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7,
                    Rest&&... rest)
    {
        return Target(N_STATE)(e0, e1, e2, e3, e4, e5, e6, e7, Expression(rest)...);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_TestRow
{
    Target operator()(Expression const& e0)
    {
        N_STATE.add_test_case({e0});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1)
    {
        N_STATE.add_test_case({e0, e1});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2)
    {
        N_STATE.add_test_case({e0, e1, e2});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3)
    {
        N_STATE.add_test_case({e0, e1, e2, e3});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4)
    {
        N_STATE.add_test_case({e0, e1, e2, e3, e4});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5)
    {
        N_STATE.add_test_case({e0, e1, e2, e3, e4, e5});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6)
    {
        N_STATE.add_test_case({e0, e1, e2, e3, e4, e5, e6});
        return Target(N_STATE);
    }
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7)
    {
        N_STATE.add_test_case({e0, e1, e2, e3, e4, e5, e6, e7});
        return Target(N_STATE);
    }

    template <class... Rest>
    Target operator()(Expression const& e0, Expression const& e1, Expression const& e2, Expression const& e3,
                          Expression const& e4, Expression const& e5, Expression const& e6, Expression const& e7,
                          Rest&&... rest)
    {
        N_STATE.add_test_case({e0, e1, e2, e3, e4, e5, e6, e7, Expression(rest)...});
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_TestComment
{
    Target operator[](boost::json::string_view comment)
    {
        N_STATE.set_comment(comment);
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_Description
{
    template <class... T>
    Target Description(boost::json::string_view comment, T&&... args)
    {
        N_STATE.set_deferred_param("/description", boost::json::array{comment, std::forward<T>(args)...});
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_Zip
{

    Target Zip(Param const& p, boost::json::value const& v0)
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0);
    }
    Target Zip(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1);
    }

    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3);
    }
    Target Zip(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4);
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
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5);
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
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6);
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
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6, v7);
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
        N_STATE.init_zip();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
    }
    template <class... A>
    Target Zip(Param const& p, A&&... args)
    {
        N_STATE.init_zip();
        return Target(N_STATE)(p, std::forward<A>(args)...);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_Prod
{
    Target Prod(Param const& p, boost::json::value const& v0)
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0);
    }
    Target Prod(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1);
    }

    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3);
    }
    Target Prod(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4);
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
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5);
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
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6);
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
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6, v7);
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
        N_STATE.init_prod();
        return Target(N_STATE)(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
    }
    template <class... A>
    Target Prod(Param const& p, A&&... args)
    {
        N_STATE.init_prod();
        return Target(N_STATE)(p, std::forward<A>(args)...);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_ParamRow
{
    template <class... A>
    Target operator()(Param const& p, A&&... args)
    {
        N_STATE.add_param_values_with_transform(p, args...);
        return Target(N_STATE);
    }

    Target operator()(Param const& p, boost::json::value const& v0)
    {
        N_STATE.add_param_values(p, boost::json::array{v0});
        return Target(N_STATE);
    }
    Target operator()(Param const& p, boost::json::value const& v0, boost::json::value const& v1)
    {
        N_STATE.add_param_values(p, {v0, v1});
        return Target(N_STATE);
    }

    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2
    )
    {
        N_STATE.add_param_values(p, {v0, v1, v2});
        return Target(N_STATE);
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3
    )
    {
        N_STATE.add_param_values(p, {v0, v1, v2, v3});
        return Target(N_STATE);
    }
    Target operator()(Param const& p,
        boost::json::value const& v0,
        boost::json::value const& v1,
        boost::json::value const& v2,
        boost::json::value const& v3,
        boost::json::value const& v4
    )
    {
        N_STATE.add_param_values(p, {v0, v1, v2, v3, v4});
        return Target(N_STATE);
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
        N_STATE.add_param_values(p, {v0, v1, v2, v3, v4, v5});
        return Target(N_STATE);
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
        N_STATE.add_param_values(p, {v0, v1, v2, v3, v4, v5, v6});
        return Target(N_STATE);
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
        N_STATE.add_param_values(p, {v0, v1, v2, v3, v4, v5, v6, v7});
        return Target(N_STATE);
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
        N_STATE.add_param_values_with_transform(p, v0, v1, v2, v3, v4, v5, v6, v7, std::forward<A>(args)...);
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_PreRun
{
    template <class... T>
    Target PreRun(T&&... tasks)
    {
        boost::mp11::tuple_for_each(std::forward_as_tuple(tasks...), [&](auto&& task) { N_STATE.add_task(task, true); });
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct ModelDefinition::T_PostRun
{
    template <class... T>
    Target PostRun(T&&... tasks)
    {
        boost::mp11::tuple_for_each(std::forward_as_tuple(tasks...), [&](auto&& task) { N_STATE.add_task(task, false); });
        return Target(N_STATE);
    }
};


}  // namespace mapping
}  // namespace zmbt

#undef DEFINE_N_CLASS_BODY
#undef N_STATE

#endif
