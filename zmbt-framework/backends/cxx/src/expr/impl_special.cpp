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
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/exceptions.hpp"

#define ASSERT(E)      if (!(E)) { throw zmbt::expression_error("%s#%d - " #E, __FILE__, __LINE__);}


using V = boost::json::value;
using L = boost::json::array;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;

namespace
{

template <Keyword keyword>
V eval_impl(V const& x, V const& param, E::EvalContext const& context);


template <>
V eval_impl<Keyword::All>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    for (auto const& e: param.get_array())
    {
        if (not E::asPredicate(e).eval(x,context++).as_bool())
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
        if (E::asPredicate(e).eval(x,context++).as_bool())
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
        auto x_as_array = value.if_array();
        auto x_as_object = value.if_object();

        auto const N = x_as_array ? x_as_array->size() : x_as_object ? x_as_object->size() : 0;

        std::size_t idx{};
        switch (at.kind())
        {
        case boost::json::kind::int64:
        case boost::json::kind::double_:
            {
                std::int64_t at_idx = boost::json::value_to<std::int64_t>(at);
                idx = (at_idx >= 0) ? at_idx : (at_idx + N); // TODO: handle overflow
            }
            break;
        case boost::json::kind::uint64:
            idx = at.get_uint64();
        default:
            break;
        }

        if (idx >= N)
        {
            return query;
        }

        try
        {
            if (x_as_array)
            {
                query = x_as_array->at(idx);
            }
            else if (x_as_object)
            {
                auto const kvpair = *(x_as_object->cbegin() + idx);
                query = boost::json::value_from(kvpair);
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
        else if (auto const as_obj = value.if_object())
        {
            if (as_obj->contains(token))
            {
                query = as_obj->at(token);
            }
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
        if (E::asPredicate(*it).eval(sample,context++).as_bool())
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
    auto const filter = E::asPredicate(param);
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
    auto const filter = E::asPredicate(param);

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
    auto F = E::asPredicate(param);
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
        ret = E::asPredicate(*fn++).eval(ret,context++);
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
    std::uint64_t count = boost::json::value_to<std::uint64_t>(E(x).eval());
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
    std::uint64_t count = boost::json::value_to<std::uint64_t>(E(x).eval());
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
        boost::json::object report {
            {"err", e.what()       },
            {"fn" , param          },
            {"x"  , x              },
        };

        if (!context.op.annotation().empty())
        {
            report["op"] = context.op.annotation();
        }
        return report;
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

    for (auto const& item: tokens)
    {
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

void delete_impl(V const& at, V& x)
{
    if(at == "")
    {
        x.emplace_null();
        return;
    }

    auto x_as_array = x.if_array();
    auto x_as_object = x.if_object();

    auto const N = x_as_array ? x_as_array->size() : x_as_object ? x_as_object->size() : 0;


    if (auto const delete_range = at.if_array())
    {
        boost::json::array sorted_delete_range;
        auto not_number = std::find_if(delete_range->cbegin(), delete_range->cend(), [](V const& v){ return not v.is_number(); });
        if (not_number == delete_range->cend())
        {
            sorted_delete_range = *delete_range;
        }
        else
        {
            sorted_delete_range.reserve(delete_range->size());

            for (auto const& el: *delete_range)
            {
                if (el.is_string()) { sorted_delete_range.push_back(el); }
                else if (el.is_number()) sorted_delete_range.push_back(zmbt::format("/%d", el).c_str());
            }
        }

        // reverse alphanumeric sort to handle nested JSON ptrs (array items are deleted from tail to head)
        std::stable_sort(sorted_delete_range.rbegin(), sorted_delete_range.rend(), zmbt::lang::Operator::generic_less);

        for (auto const& el: sorted_delete_range)
        {
            delete_impl(el, x);
        }
    }

    else if (not x.is_structured())
    {
        return;
    }
    else if (at.is_number())
    {
        std::size_t idx{};
        switch (at.kind())
        {
        case boost::json::kind::int64:
        case boost::json::kind::double_:
            {
                std::int64_t at_idx = boost::json::value_to<std::int64_t>(at);
                idx = (at_idx >= 0) ? at_idx : (at_idx + N); // TODO: handle overflow
            }
            break;
        case boost::json::kind::uint64:
            idx = at.get_uint64();
        default:
            break;
        }
        if (idx >= N) return;

        if (x_as_array)
        {
            auto pos = x_as_array->cbegin() + idx;
            x_as_array->erase(pos, pos + 1);
        }
        else if (x_as_object)
        {
            auto pos = x_as_object->cbegin() + idx;
            x_as_object->erase(pos);
        }
    }
    else if (auto const delete_at_string = at.if_string())
    {

        if (delete_at_string->starts_with("/") or delete_at_string->empty())
        {
            auto const last_token_idx = delete_at_string->find_last_of("/");
            auto const node_jp = delete_at_string->subview(0, last_token_idx);
            auto const element_jp = delete_at_string->subview(last_token_idx);

            if (last_token_idx == 0)
            {
                delete_impl(boost::json::parse(element_jp.substr(1)), x);
            }
            else // nested JSON pointer
            {
                boost::json::error_code ec;
                if (auto node_ptr = x.find_pointer(node_jp, ec))
                {
                    delete_impl(element_jp, *node_ptr);
                }
            }
        }
        // TODO: handle slice (or not?)
        else if (x_as_object)
        {
            x_as_object->erase(*delete_at_string);
        }
    }
}

template <>
V eval_impl<Keyword::Delete>(V const& x, V const& param, E::EvalContext const& context)
{
    static_cast<void>(context);
    ASSERT(not param.is_null())
    // TODO: check is x is constant expr
    boost::json::value const at = E(param).eval();
    boost::json::value result = x;

    delete_impl(at, result);
    return result;
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
V eval_impl<Keyword::Fork>(V const& x, V const& param, E::EvalContext const& context)
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


template <>
V eval_impl<Keyword::Overload>(V const& x, V const& param, E::EvalContext const& context)
{
    ASSERT(param.is_array());
    auto const& params = param.get_array();
    ASSERT(params.size() == 2);
    auto const operator_reference = E(params.at(0)).eval({}, context++);
    auto const F = E(params.at(1));
    ASSERT(operator_reference.is_string());
    E::EvalContext ctx = context++;
    ctx.op = O{operator_reference.get_string()};
    auto result = F.eval(x, ctx);

    return (F.is_const() && !F.is_boolean()) ? ctx.op.decorate(result) : result;
}

template <>
V eval_impl<Keyword::Decorate>(V const& x, V const& param, E::EvalContext const& context)
{
    auto const operator_reference = E(param).eval({}, context++);
    ASSERT(operator_reference.is_string());
    auto const op = O{operator_reference.get_string()};
    return op.decorate(x);
}

template <>
V eval_impl<Keyword::Undecorate>(V const& x, V const& param, E::EvalContext const& context)
{
    auto const operator_reference = E(param).eval({}, context++);
    ASSERT(operator_reference.is_string());
    auto const op = O{operator_reference.get_string()};
    return op.undecorate(x);
}

template <>
V eval_impl<Keyword::Debug>(V const& x, V const& param, E::EvalContext const& context)
{
    E::EvalContext local_ctx {};
    local_ctx.op = context.op;
    local_ctx.log = E::EvalLog::make();
    auto const fn_and_id = !param.is_null() ? param.as_array() : boost::json::array{zmbt::json_from(Keyword::Id), "anonymous"};
    zmbt::lang::Expression f(fn_and_id.at(0));
    auto message = zmbt::lang::Expression(fn_and_id.at(1)).eval();
    auto const result = f.eval(x, local_ctx);
    std::cerr << "ZMBT_EXPR_DEBUG(" << message.as_string().c_str() << "):\n";
    std::cerr << local_ctx.log;

    if (context.log.stack)
    {
        // if (!context.log.stack->empty())
        // {
        //     context.log.stack->front() = {context.depth, f, x, result}; // hiding nested debug
        // }
        context.log.stack->reserve(context.log.stack->capacity() + local_ctx.log.stack->size());
        for (auto line_it = std::make_move_iterator(local_ctx.log.stack->begin()),
        log_end = std::make_move_iterator(local_ctx.log.stack->end());
        line_it != log_end; ++line_it)
        {
            line_it->as_array().at(0).as_uint64() += (context.depth + 1);
            context.log.stack->push_back(*line_it);
        }
    }
    return result;
}

template <>
V eval_impl<Keyword::Eval>(V const& x, V const& param, E::EvalContext const& context)
{
    return zmbt::lang::Expression(x).eval(param, context++);
}

} // namespace


namespace zmbt {
namespace lang {


boost::json::value Expression::eval_Special(boost::json::value const& x, EvalContext const& context) const
{
    V maybe_x;
    V maybe_param;
    V const* x_ptr {nullptr};
    V const* param_ptr {nullptr};
    if (lang::detail::isBinary(keyword()))
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

    V const xx = E(*x_ptr).eval();


    V pp = *param_ptr;
    if (lang::detail::isVariadic(keyword()) && pp.is_array())
    {
        for (auto& fn: pp.get_array())
        {
            fn = E(fn).eval(nullptr, context++);
        }
    }
    else
    {
        pp  = E(pp).eval(nullptr, context++);
    }

    switch(keyword())
    {

        // terms special
        case Keyword::At:         return eval_impl<Keyword::At>          (xx, pp, context);
        case Keyword::Delete:     return eval_impl<Keyword::Delete>      (xx, pp, context);
        case Keyword::Approx:     return eval_impl<Keyword::Approx>      (xx, pp, context);
        case Keyword::Re:         return eval_impl<Keyword::Re>          (xx, pp, context);
        case Keyword::Lookup:     return eval_impl<Keyword::Lookup>      (xx, pp, context);
        case Keyword::Card:       return eval_impl<Keyword::Card>        (xx, pp, context);
        case Keyword::Size:       return eval_impl<Keyword::Size>        (xx, pp, context);
        case Keyword::Sum:        return eval_impl<Keyword::Sum>         (xx, pp, context);
        case Keyword::Prod:       return eval_impl<Keyword::Prod>        (xx, pp, context);
        case Keyword::Avg:        return eval_impl<Keyword::Avg>         (xx, pp, context);
        case Keyword::Repeat:     return eval_impl<Keyword::Repeat>      (xx, pp, context);
        case Keyword::Transp:     return eval_impl<Keyword::Transp>      (xx, pp, context);
        case Keyword::Cartesian:  return eval_impl<Keyword::Cartesian>   (xx, pp, context);
        case Keyword::Concat:     return eval_impl<Keyword::Concat>      (xx, pp, context);
        case Keyword::Push:       return eval_impl<Keyword::Push>        (xx, pp, context);
        case Keyword::Uniques:    return eval_impl<Keyword::Uniques>     (xx, pp, context);
        case Keyword::Reverse:    return eval_impl<Keyword::Reverse>     (xx, pp, context);
        case Keyword::Slide:      return eval_impl<Keyword::Slide>       (xx, pp, context);
        case Keyword::Chunks:     return eval_impl<Keyword::Chunks>      (xx, pp, context);
        case Keyword::Stride:     return eval_impl<Keyword::Stride>      (xx, pp, context);
        case Keyword::Arange:     return eval_impl<Keyword::Arange>      (xx, pp, context);
        case Keyword::Items:      return eval_impl<Keyword::Items>       (xx, pp, context);
        case Keyword::Keys:       return eval_impl<Keyword::Keys>        (xx, pp, context);
        case Keyword::Values:     return eval_impl<Keyword::Values>      (xx, pp, context);
        case Keyword::Enumerate:  return eval_impl<Keyword::Enumerate>   (xx, pp, context);
        case Keyword::Flatten:    return eval_impl<Keyword::Flatten>     (xx, pp, context);
        case Keyword::Union:      return eval_impl<Keyword::Union>       (xx, pp, context);
        case Keyword::Intersect:  return eval_impl<Keyword::Intersect>   (xx, pp, context);
        case Keyword::Diff:       return eval_impl<Keyword::Diff>        (xx, pp, context);
        case Keyword::Format:     return eval_impl<Keyword::Format>      (xx, pp, context);
        case Keyword::Decorate:   return eval_impl<Keyword::Decorate>    (xx, pp, context);
        case Keyword::Undecorate: return eval_impl<Keyword::Undecorate>  (xx, pp, context);


        default:
        {
            // TODO: throw
            throw zmbt::expression_not_implemented("%s keyword not implemented", json_from(keyword()));
            return nullptr;
        }
    }
}


boost::json::value Expression::eval_HiOrd(boost::json::value const& x, EvalContext const& context) const
{
    V const* x_ptr {nullptr};
    V const* param_ptr {nullptr};
    if (lang::detail::isBinary(keyword()))
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
        case Keyword::Eval:     return eval_impl<Keyword::Eval>    (*x_ptr, *param_ptr, context);
        case Keyword::Debug:    return eval_impl<Keyword::Debug>   (*x_ptr, *param_ptr, context);
        case Keyword::All:      return eval_impl<Keyword::All>     (*x_ptr, *param_ptr, context);
        case Keyword::Any:      return eval_impl<Keyword::Any>     (*x_ptr, *param_ptr, context);
        case Keyword::Compose:  return eval_impl<Keyword::Compose> (*x_ptr, *param_ptr, context);
        case Keyword::Count:    return eval_impl<Keyword::Count>   (*x_ptr, *param_ptr, context);
        case Keyword::Each:     return eval_impl<Keyword::Each>    (*x_ptr, *param_ptr, context);
        case Keyword::Map:      return eval_impl<Keyword::Map>     (*x_ptr, *param_ptr, context);
        case Keyword::Filter:   return eval_impl<Keyword::Filter>  (*x_ptr, *param_ptr, context);
        case Keyword::Recur:    return eval_impl<Keyword::Recur>   (*x_ptr, *param_ptr, context);
        case Keyword::Unfold:   return eval_impl<Keyword::Unfold>  (*x_ptr, *param_ptr, context);
        case Keyword::Reduce:   return eval_impl<Keyword::Reduce>  (*x_ptr, *param_ptr, context);
        case Keyword::Saturate: return eval_impl<Keyword::Saturate>(*x_ptr, *param_ptr, context);
        case Keyword::Try:      return eval_impl<Keyword::Try>     (*x_ptr, *param_ptr, context);
        case Keyword::TryCatch: return eval_impl<Keyword::TryCatch>(*x_ptr, *param_ptr, context);
        case Keyword::Fork:     return eval_impl<Keyword::Fork>    (*x_ptr, *param_ptr, context);
        case Keyword::Flip:     return eval_impl<Keyword::Flip>    (*x_ptr, *param_ptr, context);
        case Keyword::Sort:     return eval_impl<Keyword::Sort>    (*x_ptr, *param_ptr, context);
        case Keyword::Argmin:   return eval_impl<Keyword::Argmin>  (*x_ptr, *param_ptr, context);
        case Keyword::Argmax:   return eval_impl<Keyword::Argmax>  (*x_ptr, *param_ptr, context);
        case Keyword::Min:      return eval_impl<Keyword::Min>     (*x_ptr, *param_ptr, context);
        case Keyword::Max:      return eval_impl<Keyword::Max>     (*x_ptr, *param_ptr, context);
        case Keyword::Overload: return eval_impl<Keyword::Overload>(*x_ptr, *param_ptr, context);

        default:
        {
            // TODO: throw
            throw zmbt::expression_not_implemented("%s keyword not implemented", json_from(keyword()));
            return nullptr;
        }
    }

}


} // namespace lang
} // namespace zmbt

