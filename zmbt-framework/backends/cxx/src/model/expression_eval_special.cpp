/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"
#include "zmbt/model/exceptions.hpp"


#include <boost/regex.hpp>

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
V eval_impl<Keyword::Apply>(V const& param, V const& x, O const& op)
{
    ASSERT(param.is_array());
    ASSERT(x.is_null());
    auto const& expr = param.get_array().at(0);
    auto const& args = param.get_array().at(1);
    return E(expr).eval(args, op);
}


template <>
V eval_impl<Keyword::All>(V const& param, V const& x, O const& op)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (not op.is_truth(E(e).eval(x)))
        {
            return false;
        }
    }
    return true;
}


template <>
V eval_impl<Keyword::Any>(V const& param, V const& x, O const& op)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (op.is_truth(E(e).eval(x)))
        {
            return true;
        }
    }
    return false;
}


template <>
V eval_impl<Keyword::Re>(V const& pattern, V const& sample, O const&)
{
    const boost::regex re(pattern.as_string().c_str());
    std::string s = sample.is_string() ? sample.get_string().c_str() : boost::json::serialize(sample);
    return boost::regex_match(s, re);
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
        boost::json::error_code ec;
        if (boost::json::value const* ptr = value.find_pointer(at.get_string(), ec))
        {
            query = *ptr;
        }
    }
    else
    {
        // TODO: throw?
    }
    return query;
}

template <>
V eval_impl<Keyword::At>(V const& params, V const& sample, O const&)
{
    ASSERT(not params.is_null())

    return query_at(sample, params);
}

template <>
V eval_impl<Keyword::Saturate>(V const& params, V const& samples, O const& op)
{
    if (!samples.is_array())
    {
        return false;
    }
    ASSERT(params.is_array());

    auto const& sample_list = samples.as_array();
    auto const& matches = params.as_array();

    auto it = matches.cbegin();
    for (auto const& sample: sample_list)
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
V eval_impl<Keyword::Count>(V const& params, V const& samples, O const& op)
{
    ASSERT(samples.is_array() || samples.is_object())
    auto const filter = E(params);
    std::size_t count {0};

    if (samples.is_array())
    {
        for (auto const& sample: samples.get_array())
        {
            if (filter.match(sample, op))
            {
                count++;
            }
        }
    }
    else if (samples.is_object())
    {
        for (auto const& kv: samples.get_object())
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
V eval_impl<Keyword::Approx>(V const& params, V const& sample, O const&)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))

    ASSERT(params.is_array() || sample.is_number());

    auto const& args = params.get_array();

    double ref_value = args.at(0).as_double();
    double rtol = args.at(1).as_double();
    double atol = args.size() == 3 ? args.at(2).as_double() : std::numeric_limits<double>::epsilon();

    double value = boost::json::value_to<double>(sample);

    return std::abs(value - ref_value) <= (atol + rtol * std::abs(ref_value));
}


template <>
V eval_impl<Keyword::Size>(V const& params, V const& sample, O const&)
{
    ASSERT(params.is_null())
    V size = nullptr;
    if (sample.is_array())
    {
        size = sample.get_array().size();
    }
    else if (sample.is_object())
    {
        size = sample.get_object().size();
    }
    else
    {
        throw zmbt::expression_error("invalid Size operand: %s", sample);
    }
    return size;
}


template <>
V eval_impl<Keyword::Card>(V const& params, V const& sample, O const&)
{
    ASSERT(params.is_null())
    V card = nullptr;
    if (sample.is_array())
    {
        boost::json::array counter_set {};
        boost::json::array const& observed_as_array {sample.get_array()};
        std::unique_copy(observed_as_array.cbegin(), observed_as_array.cend(), std::back_inserter(counter_set));
        card = counter_set.size();
    }
    else if (sample.is_object())
    {
        card = sample.get_object().size();
    }
    else
    {
        throw zmbt::expression_error("invalid Card operand: %s", sample);
    }
    return card;
}


template <>
V eval_impl<Keyword::Reduce>(V const& expr, V const& value, O const& op)
{
    ASSERT(expr.is_array());
    ASSERT(value.is_array());
    auto const& value_as_array = value.get_array();
    auto const& expr_as_array = expr.get_array();
    ASSERT(expr_as_array.size() >= 1 && expr_as_array.size() <= 2);
    if (value_as_array.empty())
    {
        return nullptr;
    }
    auto F = E(expr_as_array.at(0));
    bool const init_value_defined = expr_as_array.size() > 1;

    auto it = value_as_array.cbegin();

    // take init term
    boost::json::value ret = init_value_defined ? expr_as_array.at(1) : *it++;

    while (it != value_as_array.cend())
    {
        ret = F.eval({ret, *it}, op);
        it++;
    }

    return ret;
}

template <>
V eval_impl<Keyword::Map>(V const& expr, V const& value, O const& op)
{
    ASSERT(value.is_array());
    auto const& value_as_array = value.get_array();
    ASSERT(expr.is_object());
    auto const& expr_as_object = expr.get_object();
    boost::json::array ret {};
    if (value_as_array.empty())
    {
        return ret;
    }
    auto F = E(expr_as_object);

    ret.reserve(value_as_array.size());

    for (auto const& el: value_as_array)
    {
        ret.push_back(F.eval(el, op));
    }

    return ret;
}


template <>
V eval_impl<Keyword::Filter>(V const& expr, V const& value, O const& op)
{

    ASSERT(value.is_array());
    auto const& value_as_array = value.get_array();
    ASSERT(expr.is_object());
    auto const& expr_as_object = expr.get_object();
    boost::json::array ret {};
    if (value_as_array.empty())
    {
        return ret;
    }
    auto F = E(expr_as_object);
    for (auto const& el: value_as_array)
    {
        if (op.is_truth(F.eval(el, op)))
        {
            ret.push_back(el);
        }
    }
    return ret;
}


template <>
V eval_impl<Keyword::Compose>(V const& expr, V const& value, O const& op)
{
    ASSERT(expr.is_array());
    auto const& expr_as_array = expr.get_array();
    ASSERT(expr_as_array.size() > 0);

    auto fn = expr_as_array.crbegin();

    boost::json::value ret {value};

    while (fn != expr_as_array.crend())
    {
        ret = E(*fn++).eval(ret, op);
    }
    return ret;
}

template <>
V eval_impl<Keyword::Repeat>(V const& expr, V const& value, O const&)
{
    ASSERT(expr.is_number());
    std::uint64_t count = boost::json::value_to<std::uint64_t>(expr);
    boost::json::array ret {};
    if (!count)
    {
        return ret;
    }
    ret.reserve(count);
    for (std::uint64_t i = 0; i < count; i++)
    {
        ret.push_back(value);
    }
    return ret;
}


template <>
V eval_impl<Keyword::Recur>(V const& expr, V const& value, O const& op)
{
    ASSERT(expr.is_array());
    ASSERT(value.is_array());
    auto const& value_as_array = value.get_array();
    auto const& expr_as_array = expr.get_array();
    ASSERT(expr_as_array.size() >= 1 && expr_as_array.size() <= 2);
    if (value_as_array.empty())
    {
        return nullptr;
    }
    auto F = E(expr_as_array.at(0));
    std::uint64_t count = boost::json::value_to<std::uint64_t>(expr_as_array.at(1));

    boost::json::value ret {value};
    for (std::uint64_t i = 0; i < count; i++)
    {
        ret = F.eval(ret, op);
    }
    return ret;
}


} // namespace


namespace zmbt
{


boost::json::value Expression::eval_Special(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    switch(keyword())
    {

    #define ZMBT_EXPR_EVAL_IMPL_CASE(K) case Keyword::K: return eval_impl<Keyword::K>(params(), x, op);

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

