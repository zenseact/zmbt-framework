/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/logging.hpp"
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
using Keyword = zmbt::Keyword;

template <Keyword keyword>
V eval_impl(V const& params, V const& x, O const& op);



template <>
V eval_impl<Keyword::Not>(V const& param, V const& x, O const& op)
{
    if (param.is_null())
    {
        return !op.is_truth(x);
    }
    else
    {
        return !op.is_truth(E(param).eval(x));
    }
}


template <>
V eval_impl<Keyword::And>(V const& param, V const& x, O const& op)
{
    if (param.is_null())
    {
        ASSERT(x.is_array());
        auto const& terms = x.get_array();
        if(terms.empty())
        {
            return nullptr;
        }

        auto term = terms.cbegin();
        auto ret = term;
        while (term != terms.cend())
        {
            if (not op.is_truth(*term))
            {
                return nullptr;
            }
            ret = term++;
        }
        return *ret;
    }
    else
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
}
template <>
V eval_impl<Keyword::Or>(V const& param, V const& x, O const& op)
{
    if (param.is_null())
    {
        ASSERT(x.is_array());
        auto const& terms = x.get_array();
        if(terms.empty())
        {
            return nullptr;
        }

        auto term = terms.cbegin();
        while (term != terms.cend())
        {
            if (op.is_truth(*term))
            {
                return *term;
            }
            term++;
        }
        return nullptr;
    }
    else
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
}
template <>
V eval_impl<Keyword::Xor>(V const& param, V const& x, O const& op)
{
    bool L, R;

    if (param.is_null())
    {
        ASSERT(x.is_array() and (x.get_array().size() == 2));
        auto const& lhs = x.get_array().at(0);
        auto const& rhs = x.get_array().at(1);

        L = op.is_truth(lhs);
        R = op.is_truth(rhs);
    }
    else
    {
        ASSERT(param.is_array() and (param.get_array().size() == 2));
        auto const& predicates = param.get_array();
        auto const p1 = E(predicates.at(0));
        auto const p2 = E(predicates.at(1));

        L = op.is_truth(p1.eval(x));
        R = op.is_truth(p2.eval(x));
    }
    return (L || R) && !(L && R);
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
V eval_impl<Keyword::At>(V const& params, V const& sample, O const& op)
{
    ASSERT(params.is_array() and not params.get_array().empty())

    auto const& q = params.get_array().at(0);
    boost::json::value query = query_at(sample, q);

    switch (params.get_array().size())
    {
    case 1:
        return query;
    case 2: // second param is optional transform
    {
        auto const e = E(params.as_array().at(1));
        return e.eval(query, op);
    }
    default:
        ASSERT(false);
        return nullptr;
    }
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
    if (!samples.is_array())
    {
        return false;
    }
    ASSERT(params.is_array() and not params.get_array().empty())

    auto const& filter_and_maybe_transform = params.get_array();
    auto const& sample_list = samples.get_array();

    auto const filter = E(filter_and_maybe_transform.at(0));
    std::size_t count {0};
    for (auto const& sample: sample_list)
    {
        if (filter.match(sample, op))
        {
            count++;
        }
    }
    switch (filter_and_maybe_transform.size())
    {
    case 1:
        return count;
    case 2:
    {
        auto const transform = E(filter_and_maybe_transform.at(1));
        return transform.eval(count); // count is number, no operator applied
    }
    default:
        ASSERT(false);
        return nullptr;
    }
}

template <>
V eval_impl<Keyword::Approx>(V const& params, V const& sample, O const&)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))

    // TODO: handle invalid expr
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
    std::size_t size {0};
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
        ZMBT_LOG_JSON(warning) << "invalid Size application";
        return nullptr;
    }

    if (params.is_null()) // optional transform
    {
        return size;
    }
    else
    {
        ASSERT(params.is_array())
        return E(params.get_array().at(0)).eval(size);
    }
}


template <>
V eval_impl<Keyword::Card>(V const& params, V const& sample, O const&)
{
    std::size_t card {0};
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
        ZMBT_LOG_JSON(warning) << "invalid Card application";
        return nullptr;
    }

    if (params.is_null()) // optional transform
    {
        return card;
    }
    else
    {
        ASSERT(params.is_array())
        return E(params.get_array().at(0)).eval(card);
    }
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



void handle_terminal_binary_args(bool const has_params, V const& params, V const& x, V const*& lhs, V const*& rhs)
{
    if (has_params)
    {
        lhs = &x;
        rhs = &params;
    }
    else // treat x as argument pair
    {
        if (x.is_array() && x.get_array().size() == 2)
        {
            lhs = x.get_array().cbegin();
            rhs = lhs + 1;
        }
        else
        {
            throw zmbt::expression_error("got invalid params for binary operator, should be pair");
        }
    }
}

} // namespace


boost::json::value zmbt::Expression::eval(boost::json::value const& x, SignalOperatorHandler const& op) const
{
    switch(keyword())
    {
        case Keyword::Id:    return x;
        case Keyword::True:  return true;
        case Keyword::False: return false;
        case Keyword::Null:  return nullptr;
        case Keyword::Noop:  return true;

        case Keyword::Neg:
        case Keyword::BitNot:
        case Keyword::Bool:
        case Keyword::Nil:
            return op.apply(keyword(), x, nullptr);

        case Keyword::Eq:
        case Keyword::Ne:
        case Keyword::Lt:
        case Keyword::Gt:
        case Keyword::Le:
        case Keyword::Ge:
        case Keyword::Subset:
        case Keyword::Superset:
        case Keyword::ProperSubset:
        case Keyword::ProperSuperset:
        case Keyword::In:
        case Keyword::NotIn:
        case Keyword::Ni:
        case Keyword::NotNi:
        case Keyword::SetEq:

        case Keyword::Add:
        case Keyword::Sub:
        case Keyword::Mul:
        case Keyword::Div:
        case Keyword::Mod:
        case Keyword::BitAnd:
        case Keyword::BitOr:
        case Keyword::BitXor:
        case Keyword::Pow:
        // case Keyword::Quot:
        // case Keyword::Log:
        {
            V const* lhs {nullptr}; // binary LHS or functor params
            V const* rhs {nullptr}; // binary RHS or functor arg
            handle_terminal_binary_args(has_params(), params(), x, lhs, rhs);
            ASSERT(lhs)
            ASSERT(rhs)
            return op.apply(keyword(), *lhs, *rhs);
        }


    #define ZMBT_EXPR_EVAL_IMPL_CASE(K) case Keyword::K: return eval_impl<Keyword::K>(params(), x, op);

        ZMBT_EXPR_EVAL_IMPL_CASE(Approx)
        ZMBT_EXPR_EVAL_IMPL_CASE(Not)
        ZMBT_EXPR_EVAL_IMPL_CASE(And)
        ZMBT_EXPR_EVAL_IMPL_CASE(Or)
        ZMBT_EXPR_EVAL_IMPL_CASE(Xor)

        ZMBT_EXPR_EVAL_IMPL_CASE(Card)
        ZMBT_EXPR_EVAL_IMPL_CASE(Size)
        ZMBT_EXPR_EVAL_IMPL_CASE(Re)
        ZMBT_EXPR_EVAL_IMPL_CASE(At)
        ZMBT_EXPR_EVAL_IMPL_CASE(Count)
        ZMBT_EXPR_EVAL_IMPL_CASE(Saturate)

        ZMBT_EXPR_EVAL_IMPL_CASE(Reduce)
        ZMBT_EXPR_EVAL_IMPL_CASE(Map)
        ZMBT_EXPR_EVAL_IMPL_CASE(Filter)
        ZMBT_EXPR_EVAL_IMPL_CASE(Recur)
        ZMBT_EXPR_EVAL_IMPL_CASE(Repeat)
        ZMBT_EXPR_EVAL_IMPL_CASE(Compose)


        default:
        {
            // TODO: throw
            throw zmbt::expression_error("%s keyword not implemented", json_from(keyword()));
            return nullptr;
        }
    }
    #undef ZMBT_EXPR_EVAL_IMPL_CASE
}
