/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/exceptions.hpp"



#include <zmbt/core/aliases.hpp>
#include <zmbt/core/exceptions.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <sstream>

using V = boost::json::value;
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
                return zmbt::dsl::Operator::generic_less(l, r);
            }
        );
    }

    bool is_less_as_object(boost::json::object const&, boost::json::object const&)
    {
        // NOTE: this logic may be implemented in future, as js object preserves order of kv pairs
        throw zmbt::expression_error("order relation is undefined for JSON objects");
    }

} // namespace


namespace zmbt {
namespace dsl {







V Operator::generic_decorate(V const& x)
{
    return x;
}
V Operator::generic_undecorate(V const& x)
{
    return x;
}


bool Operator::generic_equal_to(V const& lhs, V const& rhs)
{

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
            if (not generic_equal_to(larr[i], rarr[i]))
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
            if (not generic_equal_to(litem.value(), robj.at(litem.key())))
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

bool Operator::generic_less(V const& lhs, V const& rhs)
{

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
    else if (lhs.is_object() and rhs.is_object())
    {
        return is_less_as_object(lhs.get_object(), rhs.get_object());
    }



    auto lkind = lhs.kind();
    auto rkind = rhs.kind();
    std::stringstream msg;
    msg << "undefined JSON order relation for " << lkind << " and " << rkind;
    throw expression_error(msg.str());
    return false;
}




bool Operator::generic_less_equal(V const& lhs, V const& rhs)
{
    return !generic_less(rhs, lhs);
}

V Operator::generic_left_shift(V const& lhs, V const& rhs)
{
    auto const v = boost::json::value_to<std::uint64_t>(lhs);
    auto const shift = boost::json::value_to<std::uint64_t>(rhs);
    return v << shift;
}

V Operator::generic_right_shift(V const& lhs, V const& rhs)
{
    auto const v = boost::json::value_to<std::uint64_t>(lhs);
    auto const shift = boost::json::value_to<std::uint64_t>(rhs);
    return v >> shift;
}

V Operator::generic_negate(V const& x)
{
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
        // TODO: add kind to msg
        throw expression_error("invalid negation operand");
        return nullptr;
    }
}

V Operator::generic_complement(V const& x)
{
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
        // TODO: add kind to msg
        throw expression_error("invalid complement operand");
        return nullptr;
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
    return nullptr;                             \
}                                               \
boost::json::kind result_kind =                 \
common_arithmetic_kind(lhs, rhs);               \
if(X_ZERO_DIV &&                                \
    boost::json::value_to<double>(rhs) == 0)    \
{                                               \
    throw expression_error("zero division");    \
}                                               \
switch (result_kind)                            \
{                                               \
X_BINOP_CASES                                   \
default:                                        \
    throw expression_error("invalid operands"); \
    return nullptr;                             \
}

V Operator::generic_logical_and(V const& lhs, V const& rhs)
{
    return generic_is_truth(lhs) ? rhs : lhs;
}

V Operator::generic_logical_or(V const& lhs, V const& rhs)
{
    return not generic_is_truth(lhs) ? rhs : lhs;
}

V Operator::generic_plus(V const& lhs, V const& rhs)
{
#define X_OP            +
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_minus(V const& lhs, V const& rhs)
{
#define X_OP            -
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_multiplies(V const& lhs, V const& rhs)
{
#define X_OP            *
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}


V Operator::generic_modulus(V const& lhs, V const& rhs)
{
    #define X_OP        %
#define X_ZERO_DIV      true
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_and(V const& lhs, V const& rhs)
{
#define X_OP            &
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_or(V const& lhs, V const& rhs)
{
#define X_OP            |
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

V Operator::generic_bit_xor(V const& lhs, V const& rhs)
{
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

V Operator::generic_divides(V const& lhs, V const& rhs)
{
    if (lhs.is_null() || rhs.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(lhs, rhs);

    if (result_kind == boost::json::kind::null)
    {
        throw expression_error("invalid division operands");
    }
    if(boost::json::value_to<double>(rhs) == 0)
    {
        throw expression_error("zero division");
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

V Operator::generic_pow(V const& lhs, V const& rhs)
{
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


V Operator::generic_log(V const& lhs, V const& rhs)
{
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


V Operator::generic_quot(V const& lhs, V const& rhs)
{
    if (lhs.is_null() || rhs.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(lhs, rhs);

    if (result_kind == boost::json::kind::null)
    {
        throw expression_error("invalid division operands");
    }

    if(boost::json::value_to<double>(rhs) == 0)
    {
        throw expression_error("zero division");
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


bool Operator::generic_logical_not(V const& x)
{
    return !generic_is_truth(x);
}

bool Operator::generic_is_truth(V const& x)
{
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


} // namespace dsl
} // namespace zmbt
