/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MAPPING_DEFINITION_MACHINE_TRANSITIONS_HPP_
#define ZMBT_MAPPING_DEFINITION_MACHINE_TRANSITIONS_HPP_

#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <zmbt/core/aliases.hpp>
#include <zmbt/core/deferred_format.hpp>
#include <zmbt/core/format_string.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/core/parameter.hpp>
#include <zmbt/model/expression.hpp>
#include <zmbt/model/signal_operator_handler.hpp>
#include <zmbt/model/traits.hpp>
#include <cstddef>
#include <tuple>
#include <utility>

#include "definition_helper.hpp"
#include "zmbt/mapping/definition_machine.hpp"


#define N_STATE static_cast<Source*>(this)->state()


namespace zmbt {
namespace mapping {


template <class Source, class Target>
struct DefinitionMachine::T_OnTrigger
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

    Target OnTrigger(Param const& obj, Param const& ifc)
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
    OnTrigger(O&& obj, I&& ifc) {
        auto key = N_STATE.env.RegisterTriggerLiteral(obj, ifc);
        return OnTrigger(key);
    }

    template <class I>
    require_cal<I, Target>
    OnTrigger(I&& ifc) {
        static_assert(!is_member_pointer<I>::value, "");
        return OnTrigger(ifc_host_nullptr<I>, std::forward<I>(ifc));
    }


    template <class I>
    require_cal<I, Target>
    OnTrigger(Param const& obj, I&& ifc)
    {
        N_STATE.model("/trigger") = {
            {"ifc", N_STATE.env.RegisterParametricTriggerIfc(ifc)}
        };
        N_STATE.params("/%s/pointers/+", obj) = "/trigger/obj";
        return Target(N_STATE);
    }

    template <class H>
    require_obj<H, Target>
    OnTrigger(H&& obj, Param const& ifc)
    {
        N_STATE.model("/trigger") = {
            {"obj", N_STATE.env.RegisterParametricTriggerObj(obj)}
        };
        N_STATE.params("/%s/pointers/+", ifc) = "/trigger/ifc";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_InjectTo
{
    /// Create input channel with an interface literal
    template <class O, class C>
    require_not_str<O, Target>
    InjectTo(O&& obj, C&& cal)
    {
        N_STATE.add_channel(std::forward<O>(obj), std::forward<C>(cal), "inject");
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
struct DefinitionMachine::T_ObserveOn
{
    /// Create input channel with an interface literal
    template <class O, class C>
    require_not_str<O, Target>
    ObserveOn(O&& obj, C&& cal)
    {
        N_STATE.add_channel(std::forward<O>(obj), std::forward<C>(cal), "observe");
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
struct DefinitionMachine::T_Return
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

};

template <class Source, class Target>
struct DefinitionMachine::T_Args
{
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
};

template <class Source, class Target>
struct DefinitionMachine::T_Exception
{
    /// Interface exception
    Target Exception()
    {
        N_STATE.model("/channels/@/kind") = "exception";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Timestamp
{
    /// Interface exception
    Target Timestamp()
    {
        N_STATE.model("/channels/@/kind") = "ts";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_ThreadId
{
    /// Interface exception
    Target ThreadId()
    {
        N_STATE.model("/channels/@/kind") = "tid";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};


template <class Source, class Target>
struct DefinitionMachine::T_As
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
struct DefinitionMachine::T_OnCall
{
    /// Interface call number (1-based index). Negative value is resolved as a reverse index,
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
};

template <class Source, class Target>
struct DefinitionMachine::T_CallRange
{
    /// Access mock data as list using slice semantic (1-based, inclusive boundaries)
    Target CallRange(int start = 1, int stop = -1, int step = 1)
    {
        N_STATE.model("/channels/@/call") = {start, stop, step};
        return Target(N_STATE);
    }

    /// Access mock captures as list using slice semantic (1-based, inclusive boundaries)
    Target CallRange(Param const& param)
    {
        N_STATE.params("/%s/pointers/+", param) = format("%s/call", N_STATE.head_channel());
        N_STATE.model("/channels/@/call") = param;
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_CallRangeIn
{
    /// Inkect mock input data as list or map
    Target CallRange()
    {
        N_STATE.model("/channels/@/call") = {1, -1, 1};
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_CallCount
{
    Target CallCount()
    {
        N_STATE.model("/channels/@/kind") = "call_count";
        N_STATE.model("/channels/@/signal_path") = "";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Alias
{
    Target Alias(boost::json::string_view alias)
    {
        N_STATE.model("/channels/@/alias") = alias;
        return Target(N_STATE);
    }
};
template <class Source, class Target>
struct DefinitionMachine::T_InSeries
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
    ///
    /// Can't be combined with Join()
    Target MergeInSeries()
    {
        auto const curcnl = N_STATE.cur_cnl_idx();
        if (N_STATE.model.get_or_default(format("/channels/%s/combine", curcnl - 1), "") == "join")
        {
            throw model_error("can't combine MergeInSeries with Join");
        }

        N_STATE.model("%s/combine", N_STATE.head_channel()) = "series";

        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Join
{
    /// Combine channel outputs into an array
    /// X join Y join Z -> [X, Y, Z]
    ///
    /// Can't be combined with MergeInSeries()
    Target Join()
    {
        auto const curcnl = N_STATE.cur_cnl_idx();
        if (N_STATE.model.get_or_default(format("/channels/%s/combine", curcnl - 1), "") == "series")
        {
            throw model_error("can't combine Join with MergeInSeries");
        }
        N_STATE.model("%s/combine", N_STATE.head_channel()) = "join";
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Test
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
struct DefinitionMachine::T_TestRow
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
struct DefinitionMachine::T_TestComment
{
    Target operator[](boost::json::string_view comment)
    {
        N_STATE.set_comment(comment);
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Description
{
    template <class... T>
    Target Description(boost::json::string_view comment, T&&... args)
    {
        N_STATE.set_deferred_param("/description", boost::json::array{comment, std::forward<T>(args)...});
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_Zip
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
struct DefinitionMachine::T_Prod
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
struct DefinitionMachine::T_ParamRow
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
struct DefinitionMachine::T_PreRun
{
    template <class... T>
    Target PreRun(T&&... tasks)
    {
        boost::mp11::tuple_for_each(std::forward_as_tuple(tasks...), [&](auto&& task) { N_STATE.add_task(task, true); });
        return Target(N_STATE);
    }
};

template <class Source, class Target>
struct DefinitionMachine::T_PostRun
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
