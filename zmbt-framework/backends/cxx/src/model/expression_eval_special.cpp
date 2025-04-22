/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <deque>
#include <regex>

#include <boost/format.hpp>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model/generic_signal_operator.hpp"
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"
#include "zmbt/model/exceptions.hpp"



#define ASSERT(E)      if (!(E)) { throw zmbt::expression_error("%s#%d - " #E, __FILE__, __LINE__);}

namespace
{

using V = boost::json::value;
using L = boost::json::array;
using O = zmbt::SignalOperatorHandler;
using E = zmbt::Expression;
using Keyword = zmbt::dsl::Keyword;

template <Keyword keyword>
V eval_impl(V const& x, V const& param, E::EvalContext const& context);


template <>
V eval_impl<Keyword::Apply>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(x); // x is ignored, as Apply essentialy creates a const expr
    ASSERT(param.is_array());
    auto const& expr = param.get_array().at(0);
    auto const& args = param.get_array().at(1);
    return E(expr).eval(args, context++);
}


template <>
V eval_impl<Keyword::All>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (not E::literalAsEq(e).eval(x,context++).as_bool())
        {
            return false;
        }
    }
    return true;
}


template <>
V eval_impl<Keyword::Any>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (E::literalAsEq(e).eval(x,context++).as_bool())
        {
            return true;
        }
    }
    return false;
}


template <>
V eval_impl<Keyword::Re>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(param.is_string());
    ASSERT(x.is_string());

    auto const pattern = param.get_string().c_str();
    auto const sample = x.get_string().c_str();
    std::regex const re(pattern);
    return std::regex_match(sample, re);
}


boost::json::value query_at(boost::json::value const& value, boost::json::value const& at)
{
    boost::json::value query {};

    if (at.is_number() && value.is_structured())
    {
        try
        {
            if (value.is_array())
            {
                query = value.get_array().at(boost::json::value_to<std::size_t>(at));
            }
            else if (value.is_object())
            {
                // lookup table query
                query = value.get_object().at(boost::json::serialize(at));
            }
        }
        catch(std::exception const&)
        {
            // ignore and return default
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
                catch(std::exception const&)
                {
                    // ignore and return default
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
    else if (at.is_number())
    {
        // TODO: throw?
    }
    return query;
}

template <>
V eval_impl<Keyword::At>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(not param.is_null())
    // TODO: check is x is constant expr
    boost::json::value const q = E(x).eval();
    return query_at(q, param);
}

template <>
V eval_impl<Keyword::Lookup>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(not x.is_null())
    return query_at(param, x);
}

template <>
V eval_impl<Keyword::Saturate>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    ASSERT(param.is_array());

    auto const& samples = x.as_array();
    auto const& matches = param.as_array();

    auto it = matches.cbegin();
    for (auto const& sample: samples)
    {
        if (it == matches.cend())
        {
            break;
        }
        if (E::literalAsEq(*it).eval(sample,context++).as_bool())
        {
            it++;
        }
    }
    return it == matches.cend();
}

template <>
V eval_impl<Keyword::Count>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array() || x.is_object())
    auto const filter = E::literalAsEq(param);
    std::size_t count {0};

    if (x.is_array())
    {
        for (auto const& sample: x.get_array())
        {
            if (filter.eval(sample,context++).as_bool())
            {
                count++;
            }
        }
    }
    else if (x.is_object())
    {
        for (auto const& kv: x.get_object())
        {
            if (filter.eval({kv.key(), kv.value()},context++).as_bool())
            {
                count++;
            }
        }
    }

    return count;
}

template <>
V eval_impl<Keyword::Each>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array() || x.is_object())
    auto const filter = E::literalAsEq(param);

    if (x.is_array())
    {
        for (auto const& sample: x.get_array())
        {
            if (!filter.eval(sample,context++).as_bool())
            {
                return false;
            }
        }
    }
    else if (x.is_object())
    {
        for (auto const& kv: x.get_object())
        {
            if (!filter.eval({kv.key(), kv.value()},context++).as_bool())
            {
                return false;
            }
        }
    }

    return true;
}

template <>
V eval_impl<Keyword::Approx>(V const& x, V const& param, E::EvalContext const& context)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))
    // default rtol=1e-05, atol=1e-08
    constexpr double default_rtol = 1e-05;
    constexpr double default_atol = 1e-08;

    ASSERT(x.is_number());
    ASSERT(param.is_array() || param.is_number());

    double x_value = boost::json::value_to<double>(context.op.decorate(x));
    double ref_value = std::numeric_limits<double>::quiet_NaN();
    double rtol      = default_rtol;
    double atol      = default_atol;

    if (param.is_number())
    {
        ref_value = boost::json::value_to<double>(context.op.decorate(param));
    }
    else
    {
        auto const& params = param.get_array();
        ASSERT(params.size() >= 1 && params.size() <= 3);

        if (params.size() >= 1)
        {
            ref_value = boost::json::value_to<double>(context.op.decorate(params.at(0)));
        }
        if (params.size() >= 2)
        {
            rtol = boost::json::value_to<double>(context.op.decorate(params.at(1)));
        }
        if (params.size() == 3)
        {
            atol = boost::json::value_to<double>(context.op.decorate(params.at(2)));
        }
    }

    return std::abs(x_value - ref_value) <= (atol + rtol * std::abs(ref_value));
}

template <>
V eval_impl<Keyword::Uniques>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array());
    return boost::json::value_from(
        boost::json::value_to<std::unordered_set<V>>(x)
    );
}


template <>
V eval_impl<Keyword::Size>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(param.is_null())
    ASSERT(x.is_structured())
    V size = nullptr;
    if (x.is_array())
    {
        return x.get_array().size();
    }
    else
    {
        return x.get_object().size();
    }
}


template <>
V eval_impl<Keyword::Card>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(param.is_null())
    ASSERT(x.is_structured())

    if (x.is_array())
    {
        return eval_impl<Keyword::Uniques>(x, {}, {}).as_array().size();
    }
    else
    {
        return x.get_object().size();
    }
}


template <>
V eval_impl<Keyword::Reduce>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    if (samples.empty())
    {
        return nullptr;
    }
    auto F = E(param);
    auto it = samples.cbegin();

    // take init term
    boost::json::value ret = *it++;

    while (it != samples.cend())
    {
        ret = F.eval({ret, *it},context++);
        it++;
    }

    return ret;
}

template <>
V eval_impl<Keyword::Map>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    auto const F = E(param);
    boost::json::array ret {};
    if (samples.empty())
    {
        return ret;
    }

    ret.reserve(samples.size());

    for (auto const& el: samples)
    {
        ret.push_back(F.eval(el,context++));
    }

    return ret;
}


template <>
V eval_impl<Keyword::Filter>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_object());
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    boost::json::array ret {};
    if (samples.empty())
    {
        return ret;
    }
    auto F = E::literalAsEq(param);
    for (auto const& el: samples)
    {
        if (F.eval(el,context++).as_bool())
        {
            ret.push_back(el);
        }
    }
    return ret;
}


template <>
V eval_impl<Keyword::Compose>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    auto const& funcs = param.get_array();
    ASSERT(funcs.size() > 0);

    auto fn = funcs.crbegin();

    // first el eval as is
    boost::json::value ret {E(*fn++).eval(x,context++)};

    while (fn != funcs.crend())
    {
        // any consequent literals eval as eq
        ret = E::literalAsEq(*fn++).eval(ret,context++);
    }
    return ret;
}

template <>
V eval_impl<Keyword::Repeat>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
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
V eval_impl<Keyword::Recur>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    auto const& params = param.get_array();
    ASSERT(params.size() == 2);
    auto F = E(params.at(0));
    std::uint64_t count = boost::json::value_to<std::uint64_t>(x);
    boost::json::value result {params.at(1)};

    for (std::uint64_t i = 0; i < count; i++)
    {
        result = F.eval(result,context++);
    }
    return result;
}

template <>
V eval_impl<Keyword::Unfold>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    auto const& params = param.get_array();
    ASSERT(params.size() == 2);
    auto F = E(params.at(0));
    std::uint64_t count = boost::json::value_to<std::uint64_t>(x);
    boost::json::array result {};
    result.reserve(count);
    result.push_back(params.at(1));

    for (std::uint64_t i = 0; i < count; i++)
    {
        result.push_back(F.eval(result.back(),context++));
    }
    return result;
}

template <>
V eval_impl<Keyword::Transp>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
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
V eval_impl<Keyword::Cartesian>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
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
V eval_impl<Keyword::Try>(V const& x, V const& param, E::EvalContext const& context)
{
    try
    {
        auto const fn = E(param);
        return fn.eval(x,context++);
    }
    catch(const std::exception& e)
    {
        return nullptr;
    }
}

template <>
V eval_impl<Keyword::TryCatch>(V const& x, V const& param, E::EvalContext const& context)
{
    try
    {
        auto const fn = E(param);
        return fn.eval(x,context++);
    }
    catch(const std::exception& e)
    {
        return {
            {"err", e.what()       },
            {"fn" , param          },
            {"x"  , x              },
            {"op" , context.op.annotation()},
        };
    }
}

template <>
V eval_impl<Keyword::Concat>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind());
    ASSERT(x.is_string() || x.is_array());

    if (x.is_string())
    {
        return {zmbt::format("%s%s", x, param)};
    }
    else if (x.is_array())
    {
        boost::json::array out = x.get_array();
        out.reserve(out.size() + param.get_array().size());
        for (auto const& el: param.get_array())
        {
            out.push_back(el);
        }
        return out;
    }
    return nullptr;
}

template <>
V eval_impl<Keyword::Push>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.is_string() || x.is_array());

    if (x.is_string())
    {
        return {zmbt::format("%s%s", param, x)};
    }
    else if (x.is_array())
    {
        boost::json::array out {};
        out.reserve(1 + x.get_array().size());
        out.push_back(param);
        for (auto const& el: x.get_array())
        {
            out.push_back(el);
        }
        return out;
    }
    return nullptr;
}

template <>
V eval_impl<Keyword::Format>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.is_string());
    ASSERT(param.is_array());
    auto const& tokens = param.get_array();
    if (tokens.empty()) return x;

    boost::format fmt {x.get_string().c_str()};
    auto const N = static_cast<std::size_t>(fmt.expected_args());
    ASSERT(tokens.size() == N);

    for (auto const& token: tokens)
    {
        auto const item = E(token).eval();
        fmt = item.is_string() ? (fmt % item.get_string().c_str()) : (fmt % item);
    }
    return {fmt.str()};
}


template <>
V eval_impl<Keyword::Sort>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const key_fn = param.is_null() ? E(Keyword::Id) : E(param);
    auto const& op = context.op;
    std::function<bool(V const&, V const&)> is_less = [&key_fn, &op](V const& lhs, V const& rhs) ->bool {
        return op.apply(Keyword::Lt, key_fn.eval(lhs), key_fn.eval(rhs)).as_bool();
    };
    boost::json::array out = x.get_array();
    std::stable_sort(out.begin(), out.end(), is_less);
    return out;
}

template <>
V eval_impl<Keyword::Reverse>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array());
    boost::json::array out = x.get_array();
    std::reverse(out.begin(), out.end());
    return out;
}

template <>
V eval_impl<Keyword::Slide>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.is_array());
    ASSERT(param.is_number());

    auto const W = boost::json::value_to<std::uint64_t>(param);
    ASSERT(W > 0);

    auto const& samples = x.get_array();
    boost::json::array out {};
    if (samples.empty() || (0 == W))
    {
        return out;
    }
    if (samples.size() <= W)
    {
        return out;
    }
    out.reserve(samples.size() - W);
    auto window_begin = samples.cbegin();
    auto window_end = window_begin + W;
    while (window_end <= samples.cend())
    {
        out.push_back(boost::json::array(window_begin, window_end));
        window_begin++;
        window_end++;
    }
    return out;
}

template <>
V eval_impl<Keyword::Chunks>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.is_array());
    ASSERT(param.is_number());

    auto const& samples = x.get_array();
    auto const W = boost::json::value_to<std::uint64_t>(param);
    ASSERT(W > 0);
    auto const N = samples.size();
    boost::json::array out {};
    if (samples.empty() || (0 == W))
    {
        return out;
    }
    if (samples.size() <= W)
    {
        out.push_back(samples);
        return out;
    }
    auto stride_begin = samples.cbegin();
    auto stride_end = stride_begin + W;
    auto stride_boundary = samples.cbegin() + (N - (N % W));

    while (stride_end <= stride_boundary)
    {
        out.push_back(boost::json::array(stride_begin, stride_end));
        stride_begin += W;
        stride_end += W;
    }
    if (stride_boundary != samples.cend())
    {
        out.push_back(boost::json::array(stride_boundary, samples.cend()));
    }
    return out;
}

template <>
V eval_impl<Keyword::Stride>(V const& x, V const& param, E::EvalContext const& context)
{
    auto result = eval_impl<Keyword::Chunks>(x, param, context).as_array();

    if (!result.empty() && result.back().as_array().size() != param)
    {
        result.pop_back();
    }
    return result;
}


template <>
V eval_impl<Keyword::Sum>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    ASSERT(x.is_array());
    return eval_impl<Keyword::Reduce>(x, E(Keyword::Add),context++);
}

template <>
V eval_impl<Keyword::Prod>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    ASSERT(x.is_array());
    return eval_impl<Keyword::Reduce>(x, E(Keyword::Mul),context++);
}

template <>
V eval_impl<Keyword::Avg>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    ASSERT(x.is_array());
    auto const N = eval_impl<Keyword::Size>(x, {}, {});
    auto const sum = eval_impl<Keyword::Reduce>(x, E(Keyword::Add), {});
    return context.op.apply(Keyword::Div, sum, N);
}

template <>
V eval_impl<Keyword::Pack>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(param.is_array());
    auto const& funcs = param.get_array();
    boost::json::array out {};
    out.reserve(funcs.size());
    for (auto const& fn: funcs)
    {
        out.push_back(E(fn).eval(x,context++));
    }
    return out;
}

using compare_fn_t = std::function<bool(V const&, V const&)>;

using get_element_fn_t =
std::function<L::const_iterator(L::const_iterator, L::const_iterator, compare_fn_t const&)>;

L::const_iterator find_argminmax(L const& samples, V const& param, O const& op, get_element_fn_t const& get_element)
{
    E const key_fn = param.is_null() ? E(Keyword::Id) : E(param);
    // TODO: add log?
    std::function<bool(V const&, V const&)> is_less = [key_fn, op](V const& lhs, V const& rhs) ->bool {
        return op.apply(Keyword::Lt, key_fn.eval(lhs), key_fn.eval(rhs)).as_bool();
    };
    return get_element(samples.cbegin(), samples.cend(), is_less);
}

template <>
V eval_impl<Keyword::Argmin>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, param, context.op, &std::min_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

template <>
V eval_impl<Keyword::Argmax>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, param, context.op, &std::max_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

template <>
V eval_impl<Keyword::Min>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, param, context.op, &std::min_element<L::const_iterator, compare_fn_t>);
}

template <>
V eval_impl<Keyword::Max>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(x.is_array());
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, param, context.op, &std::max_element<L::const_iterator, compare_fn_t>);
}


template <>
V eval_impl<Keyword::Intersect>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind());
    ASSERT(x.is_array());

    auto const lhs = boost::json::value_to<std::unordered_set<V>>(x);
    auto const rhs = boost::json::value_to<std::unordered_set<V>>(param);
    auto const& op = context.op;
    L intersection {};
    intersection.reserve(std::max(lhs.size(), rhs.size()));
    auto out_it = std::back_inserter(intersection);

    auto lhs_it = lhs.cbegin();
    while (lhs_it != lhs.cend())
    {
        if (std::find_if(rhs.cbegin(), rhs.cend(), [&op, lhs_it](V const& v){ return op.apply(Keyword::Eq, v, *lhs_it).as_bool();}) != rhs.cend())
        {
            *out_it = *lhs_it;
            out_it++;
        }
        lhs_it++;
    }
    intersection.shrink_to_fit();
    return intersection;
}

template <>
V eval_impl<Keyword::Diff>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind());
    ASSERT(x.is_structured());

    auto const lhs = boost::json::value_to<std::unordered_set<V>>(x);
    auto const rhs = boost::json::value_to<std::unordered_set<V>>(param);
    auto const& op = context.op;
    L difference {};
    difference.reserve(std::max(lhs.size(), rhs.size()));
    auto out_it = std::back_inserter(difference);

    auto lhs_it = lhs.cbegin();
    while (lhs_it != lhs.cend())
    {
        if (std::find_if(rhs.cbegin(), rhs.cend(), [&op, lhs_it](V const& v){ return op.apply(Keyword::Eq, v, *lhs_it).as_bool();}) == rhs.cend())
        {
            *out_it = *lhs_it;
            out_it++;
        }
        lhs_it++;
    }
    difference.shrink_to_fit();
    return difference;
}


template <>
V eval_impl<Keyword::Union>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind());
    ASSERT(x.is_structured());



    auto const lhs = boost::json::value_to<std::unordered_set<V>>(x);
    auto const rhs = boost::json::value_to<std::unordered_set<V>>(param);
    auto const& op = context.op;
    L set_union {};
    set_union.reserve(lhs.size() + rhs.size());

    auto out_it = std::back_inserter(set_union);
    std::copy(lhs.cbegin(), lhs.cend(), out_it);
    std::copy_if(
        rhs.cbegin(),
        rhs.cend(),
        out_it,
        [&op, &lhs](V const& relem){
            return std::find_if(
                lhs.cbegin(),
                lhs.cend(),
                [&op, &relem](V const& lelem){
                    return op.apply(Keyword::Eq, lelem, relem).as_bool();
                }
            ) == lhs.cend();
        }
    );

    return set_union;
}

template <>
V eval_impl<Keyword::Arange>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    static_cast<void>(param);
    ASSERT(x.is_number() || x.is_array() || x.is_string());

    std::int64_t start = 0;
    std::int64_t stop = 0;
    std::int64_t step = 1;

    if (x.is_number())
    {
        stop = boost::json::value_to<int>(x);
    }
    else if (x.is_array())
    {
        auto const& params = x.get_array();
        ASSERT(params.size() >= 1 && params.size() <= 3);
        if (params.size() == 1)
        {
            stop = boost::json::value_to<std::int64_t>(params.at(0));
        }
        else if (params.size() >= 2)
        {
            start = boost::json::value_to<std::int64_t>(params.at(0));
            stop = boost::json::value_to<std::int64_t>(params.at(1));
        }
        if (params.size() == 3)
        {
            step = boost::json::value_to<std::int64_t>(params.at(2));
        }
    }
    else if (x.is_string())
    {
        auto const slice_idx = zmbt::detail::str_to_slice_idx(x.get_string());
        start = slice_idx.at(0);
        stop = slice_idx.at(1);
        step = slice_idx.at(2);
    }

    if (step == 0)
    {
        throw zmbt::expression_error("step cannot be zero");
    }

    L out {};
    if ((step > 0 && start >= stop) || (step < 0 && start <= stop))
    {
        return out;
    }

    out.reserve(std::max(0l, (stop - start) / step));
    for (std::int64_t i = start; (stop > start) ? i < stop : i > stop; i += step)
    {
        out.push_back(i);
    }
    return out;
}

template <>
V eval_impl<Keyword::Items>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object());
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back({kv.key(), kv.value()});
    }
    return out;
}

template <>
V eval_impl<Keyword::Keys>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object());
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.key());
    }
    return out;
}

template <>
V eval_impl<Keyword::Values>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object());
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.value());
    }
    return out;
}

template <>
V eval_impl<Keyword::Enumerate>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array());
    auto const& arr = x.get_array();
    boost::json::array out {};
    out.reserve(arr.size());
    std::size_t i = 0;
    for (auto const& el: arr)
    {
        out.push_back({i++, el});
    }
    return out;
}

template <>
V eval_impl<Keyword::Flatten>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array());
    auto const& arr = x.get_array();
    boost::json::array out {};
    out.reserve(arr.size());
    for (auto const& el: arr)
    {
        if (el.is_array())
        {
            for (auto const& subel: el.get_array())
            {
                out.push_back(subel);
            }
        }
        else
        {
            out.push_back(el);
        }
    }
    return out;
}



template <>
V eval_impl<Keyword::Flip>(V const& x, V const& param, E::EvalContext const& context)
{
    auto const f = E(param);
    auto const flip = E(f.keyword(), x);
    return flip.eval(f.params(), context++);
}

} // namespace


namespace zmbt
{


boost::json::value Expression::eval_Special(boost::json::value const& x, EvalContext const& context) const
{
    V const* x_ptr {nullptr};
    V const* param_ptr {nullptr};
    if (dsl::detail::isBinary(keyword()))
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

    #define ZMBT_EXPR_EVAL_IMPL_CASE(K) case Keyword::K: return eval_impl<Keyword::K>(*x_ptr, *param_ptr, context);

        // terms special
        ZMBT_EXPR_EVAL_IMPL_CASE(Approx)
        ZMBT_EXPR_EVAL_IMPL_CASE(Re)
        ZMBT_EXPR_EVAL_IMPL_CASE(At)
        ZMBT_EXPR_EVAL_IMPL_CASE(Lookup)

        // props
        ZMBT_EXPR_EVAL_IMPL_CASE(Card)
        ZMBT_EXPR_EVAL_IMPL_CASE(Size)
        ZMBT_EXPR_EVAL_IMPL_CASE(Sum)
        ZMBT_EXPR_EVAL_IMPL_CASE(Prod)
        ZMBT_EXPR_EVAL_IMPL_CASE(Avg)


        // combo
        ZMBT_EXPR_EVAL_IMPL_CASE(All)
        ZMBT_EXPR_EVAL_IMPL_CASE(Any)

        // high-orded
        ZMBT_EXPR_EVAL_IMPL_CASE(Compose)
        ZMBT_EXPR_EVAL_IMPL_CASE(Count)
        ZMBT_EXPR_EVAL_IMPL_CASE(Each)
        ZMBT_EXPR_EVAL_IMPL_CASE(Map)
        ZMBT_EXPR_EVAL_IMPL_CASE(Filter)
        ZMBT_EXPR_EVAL_IMPL_CASE(Recur)
        ZMBT_EXPR_EVAL_IMPL_CASE(Unfold)
        ZMBT_EXPR_EVAL_IMPL_CASE(Reduce)
        ZMBT_EXPR_EVAL_IMPL_CASE(Saturate)
        ZMBT_EXPR_EVAL_IMPL_CASE(Apply)
        ZMBT_EXPR_EVAL_IMPL_CASE(Try)
        ZMBT_EXPR_EVAL_IMPL_CASE(TryCatch)
        ZMBT_EXPR_EVAL_IMPL_CASE(Pack)
        ZMBT_EXPR_EVAL_IMPL_CASE(Flip)


        // vector ops
        ZMBT_EXPR_EVAL_IMPL_CASE(Repeat)
        ZMBT_EXPR_EVAL_IMPL_CASE(Transp)
        ZMBT_EXPR_EVAL_IMPL_CASE(Cartesian)
        ZMBT_EXPR_EVAL_IMPL_CASE(Concat)
        ZMBT_EXPR_EVAL_IMPL_CASE(Push)
        ZMBT_EXPR_EVAL_IMPL_CASE(Uniques)
        ZMBT_EXPR_EVAL_IMPL_CASE(Sort)
        ZMBT_EXPR_EVAL_IMPL_CASE(Reverse)
        ZMBT_EXPR_EVAL_IMPL_CASE(Slide)
        ZMBT_EXPR_EVAL_IMPL_CASE(Chunks)
        ZMBT_EXPR_EVAL_IMPL_CASE(Stride)
        ZMBT_EXPR_EVAL_IMPL_CASE(Arange)

        ZMBT_EXPR_EVAL_IMPL_CASE(Items)
        ZMBT_EXPR_EVAL_IMPL_CASE(Keys)
        ZMBT_EXPR_EVAL_IMPL_CASE(Values)
        ZMBT_EXPR_EVAL_IMPL_CASE(Enumerate)
        ZMBT_EXPR_EVAL_IMPL_CASE(Flatten)

        ZMBT_EXPR_EVAL_IMPL_CASE(Argmin)
        ZMBT_EXPR_EVAL_IMPL_CASE(Argmax)
        ZMBT_EXPR_EVAL_IMPL_CASE(Min)
        ZMBT_EXPR_EVAL_IMPL_CASE(Max)

        // set ops
        ZMBT_EXPR_EVAL_IMPL_CASE(Union)
        ZMBT_EXPR_EVAL_IMPL_CASE(Intersect)
        ZMBT_EXPR_EVAL_IMPL_CASE(Diff)

        // string ops
        ZMBT_EXPR_EVAL_IMPL_CASE(Format)

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

