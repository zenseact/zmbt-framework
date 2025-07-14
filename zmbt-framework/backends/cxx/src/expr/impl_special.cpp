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
#include "zmbt/logging.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/exceptions.hpp"

#define ASSERT(cond, msg) if (!(cond)) { return ::zmbt::lang::detail::make_error_expr(msg, keyword_to_str());}
#define MAX_RECURSION_DEPTH INT32_MAX
// #define MAX_RECURSION_DEPTH 32


using V = boost::json::value;
using L = boost::json::array;
using O = zmbt::lang::Operator;
using E = zmbt::lang::Expression;
using Keyword = zmbt::lang::Keyword;

namespace
{

template <Keyword k>
struct eval_impl_base
{
    constexpr static Keyword keyword() { return k; }
    static boost::json::string_view keyword_to_str() { return ::zmbt::lang::keyword_to_str(k); }
};

template <Keyword keyword>
struct eval_impl
{
    V operator()(V const& x, V const& param, E::EvalContext const& context) const;
};

#define EXPR_IMPL(Kw) \
template <> struct eval_impl<Keyword::Kw> : eval_impl_base<Keyword::Kw> \
{ V operator()(V const& x, V const& param, E::EvalContext const& context) const; }; \
V eval_impl<Keyword::Kw>::operator()(V const& x, V const& param, E::EvalContext const& context) const

template <Keyword keyword>
struct eval_variadic_impl
{
    V operator()(V const& x, std::list<E> const& subexpressions, E::EvalContext const& context) const;
};

#define EXPR_VARIADIC_IMPL(Kw) \
template <> struct eval_variadic_impl<Keyword::Kw> : eval_impl_base<Keyword::Kw> \
{ V operator()(V const& x, std::list<E> const&, E::EvalContext const& context) const; }; \
V eval_variadic_impl<Keyword::Kw>::operator()(V const& x, std::list<E> const& subexpressions, E::EvalContext const& context) const

EXPR_VARIADIC_IMPL(All)
{
    for (auto const& e: subexpressions)
    {
        if (not E::asPredicate(e).eval(x,context++).as_bool())
        {
            return false;
        }
    }
    return true;
}


EXPR_VARIADIC_IMPL(Any)
{
    for (auto const& e: subexpressions)
    {
        if (E::asPredicate(e).eval(x,context++).as_bool())
        {
            return true;
        }
    }
    return false;
}


EXPR_IMPL(Re)
{
    static_cast<void>(context);
    ASSERT(param.is_string(), "invalid parameter");
    ASSERT(x.is_string(), "invalid argument");

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
    return query;
}


EXPR_IMPL(At)
{
    static_cast<void>(context);
    ASSERT(not param.is_null(), "null parameter")
    boost::json::value const q = E(x).eval();
    return query_at(q, param);
}

EXPR_IMPL(Lookup)
{
    static_cast<void>(context);
    ASSERT(not x.is_null(), "null argument")
    return query_at(param, x);
}

EXPR_VARIADIC_IMPL(Saturate)
{
    ASSERT(x.is_array(), "invalid argument");

    auto const& samples = x.as_array();

    auto it = subexpressions.cbegin();
    for (auto const& sample: samples)
    {
        if (it == subexpressions.cend())
        {
            break;
        }
        if (E::asPredicate(*it).eval(sample,context++).as_bool())
        {
            it++;
        }
    }
    return it == subexpressions.cend();
}

EXPR_IMPL(Count)
{
    ASSERT(x.is_array() || x.is_object(), "invalid argument")
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

EXPR_IMPL(Each)
{
    ASSERT(x.is_array() || x.is_object(), "invalid argument")
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

EXPR_IMPL(Near)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))
    // default rtol=1e-05, atol=1e-08
    constexpr double default_rtol = 1e-05;
    constexpr double default_atol = 1e-08;

    ASSERT(x.is_number(), "invalid argument")
    ASSERT(param.is_array() || param.is_number(), "invalid parameter")

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
        ASSERT(params.size() >= 1 && params.size() <= 3, "invalid parameter")

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

EXPR_IMPL(Uniques)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument")
    return boost::json::value_from(
        boost::json::value_to<std::unordered_set<V>>(x)
    );
}


EXPR_IMPL(Size)
{
    static_cast<void>(context);
    ASSERT(param.is_null(), "invalid parameter")
    ASSERT(x.is_structured(), "invalid argument")
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


EXPR_IMPL(Card)
{
    static_cast<void>(context);
    ASSERT(param.is_null(), "invalid parameter")
    ASSERT(x.is_structured(), "invalid argument")

    if (x.is_array())
    {
        return eval_impl<Keyword::Uniques>()(x, {}, {}).as_array().size();
    }
    else
    {
        return x.get_object().size();
    }
}


EXPR_IMPL(Fold)
{
    ASSERT(x.is_array(), "invalid argument");
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

EXPR_IMPL(Map)
{
    ASSERT(x.is_array(), "invalid argument");
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


EXPR_IMPL(Filter)
{
    ASSERT(param.is_object(), "invalid parameter");
    ASSERT(x.is_array(), "invalid argument");
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


EXPR_VARIADIC_IMPL(Pipe)
{
    ASSERT(subexpressions.size() > 0, "invalid parameter (empty array)");

    auto fn = subexpressions.cbegin();

    // first el eval as is
    boost::json::value ret {E(*fn++).eval(x,context++)};

    while (fn != subexpressions.cend())
    {
        // any consequent literals eval as eq
        ret = E::asPredicate(*fn++).eval(ret,context++);
    }
    return ret;
}

EXPR_IMPL(Repeat)
{
    static_cast<void>(context);
    ASSERT(param.is_number(), "invalid parameter");
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


EXPR_IMPL(Recur)
{
    auto const fork_params = E(param).parameter_list();
    ASSERT(fork_params.size() == 2, "invalid parameters, expected Fn & initial");
    auto const initial = fork_params.front().eval();
    auto const F = fork_params.back();
    std::uint64_t max_recursion_depth = MAX_RECURSION_DEPTH;

    auto cond = E(x);
    if (cond.is_literal())
    {
        auto const maybe_depth = boost::json::try_value_to<std::uint64_t>(cond.eval());
        ASSERT(maybe_depth.has_value(), "invalid parameter")
        max_recursion_depth = maybe_depth.value();
        cond = E(false);
    }

    boost::json::value result = initial;

    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {
        auto const next = F.eval(result,context++);
        auto const maybe_exit = cond.eval(next);
        if ((maybe_exit.is_bool() && maybe_exit.get_bool()) || E(next).is_error())
        {
            break;
        }
        result = next;
    }
    return result;
}

EXPR_IMPL(Unfold)
{
    auto const fork_params = E(param).parameter_list();
    ASSERT(fork_params.size() == 2, "invalid parameters, expected Fn & initial");
    auto const initial = fork_params.front().eval();
    auto const F = fork_params.back();
    std::uint64_t max_recursion_depth = MAX_RECURSION_DEPTH;

    auto cond = E(x);
    if (cond.is_literal())
    {
        auto const maybe_depth = boost::json::try_value_to<std::uint64_t>(cond.eval());
        ASSERT(maybe_depth.has_value(), "invalid parameter")
        max_recursion_depth = maybe_depth.value();
        cond = E(false);
    }

    boost::json::array result {};
    result.push_back(initial);
    for (std::uint64_t i = 0; i < max_recursion_depth; i++)
    {
        auto const next = F.eval(result.back(),context++);
        auto const maybe_exit = cond.eval(next);
        if ((maybe_exit.is_bool() && maybe_exit.get_bool()) || E(next).is_error())
        {
            break;
        }
        result.push_back(next);
    }
    return result;
}

EXPR_IMPL(Transp)
{
    static_cast<void>(context);
    ASSERT(param.is_null(), "invalid parameter");
    ASSERT(x.is_array(), "invalid argument");
    auto const& arr = x.get_array();
    if (arr.empty()) return arr;
    auto const N = arr.at(0).as_array().size();
    ASSERT(
        std::all_of(arr.cbegin(), arr.cend(), [N](auto const& el) -> bool { return el.is_array() && el.as_array().size() == N;})
        , "invalid argument"
    );

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


EXPR_IMPL(Cartesian)
{
    static_cast<void>(context);
    ASSERT(param.is_null(), "invalid parameter");
    ASSERT(x.is_array(), "invalid argument");
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
    }), "invalid argument"
    );

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

EXPR_IMPL(Try)
{
    auto const fn = E(param);
    auto const result = fn.eval(x,context++);
    if (E(result).is_error()) return nullptr;
    return result;
}

EXPR_IMPL(Cat)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind(), "invalid argument");
    ASSERT(x.is_string() || x.is_array(), "invalid argument");

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

EXPR_IMPL(Push)
{
    static_cast<void>(context);
    ASSERT(x.is_string() || x.is_array(), "invalid argument");

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

EXPR_VARIADIC_IMPL(Fmt)
{
    static_cast<void>(context);
    V fmtstr;
    boost::format fmt;
    zmbt::remove_cvref_t<decltype(subexpressions)> args;

    auto const if_arr = x.if_array();
    auto const if_str = x.if_string();

    if (if_str)
    {
       fmt = boost::format{if_str->c_str()};
       args = subexpressions;
    }
    else if (if_arr && subexpressions.empty())
    {
        ASSERT(if_arr->size() == 2, "invalid arity");

        auto const if_lhs_str = if_arr->front().if_string();
        auto const if_rhs_args = if_arr->back().if_array();

        ASSERT(if_lhs_str && if_rhs_args, "invalid operands");

        fmt = boost::format{if_lhs_str->c_str()};
        for (auto const& arg: *if_rhs_args)
        {
            args.push_back(E(arg));
        }
    }
    else
    {
        ASSERT(false, "invalid parameters");
    }

    // ASSERT(x.is_string(), "invalid argument");
    if (args.empty()) return x;

    auto const N = static_cast<std::size_t>(fmt.expected_args());
    ASSERT(args.size() == N, "invalid formatting");

    for (auto const& fn: args)
    {
        auto const item = fn.eval({}, context++);
        fmt = item.is_string() ? (fmt % item.get_string().c_str()) : (fmt % item);
    }
    return {fmt.str()};
}


EXPR_IMPL(Sort)
{
    ASSERT(x.is_array(), "invalid argument");
    auto const key_fn = param.is_null() ? E(Keyword::Id) : E(param);
    auto const& op = context.op;
    std::function<bool(V const&, V const&)> is_less = [&key_fn, &op](V const& lhs, V const& rhs) ->bool {
        return op.apply(Keyword::Lt, key_fn.eval(lhs), key_fn.eval(rhs)).as_bool();
    };
    boost::json::array out = x.get_array();
    std::stable_sort(out.begin(), out.end(), is_less);
    return out;
}

EXPR_IMPL(Reverse)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument");
    boost::json::array out = x.get_array();
    std::reverse(out.begin(), out.end());
    return out;
}

void deleteval_impl(V const& at, V& x)
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
        std::stable_sort(
            sorted_delete_range.rbegin(),
            sorted_delete_range.rend(),
            [](auto const& a, auto const& b){
                return zmbt::lang::Operator::generic_less(a, b).as_bool();
            }
        );

        for (auto const& el: sorted_delete_range)
        {
            deleteval_impl(el, x);
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
                deleteval_impl(boost::json::parse(element_jp.substr(1)), x);
            }
            else // nested JSON pointer
            {
                boost::json::error_code ec;
                if (auto node_ptr = x.find_pointer(node_jp, ec))
                {
                    deleteval_impl(element_jp, *node_ptr);
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

EXPR_IMPL(Del)
{
    static_cast<void>(context);
    ASSERT(not param.is_null(), "invalid parameter")
    // TODO: check is x is constant expr
    boost::json::value const at = E(param).eval();
    boost::json::value result = x;

    deleteval_impl(at, result);
    return result;
}

EXPR_IMPL(Slide)
{
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument");
    ASSERT(param.is_number(), "invalid parameter");

    auto const W = boost::json::value_to<std::int64_t>(param);
    ASSERT(W > 0, "invalid parameter");

    auto const& samples = x.get_array();
    boost::json::array out {};
    if (samples.empty() || (0 == W))
    {
        return out;
    }
    if (samples.size() <= static_cast<std::size_t>(W))
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

EXPR_IMPL(Chunks)
{
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument");
    ASSERT(param.is_number(), "invalid parameter");
    auto const W = boost::json::value_to<std::uint64_t>(param);
    ASSERT(W > 0, "invalid parameter");
    auto const& samples = x.get_array();
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

EXPR_IMPL(Stride)
{
    auto result = eval_impl<Keyword::Chunks>()(x, param, context).as_array();

    if (!result.empty() && result.back().as_array().size() != param)
    {
        result.pop_back();
    }
    return result;
}


EXPR_IMPL(Sum)
{
    static_cast<void>(param);
    ASSERT(x.is_array(), "invalid argument");
    return eval_impl<Keyword::Fold>()(x, E(Keyword::Add),context++);
}

EXPR_IMPL(Prod)
{
    static_cast<void>(param);
    ASSERT(x.is_array(), "invalid argument");
    return eval_impl<Keyword::Fold>()(x, E(Keyword::Mul),context++);
}

EXPR_IMPL(Avg)
{
    static_cast<void>(param);
    ASSERT(x.is_array(), "invalid argument");
    auto const N = eval_impl<Keyword::Size>()(x, {}, {});
    auto const sum = eval_impl<Keyword::Fold>()(x, E(Keyword::Add), {});
    return context.op.apply(Keyword::Div, sum, N);
}

EXPR_VARIADIC_IMPL(Fork)
{
    static_cast<void>(context);
    // ASSERT(param.is_array(), "invalid parameter");
    // auto const& funcs = param.get_array();
    boost::json::array out {};
    out.reserve(subexpressions.size());
    for (auto const& fn: subexpressions)
    {
        out.push_back(fn.eval(x,context++));
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

EXPR_IMPL(Argmin)
{
    ASSERT(x.is_array(), "invalid argument");
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, param, context.op, &std::min_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

EXPR_IMPL(Argmax)
{
    ASSERT(x.is_array(), "invalid argument");
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return find_argminmax(samples, param, context.op, &std::max_element<L::const_iterator, compare_fn_t>) - samples.cbegin();
}

EXPR_IMPL(Min)
{
    ASSERT(x.is_array(), "invalid argument");
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, param, context.op, &std::min_element<L::const_iterator, compare_fn_t>);
}

EXPR_IMPL(Max)
{
    ASSERT(x.is_array(), "invalid argument");
    auto const& samples = x.get_array();
    if (samples.empty()) return nullptr;
    return *find_argminmax(samples, param, context.op, &std::max_element<L::const_iterator, compare_fn_t>);
}


EXPR_IMPL(Intersect)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind(), "invalid operands");
    ASSERT(x.is_array(), "invalid argument");

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

EXPR_IMPL(Diff)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind(), "invalid operands");
    ASSERT(x.is_structured(), "invalid argument");

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


EXPR_IMPL(Union)
{
    static_cast<void>(context);
    ASSERT(x.kind() == param.kind(), "invalid operands");
    ASSERT(x.is_structured(), "invalid argument");



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

EXPR_IMPL(Arange)
{
    static_cast<void>(context);
    static_cast<void>(param);
    ASSERT(x.is_number() || x.is_array() || x.is_string(), "invalid argument");

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
        ASSERT(params.size() >= 1 && params.size() <= 3, "invalid argument");
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

    ASSERT(step != 0, "invalid parameter")

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

EXPR_IMPL(Items)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back({kv.key(), kv.value()});
    }
    return out;
}

EXPR_IMPL(Keys)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.key());
    }
    return out;
}

EXPR_IMPL(Values)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_object(), "invalid argument");
    auto const& obj = x.get_object();
    boost::json::array out {};
    out.reserve(obj.size());
    for (auto const& kv: obj)
    {
        out.push_back(kv.value());
    }
    return out;
}

EXPR_IMPL(Enumerate)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument");
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

EXPR_IMPL(Flatten)
{
    static_cast<void>(param);
    static_cast<void>(context);
    ASSERT(x.is_array(), "invalid argument");
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



EXPR_IMPL(Flip)
{
    auto const f = E(param);
    auto const flip = E(E::encodeNested(f.keyword(), {x}));
    auto const pl = f.parameter_list();
    ASSERT(!pl.empty(), "invalid parameter");
    return flip.eval(f.parameter_list().front(), context++);
}


EXPR_IMPL(Op)
{
    auto const pl = E(param).parameter_list();

    // ASSERT(param.is_array(), "invalid parameter");
    // auto const& params = param.get_array();
    ASSERT(pl.size() == 2, "invalid parameter");
    auto const operator_reference = pl.front().eval({}, context++);
    auto const& F = pl.back();
    ASSERT(operator_reference.is_string(), "invalid parameter");
    E::EvalContext ctx = context++;
    ctx.op = O{operator_reference.get_string()};
    auto result = F.eval(x, ctx);

    return (F.is_const() && !F.is_boolean()) ? ctx.op.decorate(result) : result;
}

EXPR_IMPL(Cast)
{
    auto const operator_reference = E(param).eval({}, context++);
    ASSERT(operator_reference.is_string(), "invalid parameter");
    auto const op = O{operator_reference.get_string()};
    return op.decorate(x);
}

EXPR_IMPL(Uncast)
{
    auto const operator_reference = E(param).eval({}, context++);
    ASSERT(operator_reference.is_string(), "invalid parameter");
    auto const op = O{operator_reference.get_string()};
    return op.undecorate(x);
}

EXPR_IMPL(Dbg)
{
    E::EvalContext local_ctx {};
    local_ctx.op = context.op;
    local_ctx.log = E::EvalLog::make();

    zmbt::lang::Expression f(param);
    auto const result = f.eval(x, local_ctx);

    ZMBT_LOG_JSON(INFO).WithSrcLoc("ZMBT_EXPR_DEBUG") << *local_ctx.log.stack;
    ZMBT_LOG_CERR(DEBUG).WithSrcLoc("ZMBT_EXPR_DEBUG") << "\n" << local_ctx.log.str(2);

    if (context.log.stack)
    {
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

EXPR_IMPL(Trace)
{
    static_cast<void>(context);
    ZMBT_LOG_JSON(INFO).WithSrcLoc("ZMBT_EXPR_TRACE") << param;
    ZMBT_LOG_CERR(DEBUG).WithSrcLoc("ZMBT_EXPR_TRACE") << param;
    return x;
}

EXPR_IMPL(Eval)
{
    return zmbt::lang::Expression(x).eval(param, context++);
}


EXPR_IMPL(Kwrd)
{
    static_cast<void>(param);
    static_cast<void>(context);
    return zmbt::lang::Expression(x).keyword_to_str();
}

EXPR_IMPL(Prms)
{
    static_cast<void>(param);
    static_cast<void>(context);
    auto enc = E(x).encoding();
    enc.keywords.front() = Keyword::Fork;
    return E(enc);
}

EXPR_IMPL(Bind)
{
    static_cast<void>(context);
    auto enc = zmbt::lang::Expression(param).encoding();
    ASSERT(enc.keywords.size() == 1, "function has parameters")

    if (zmbt::lang::detail::isVariadic(enc.keywords.front()))
    {
        auto const if_arr = x.if_array();
        ASSERT(if_arr, "invalid argument")
        std::vector<E> xx;
        xx.reserve(if_arr->size());
        for (auto const& item: *if_arr)
        {
            xx.emplace_back(E(item));
        }
        return E(E::encodeNested(enc.keywords.front(), xx));

    }
    else
    {
        return E(E::encodeNested(enc.keywords.front(), {E(x)}));
    }
}

EXPR_IMPL(PreProc)
{
    static_cast<void>(x);
    static_cast<void>(context);
    return param;
}

} // namespace


namespace zmbt {
namespace lang {


boost::json::value Expression::eval_Special(boost::json::value const& x, EvalContext const& context) const
try
{
    V lhs {}; // binary LHS or expr params
    V rhs {}; // binary RHS or expr arg
    if (lang::detail::isBinary(keyword()))
    {
        handle_binary_args(x, lhs, rhs);
    }
    else
    {
        lhs = x;
        rhs = subexpr().to_json();
    }

    V const xx = E(lhs).eval(nullptr, context++);
    V pp = E(rhs).eval(nullptr, context++);

    switch(keyword())
    {

        // terms special
        case Keyword::At:         return eval_impl<Keyword::At>()          (xx, pp, context);
        case Keyword::Del:        return eval_impl<Keyword::Del>()         (xx, pp, context);
        case Keyword::Near:       return eval_impl<Keyword::Near>()        (xx, pp, context);
        case Keyword::Re:         return eval_impl<Keyword::Re>()          (xx, pp, context);
        case Keyword::Lookup:     return eval_impl<Keyword::Lookup>()      (xx, pp, context);
        case Keyword::Card:       return eval_impl<Keyword::Card>()        (xx, pp, context);
        case Keyword::Size:       return eval_impl<Keyword::Size>()        (xx, pp, context);
        case Keyword::Sum:        return eval_impl<Keyword::Sum>()         (xx, pp, context);
        case Keyword::Prod:       return eval_impl<Keyword::Prod>()        (xx, pp, context);
        case Keyword::Avg:        return eval_impl<Keyword::Avg>()         (xx, pp, context);
        case Keyword::Repeat:     return eval_impl<Keyword::Repeat>()      (xx, pp, context);
        case Keyword::Transp:     return eval_impl<Keyword::Transp>()      (xx, pp, context);
        case Keyword::Cartesian:  return eval_impl<Keyword::Cartesian>()   (xx, pp, context);
        case Keyword::Cat:        return eval_impl<Keyword::Cat>()         (xx, pp, context);
        case Keyword::Push:       return eval_impl<Keyword::Push>()        (xx, pp, context);
        case Keyword::Uniques:    return eval_impl<Keyword::Uniques>()     (xx, pp, context);
        case Keyword::Reverse:    return eval_impl<Keyword::Reverse>()     (xx, pp, context);
        case Keyword::Slide:      return eval_impl<Keyword::Slide>()       (xx, pp, context);
        case Keyword::Chunks:     return eval_impl<Keyword::Chunks>()      (xx, pp, context);
        case Keyword::Stride:     return eval_impl<Keyword::Stride>()      (xx, pp, context);
        case Keyword::Arange:     return eval_impl<Keyword::Arange>()      (xx, pp, context);
        case Keyword::Items:      return eval_impl<Keyword::Items>()       (xx, pp, context);
        case Keyword::Keys:       return eval_impl<Keyword::Keys>()        (xx, pp, context);
        case Keyword::Values:     return eval_impl<Keyword::Values>()      (xx, pp, context);
        case Keyword::Enumerate:  return eval_impl<Keyword::Enumerate>()   (xx, pp, context);
        case Keyword::Flatten:    return eval_impl<Keyword::Flatten>()     (xx, pp, context);
        case Keyword::Union:      return eval_impl<Keyword::Union>()       (xx, pp, context);
        case Keyword::Intersect:  return eval_impl<Keyword::Intersect>()   (xx, pp, context);
        case Keyword::Diff:       return eval_impl<Keyword::Diff>()        (xx, pp, context);
        case Keyword::Cast:       return eval_impl<Keyword::Cast>()        (xx, pp, context);
        case Keyword::Uncast:     return eval_impl<Keyword::Uncast>()      (xx, pp, context);
        case Keyword::Trace:      return eval_impl<Keyword::Trace>()       (xx, pp, context);



        default:
        {
            return detail::make_error_expr("not implemented", json_from(keyword()).as_string());
        }
    }
}
catch(const std::exception& e)
{
    return detail::make_error_expr(e.what(), prettify());
}


boost::json::value Expression::eval_Variadic(boost::json::value const& x, EvalContext const& context) const
try
{
    switch (keyword())
    {
    case Keyword::Pipe:     return eval_variadic_impl<Keyword::Pipe>()    (x, parameter_list(), context);
    case Keyword::Fork:     return eval_variadic_impl<Keyword::Fork>()    (x, parameter_list(), context);
    case Keyword::Fmt:      return eval_variadic_impl<Keyword::Fmt>()     (x, parameter_list(), context);
    case Keyword::All:      return eval_variadic_impl<Keyword::All>()     (x, parameter_list(), context);
    case Keyword::Any:      return eval_variadic_impl<Keyword::Any>()     (x, parameter_list(), context);
    case Keyword::Saturate: return eval_variadic_impl<Keyword::Saturate>()(x, parameter_list(), context);

    default:
        {
            return detail::make_error_expr("not implemented", json_from(keyword()).as_string());
        }
    }
}
catch(const std::exception& e)
{
    return detail::make_error_expr(e.what(), keyword_to_str());
}


boost::json::value Expression::eval_HiOrd(boost::json::value const& x, EvalContext const& context) const
try
{
    // V const* x_ptr {nullptr};
    // V const* param_ptr {nullptr};
    // if (lang::detail::isBinary(keyword()))
    // {
    //     handle_binary_args(x, x_ptr, param_ptr);
    // }
    // else
    // {
    //     x_ptr = &x;
    //     param_ptr = &data();
    //     // param_ptr = &subexpr();
        
    // }
    // ASSERT(x_ptr, "invalid argument")
    // ASSERT(param_ptr, "invalid parameter")

    V lhs {}; // binary LHS or expr params
    V rhs {}; // binary RHS or expr arg
    if (lang::detail::isBinary(keyword()))
    {
        handle_binary_args(x, lhs, rhs);
    }
    else
    {
        lhs = x;
        rhs = subexpr().to_json();
    }

    switch(keyword())
    {
        case Keyword::Eval:     return eval_impl<Keyword::Eval>()    (lhs, rhs, context);
        case Keyword::Dbg:      return eval_impl<Keyword::Dbg>()     (lhs, rhs, context);
        case Keyword::Count:    return eval_impl<Keyword::Count>()   (lhs, rhs, context);
        case Keyword::Each:     return eval_impl<Keyword::Each>()    (lhs, rhs, context);
        case Keyword::Map:      return eval_impl<Keyword::Map>()     (lhs, rhs, context);
        case Keyword::Filter:   return eval_impl<Keyword::Filter>()  (lhs, rhs, context);
        case Keyword::Recur:    return eval_impl<Keyword::Recur>()   (lhs, rhs, context);
        case Keyword::Unfold:   return eval_impl<Keyword::Unfold>()  (lhs, rhs, context);
        case Keyword::Fold:     return eval_impl<Keyword::Fold>()    (lhs, rhs, context);
        case Keyword::Try:      return eval_impl<Keyword::Try>()     (lhs, rhs, context);
        case Keyword::Flip:     return eval_impl<Keyword::Flip>()    (lhs, rhs, context);
        case Keyword::Sort:     return eval_impl<Keyword::Sort>()    (lhs, rhs, context);
        case Keyword::Argmin:   return eval_impl<Keyword::Argmin>()  (lhs, rhs, context);
        case Keyword::Argmax:   return eval_impl<Keyword::Argmax>()  (lhs, rhs, context);
        case Keyword::Min:      return eval_impl<Keyword::Min>()     (lhs, rhs, context);
        case Keyword::Max:      return eval_impl<Keyword::Max>()     (lhs, rhs, context);
        case Keyword::Op:       return eval_impl<Keyword::Op>()      (lhs, rhs, context);
        case Keyword::Kwrd:     return eval_impl<Keyword::Kwrd>()    (lhs, rhs, context);
        case Keyword::Prms:     return eval_impl<Keyword::Prms>()    (lhs, rhs, context);
        case Keyword::Bind:     return eval_impl<Keyword::Bind>()    (lhs, rhs, context);

        default:
        {
            ASSERT(false, "not implemented")
        }
    }

}
catch(const std::exception& e)
{
    return detail::make_error_expr(e.what(), keyword_to_str());
}


} // namespace lang
} // namespace zmbt

