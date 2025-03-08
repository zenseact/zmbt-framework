/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <boost/regex.hpp>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"
#include "zmbt/model/exceptions.hpp"



#define ASSERT(E)      if (!(E)) { throw zmbt::expression_error("%s#%d - " #E, __FILE__, __LINE__);}

namespace
{

using V = boost::json::value;
using O = zmbt::SignalOperatorHandler;
using E = zmbt::Expression;
using Keyword = zmbt::expr::Keyword;

template <Keyword keyword>
V eval_impl(V const& params, V const& x, O const& op);


template <>
V eval_impl<Keyword::Apply>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    ASSERT(x.is_null());
    auto const& expr = param.get_array().at(0);
    auto const& args = param.get_array().at(1);
    return E(expr).eval(args, op);
}


template <>
V eval_impl<Keyword::All>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (not E(e).match(x, op))
        {
            return false;
        }
    }
    return true;
}


template <>
V eval_impl<Keyword::Any>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (E(e).match(x, op))
        {
            return true;
        }
    }
    return false;
}


template <>
V eval_impl<Keyword::Re>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_string());
    auto const pattern = param.get_string().c_str();
    const boost::regex re(pattern);
    std::string sample = x.is_string() ? x.get_string().c_str() : boost::json::serialize(x);
    return boost::regex_match(sample, re);
}


boost::json::value query_at(boost::json::value const& value, boost::json::value const& at)
{
    boost::json::value query {};

    if (at.is_number() && value.is_array())
    {
        try
        {
            query = value.get_array().at(boost::json::value_to<std::size_t>(at));
        }
        catch(...)
        {
        }
    }
    else if (at.is_array())
    {
        query.emplace_array();
        for (auto const& jp: at.get_array())
        {
            query.get_array().push_back(query_at(value, jp));
        }
    }
    else if (at.is_object())
    {
        query.emplace_object();
        auto& as_object = query.get_object();
        for (auto const& kv: at.get_object())
        {
            if (kv.key().starts_with("$")) // dynamic key as ptr
            {
                try
                {
                    auto dynamic_key = query_at(value, kv.key().substr(1));
                    if (dynamic_key.is_number())
                    {
                        dynamic_key = zmbt::format("%s", dynamic_key);
                    }
                    as_object.emplace(dynamic_key.as_string(), query_at(value, kv.value()));
                }
                catch(...)
                {
                }
            }
            else
            {
                as_object.emplace(kv.key(), query_at(value, kv.value()));
            }
        }
    }
    else if (at.is_string())
    {
        auto const& token = at.get_string();
        if (token.starts_with("/") or token.empty())
        {
            boost::json::error_code ec;
            if (boost::json::value const* ptr = value.find_pointer(at.get_string(), ec))
            {
                query = *ptr;
            }
        }
        else if (value.is_array())
        {
            auto const slice_idx = zmbt::detail::str_to_slice_idx(token);
            query = zmbt::slice(value.get_array(), slice_idx.at(0),slice_idx.at(1),slice_idx.at(2));
        }
    }
    else
    {
        // TODO: throw?
    }
    return query;
}

template <>
V eval_impl<Keyword::At>(V const& x, V const& param, O const&)
{
    ASSERT(not param.is_null())
    return query_at(x, param);
}

template <>
V eval_impl<Keyword::Saturate>(V const& x, V const& params, O const& op)
{
    ASSERT(x.is_array());
    ASSERT(params.is_array());

    auto const& samples = x.as_array();
    auto const& matches = params.as_array();

    auto it = matches.cbegin();
    for (auto const& sample: samples)
    {
        if (it == matches.cend())
        {
            break;
        }
        if (E(*it).match(sample, op))
        {
            it++;
        }
    }
    return it == matches.cend();
}

template <>
V eval_impl<Keyword::Count>(V const& x, V const& param, O const& op)
{
    ASSERT(x.is_array() || x.is_object())
    auto const filter = E(param);
    std::size_t count {0};

    if (x.is_array())
    {
        for (auto const& sample: x.get_array())
        {
            if (filter.match(sample, op))
            {
                count++;
            }
        }
    }
    else if (x.is_object())
    {
        for (auto const& kv: x.get_object())
        {
            if (filter.match({kv.key(), kv.value()}, op))
            {
                count++;
            }
        }
    }

    return count;
}

template <>
V eval_impl<Keyword::Approx>(V const& x, V const& param, O const&)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))
    ASSERT(x.is_number());
    ASSERT(param.is_array());
    auto const& params = param.get_array();
    ASSERT(params.size() >= 2 && params.size() <= 3);

    double ref_value = boost::json::value_to<double>(params.at(0));
    double rtol      = boost::json::value_to<double>(params.at(1));
    double atol      = params.size() == 3
        ? boost::json::value_to<double>(params.at(2))
        : std::numeric_limits<double>::epsilon();

    double value = boost::json::value_to<double>(x);
    return std::abs(value - ref_value) <= (atol + rtol * std::abs(ref_value));
}


template <>
V eval_impl<Keyword::Size>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_null())
    V size = nullptr;
    if (x.is_array())
    {
        size = x.get_array().size();
    }
    else if (x.is_object())
    {
        size = x.get_object().size();
    }
    else
    {
        throw zmbt::expression_error("invalid Size operand: %s", x);
    }
    return size;
}


template <>
V eval_impl<Keyword::Card>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_null())
    V card = nullptr;
    if (x.is_array())
    {
        boost::json::array counter_set {};
        boost::json::array const& observed_as_array {x.get_array()};
        std::unique_copy(observed_as_array.cbegin(), observed_as_array.cend(), std::back_inserter(counter_set));
        card = counter_set.size();
    }
    else if (x.is_object())
    {
        card = x.get_object().size();
    }
    else
    {
        throw zmbt::expression_error("invalid Card operand: %s", x);
    }
    return card;
}


template <>
V eval_impl<Keyword::Reduce>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    auto const& params = param.get_array();
    ASSERT(params.size() >= 1 && params.size() <= 2);
    if (samples.empty())
    {
        return nullptr;
    }
    auto F = E(params.at(0));
    bool const init_value_defined = params.size() > 1;

    auto it = samples.cbegin();

    // take init term
    boost::json::value ret = init_value_defined ? params.at(1) : *it++;

    while (it != samples.cend())
    {
        ret = F.eval({ret, *it}, op);
        it++;
    }

    return ret;
}

template <>
V eval_impl<Keyword::Map>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_object());
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    boost::json::array ret {};
    if (samples.empty())
    {
        return ret;
    }
    auto F = E(param);

    ret.reserve(samples.size());

    for (auto const& el: samples)
    {
        ret.push_back(F.eval(el, op));
    }

    return ret;
}


template <>
V eval_impl<Keyword::Filter>(V const& x, V const& param, O const& op)
{

    ASSERT(param.is_object());
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    boost::json::array ret {};
    if (samples.empty())
    {
        return ret;
    }
    auto F = E(param);
    for (auto const& el: samples)
    {
        if (F.match(el, op))
        {
            ret.push_back(el);
        }
    }
    return ret;
}


template <>
V eval_impl<Keyword::Compose>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    auto const& funcs = param.get_array();
    ASSERT(funcs.size() > 0);

    auto fn = funcs.crbegin();

    boost::json::value ret {x};

    while (fn != funcs.crend())
    {
        ret = E(*fn++).eval(ret, op);
    }
    return ret;
}

template <>
V eval_impl<Keyword::Repeat>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_number());
    std::uint64_t count = boost::json::value_to<std::uint64_t>(param);
    boost::json::array ret {};
    if (!count)
    {
        return ret;
    }
    ret.reserve(count);
    for (std::uint64_t i = 0; i < count; i++)
    {
        ret.push_back(x);
    }
    return ret;
}


template <>
V eval_impl<Keyword::Recur>(V const& x, V const& param, O const& op)
{
    ASSERT(param.is_array());
    auto const& params = param.get_array();
    ASSERT(params.size() >= 1 && params.size() <= 2);
    auto F = E(params.at(0));
    std::uint64_t count = boost::json::value_to<std::uint64_t>(params.at(1));
    boost::json::value ret {x};
    for (std::uint64_t i = 0; i < count; i++)
    {
        ret = F.eval(ret, op);
    }
    return ret;
}

template <>
V eval_impl<Keyword::List>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_array());
    ASSERT(x.is_null());
    return param;
}


template <>
V eval_impl<Keyword::Transp>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_null());
    ASSERT(x.is_array());
    auto const& arr = x.get_array();
    if (arr.empty()) return arr;
    auto const N = arr.at(0).as_array().size();
    ASSERT(std::all_of(arr.cbegin(), arr.cend(), [N](auto const& el) -> bool { return el.is_array() && el.as_array().size() == N;}));

    zmbt::JsonZipIter zip {arr};
    boost::json::array out {};
    out.reserve(N);
    while(!zip.halt())
    {
        out.push_back(*zip);
        zip++;
    }
    return out;
}


template <>
V eval_impl<Keyword::Cartesian>(V const& x, V const& param, O const&)
{
    ASSERT(param.is_null());
    ASSERT(x.is_array());
    auto const& arr = x.get_array();
    if (arr.empty()) return arr;
    std::size_t n {1};

    ASSERT(std::all_of(arr.cbegin(), arr.cend(), [&n](auto const& el) -> bool {
        if (el.is_array())
        {
            n *= el.get_array().size();
            return true;
        }
        return false;
    }));

    zmbt::JsonProdIter zip {arr};
    boost::json::array out {};
    out.reserve(n);
    while(!zip.halt())
    {
        out.push_back(*zip);
        zip++;
    }
    return out;
}

template <>
V eval_impl<Keyword::Try>(V const& x, V const& param, O const& op)
{
    try
    {
        auto const fn = E(param);
        return fn.eval(x, op);
    }
    catch(const std::exception& e)
    {
        return nullptr;
    }
}

template <>
V eval_impl<Keyword::TryCatch>(V const& x, V const& param, O const& op)
{
    try
    {
        auto const fn = E(param);
        return fn.eval(x, op);
    }
    catch(const std::exception& e)
    {
        return {
            {"err", e.what()       },
            {"fn" , param          },
            {"x"  , x              },
            {"op" , op.annotation()},
        };
    }
}


} // namespace


namespace zmbt
{


boost::json::value Expression::eval_Special(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    V const* x_ptr {nullptr};
    V const* param_ptr {nullptr};
    if (expr::detail::isBinary(keyword()))
    {
        handle_binary_args(x, x_ptr, param_ptr);
    }
    else
    {
        x_ptr = &x;
        param_ptr = &params();
    }
    ASSERT(x_ptr)
    ASSERT(param_ptr)

    switch(keyword())
    {

    #define ZMBT_EXPR_EVAL_IMPL_CASE(K) case Keyword::K: return eval_impl<Keyword::K>(*x_ptr, *param_ptr, op);

        // terms special
        ZMBT_EXPR_EVAL_IMPL_CASE(Approx)
        ZMBT_EXPR_EVAL_IMPL_CASE(Re)
        ZMBT_EXPR_EVAL_IMPL_CASE(At)

        // props
        ZMBT_EXPR_EVAL_IMPL_CASE(Card)
        ZMBT_EXPR_EVAL_IMPL_CASE(Size)

        // combo
        ZMBT_EXPR_EVAL_IMPL_CASE(All)
        ZMBT_EXPR_EVAL_IMPL_CASE(Any)

        // high-orded
        ZMBT_EXPR_EVAL_IMPL_CASE(Compose)
        ZMBT_EXPR_EVAL_IMPL_CASE(Count)
        ZMBT_EXPR_EVAL_IMPL_CASE(Map)
        ZMBT_EXPR_EVAL_IMPL_CASE(Filter)
        ZMBT_EXPR_EVAL_IMPL_CASE(Recur)
        ZMBT_EXPR_EVAL_IMPL_CASE(Reduce)
        ZMBT_EXPR_EVAL_IMPL_CASE(Saturate)
        ZMBT_EXPR_EVAL_IMPL_CASE(Apply)

        // vector ops
        ZMBT_EXPR_EVAL_IMPL_CASE(Repeat)
        ZMBT_EXPR_EVAL_IMPL_CASE(List)
        ZMBT_EXPR_EVAL_IMPL_CASE(Transp)
        ZMBT_EXPR_EVAL_IMPL_CASE(Cartesian)

        ZMBT_EXPR_EVAL_IMPL_CASE(Try)
        ZMBT_EXPR_EVAL_IMPL_CASE(TryCatch)

        default:
        {
            // TODO: throw
            throw zmbt::expression_not_implemented("%s keyword not implemented", json_from(keyword()));
            return nullptr;
        }
    }
    #undef ZMBT_EXPR_EVAL_IMPL_CASE
}

} // namespace zmbt

