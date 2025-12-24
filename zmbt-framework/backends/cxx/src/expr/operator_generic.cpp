/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <sstream>

#include <boost/current_function.hpp>

#include "zmbt/core/aliases.hpp"
#include "zmbt/expr/operator.hpp"

using V = boost::json::value;

#define RETURN_ERROR(msg) return ::zmbt::lang::detail::make_error_expr(msg, BOOST_CURRENT_FUNCTION);

namespace
{
    template <class T, bool check_equal>
    bool compare_as(boost::json::value const& lhs, boost::json::value const& rhs)
    {
        auto const l = boost::json::value_to<T>(lhs);
        auto const r = boost::json::value_to<T>(rhs);
        return check_equal ? (l == r) : (l < r);
    }

    template <bool check_equal>
    bool compare_as_number(boost::json::value const& lhs, boost::json::value const& rhs)
    {
        if (lhs.is_double() or rhs.is_double())
        {
            return compare_as<double, check_equal>(lhs, rhs);
        }
        else if (lhs.is_uint64() or rhs.is_uint64())
        {
            return compare_as<std::uint64_t, check_equal>(lhs, rhs);
        }
        else
        {
            return compare_as<std::int64_t, check_equal>(lhs, rhs);
        }
    }

    bool is_equal_as_number(boost::json::value const& lhs, boost::json::value const& rhs)
    {
        return compare_as_number<true>(lhs, rhs);
    }

    bool is_less_as_number(boost::json::value const& lhs, boost::json::value const& rhs)
    {
        return compare_as_number<false>(lhs, rhs);
    }

    bool is_less_as_array(boost::json::array const& lhs, boost::json::array const& rhs)
    {
        return std::lexicographical_compare(
            lhs.cbegin(),
            lhs.cend(),
            rhs.cbegin(),
            rhs.cend(),
            [](auto const& l, auto const& r) {
                return zmbt::lang::Operator::generic_less(l, r).as_bool();
            }
        );
    }


} // namespace


namespace zmbt {
namespace lang {







V Operator::generic_decorate(LV x)
{
    return x();
}
V Operator::generic_undecorate(LV x)
{
    return x();
}

// this shall never throw or yield error
V Operator::generic_equal_to(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    if (lhs.is_number() and rhs.is_number())
    {
        return is_equal_as_number(lhs, rhs);
    }
    else if (lhs.is_array() and rhs.is_array())
    {
        auto const& larr = lhs.as_array();
        auto const& rarr = rhs.as_array();
        if (larr.size() != rarr.size())
        {
            return false;
        }
        for (size_t i = 0; i < larr.size(); i++)
        {
            if (not generic_equal_to(larr[i], rarr[i]).as_bool())
            {
                return false;
            }
        }
        return true;
    }
    else if (lhs.is_object() and rhs.is_object())
    {
        auto lobj = lhs.as_object();
        auto robj = rhs.as_object();
        if (lobj.size() != lobj.size())
        {
            return false;
        }
        for (auto const& litem: lobj)
        {
            if (not robj.contains(litem.key())){
                return false;
            }
            if (not generic_equal_to(litem.value(), robj.at(litem.key())).as_bool())
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return lhs == rhs;
    }
}

// this shall never throw or yield error
V Operator::generic_less(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    if (lhs.is_null())
    {
        return true;
    }
    else if (rhs.is_null())
    {
        return false;
    }
    else if (lhs.is_bool() and rhs.is_bool())
    {
        return lhs.as_bool() < rhs.as_bool();
    }
    else if (lhs.is_string() and rhs.is_string())
    {
        return lhs.as_string() < rhs.as_string();
    }
    else if (lhs.is_number() and rhs.is_number())
    {
        return is_less_as_number(lhs, rhs);
    }
    else if (lhs.is_array() and rhs.is_array())
    {
        return is_less_as_array(lhs.get_array(), rhs.get_array());
    }
    else
    {
        return false;
    }
}




V Operator::generic_less_equal(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    return not generic_less(rhs, lhs).as_bool();
}

V Operator::generic_left_shift(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    auto const v = boost::json::value_to<std::uint64_t>(lhs);
    auto const shift = boost::json::value_to<std::uint64_t>(rhs);
    return v << shift;
}

V Operator::generic_right_shift(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    auto const v = boost::json::value_to<std::uint64_t>(lhs);
    auto const shift = boost::json::value_to<std::uint64_t>(rhs);
    return v >> shift;
}

V Operator::generic_negate(LV lx)
{
    auto const& x = lx();
    switch (x.kind())
    {
    case boost::json::kind::uint64:
        return -x.get_uint64();
    case boost::json::kind::int64:
        return -x.get_int64();
    case boost::json::kind::double_:
        return -x.get_double();
    case boost::json::kind::null:
        return nullptr;
    default:
        RETURN_ERROR("invalid operand")
    }
}

V Operator::generic_complement(LV lx)
{
    auto const& x = lx();
    switch (x.kind())
    {
    case boost::json::kind::uint64:
        return ~x.get_uint64();
    case boost::json::kind::int64:
        return ~x.get_int64();
    case boost::json::kind::bool_:
        return !x.get_bool();
    case boost::json::kind::null:
        return nullptr;
    default:
        RETURN_ERROR("invalid operand")
    }
}


boost::json::kind common_arithmetic_kind(boost::json::value const& a, boost::json::value const& b)
{
    boost::json::kind result_kind {boost::json::kind::null};
    if (!a.is_number() && !a.is_bool())
    {
        return result_kind;
    }
    if (!b.is_number() && !b.is_bool())
    {
        return result_kind;
    }

    if (a.is_double() || b.is_double())
    {
        result_kind = boost::json::kind::double_;
    }
    else if (a.is_int64() || b.is_int64())
    {
        result_kind = boost::json::kind::int64;
    }
    else if (a.is_bool() || b.is_bool())
    {
        result_kind = boost::json::kind::int64;
    }
    else if (a.is_uint64() && b.is_uint64())
    {
        result_kind = boost::json::kind::uint64;
    }
    return result_kind;
}

#define CASE_DOUBLE case boost::json::kind::double_: return real_to_number(boost::json::value_to<double>(lhs)       X_OP boost::json::value_to<double>(rhs));
#define CASE_INT64  case boost::json::kind::int64:   return real_to_number(boost::json::value_to<std::int64_t>(lhs) X_OP boost::json::value_to<std::int64_t>(rhs));
#define CASE_UINT64 case boost::json::kind::uint64:  return boost::json::value_to<std::size_t>(lhs)  X_OP boost::json::value_to<std::size_t>(rhs);
#define STR(a) #a

#define BINOP_IMPL                              \
if (lhs.is_null() || rhs.is_null())             \
{                                               \
    RETURN_ERROR("null operands")               \
}                                               \
boost::json::kind result_kind =                 \
common_arithmetic_kind(lhs, rhs);               \
if(X_ZERO_DIV &&                                \
    boost::json::value_to<double>(rhs) == 0)    \
{                                               \
    RETURN_ERROR("zero division")               \
}                                               \
switch (result_kind)                            \
{                                               \
X_BINOP_CASES                                   \
default:                                        \
    RETURN_ERROR("invalid operands")            \
}

V Operator::generic_logical_and(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    return (generic_is_truth(lhs).as_bool() ? lrhs() : lhs);
}

V Operator::generic_logical_or(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    return not generic_is_truth(lhs).as_bool() ? lrhs() : lhs;
}

V Operator::generic_plus(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            +
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_minus(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            -
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_multiplies(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            *
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}


V Operator::generic_modulus(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    #define X_OP        %
#define X_ZERO_DIV      true
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_and(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            &
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_or(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            |
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_xor(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
#define X_OP            ^
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

#undef CASE_INT64
#undef CASE_UINT64
#undef CASE_DOUBLE
#undef ZMBT_GSO_BIN_OP

V Operator::generic_divides(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    if (lhs.is_null() || rhs.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(lhs, rhs);

    if (result_kind == boost::json::kind::null)
    {
        RETURN_ERROR("invalid operands")
    }
    if(boost::json::value_to<double>(rhs) == 0)
    {
        RETURN_ERROR("zero division")
    }

    double const result_double = boost::json::value_to<double>(lhs) / boost::json::value_to<double>(rhs);

    switch (result_kind)
    {
    case boost::json::kind::double_:
        return result_double;
    case boost::json::kind::uint64:
    {
        if(0 == boost::json::value_to<std::uint64_t>(lhs) % boost::json::value_to<std::uint64_t>(rhs))
        {
            return boost::json::value_to<std::uint64_t>(lhs) / boost::json::value_to<std::uint64_t>(rhs);
        }
        else
        {
            return result_double;
        }
    }
    case boost::json::kind::int64:
    {
        if(0 == boost::json::value_to<std::int64_t>(lhs) % boost::json::value_to<std::int64_t>(rhs))
        {
            return boost::json::value_to<std::int64_t>(lhs) / boost::json::value_to<std::int64_t>(rhs);
        }
        else
        {
            return result_double;
        }
    }
    default:
        return 0;
    }
}

V Operator::generic_pow(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    boost::json::value ret;
    if (lhs.is_number() && rhs.is_number())
    {
        auto x = dejsonize<double>(lhs);
        auto y = dejsonize<double>(rhs);
        ret = json_from(std::pow(x, y));
    }
    else
    {
        auto x = dejsonize<std::complex<double>>(lhs);
        auto y = dejsonize<std::complex<double>>(rhs);
        ret = json_from(std::pow(x, y));
    }

    if (ret.is_number())
    {
        ret = real_to_number(boost::json::value_to<double>(ret));
    }
    return ret;
}


V Operator::generic_log(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    boost::json::value ret;
    if (lhs.is_number() && rhs.is_number())
    {
        auto x = dejsonize<double>(lhs);
        auto y = dejsonize<double>(rhs);
        ret = json_from(std::log(x) / std::log(y));
    }
    else
    {
        auto x = dejsonize<std::complex<double>>(lhs);
        auto y = dejsonize<std::complex<double>>(rhs);
        ret = json_from(std::log(x) / std::log(y));
    }

    if (ret.is_number())
    {
        ret = real_to_number(boost::json::value_to<double>(ret));
    }
    return ret;

}


V Operator::generic_quot(LV llhs, LV lrhs)
{
    auto const& lhs = llhs();
    auto const& rhs = lrhs();
    if (lhs.is_null() || rhs.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(lhs, rhs);

    if (result_kind == boost::json::kind::null)
    {
        RETURN_ERROR("invalid operands")
    }

    if(boost::json::value_to<double>(rhs) == 0)
    {
        RETURN_ERROR("zero division")
    }

    switch (result_kind)
    {
    case boost::json::kind::double_:
    case boost::json::kind::int64:
        return boost::json::value_to<std::int64_t>(lhs) / boost::json::value_to<std::int64_t>(rhs);
    case boost::json::kind::uint64:
        return boost::json::value_to<std::uint64_t>(lhs) / boost::json::value_to<std::uint64_t>(rhs);
    default:
        return 0;
    }
}


V Operator::generic_logical_not(LV lx)
{
    auto const& x = lx();
    return not generic_is_truth(x).as_bool(); // assume generic_is_truth never throws
}

V Operator::generic_is_truth(LV lx)
{
    auto const& x = lx();
    switch (x.kind())
    {
    case boost::json::kind::null:
        return false;
        break;

    case boost::json::kind::bool_:
        return x.as_bool();
        break;

    case boost::json::kind::string:
        return 0 != x.as_string().size();
        break;

    case boost::json::kind::array:
        return 0 != x.as_array().size();
        break;

    case boost::json::kind::object:
        return 0 != x.as_object().size();
        break;

    case boost::json::kind::int64:
        return 0 != x.as_int64();
        break;

    case boost::json::kind::uint64:
        return 0 != x.as_uint64();
        break;

    case boost::json::kind::double_:
        return 0 != x.as_double();
        break;

    default:
        return false;
        break;
    }
}


} // namespace lang
} // namespace zmbt
