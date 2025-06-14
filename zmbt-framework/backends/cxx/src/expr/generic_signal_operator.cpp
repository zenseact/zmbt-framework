/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/expr/generic_signal_operator.hpp"
#include "zmbt/expr/exceptions.hpp"



#include <zmbt/core/aliases.hpp>
#include <zmbt/core/exceptions.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <sstream>


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
                return zmbt::dsl::GenericSignalOperator(l) < zmbt::dsl::GenericSignalOperator(r);
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




GenericSignalOperator::GenericSignalOperator(boost::json::value const& value)
    : value_{value}
{
}

GenericSignalOperator::GenericSignalOperator(boost::json::array const& value)
    : GenericSignalOperator{static_cast<boost::json::value const&>(value)}
{
}

GenericSignalOperator::GenericSignalOperator(boost::json::object const& value)
    : GenericSignalOperator{static_cast<boost::json::value const&>(value)}
{
}


bool GenericSignalOperator::operator==(GenericSignalOperator const& other) const
{
    boost::json::value const& lhs = value();
    boost::json::value const& rhs = other.value();

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
            if (GenericSignalOperator(larr[i]) != GenericSignalOperator(rarr[i]))
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
            if (GenericSignalOperator(litem.value()) != GenericSignalOperator(robj.at(litem.key())))
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

bool GenericSignalOperator::operator!=(GenericSignalOperator const& rhs) const
{
    return not this->operator==(rhs);
}

bool GenericSignalOperator::operator<(GenericSignalOperator const& other) const
{
    boost::json::value const& lhs = value();
    boost::json::value const& rhs = other.value();

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


bool GenericSignalOperator::operator>=(GenericSignalOperator const& rhs) const
{
    return not this->operator<(rhs);
}


bool GenericSignalOperator::operator>(GenericSignalOperator const& rhs) const
{
    return rhs.operator<(*this);
}


bool GenericSignalOperator::operator<=(GenericSignalOperator const& rhs) const
{
    return not rhs.operator<(*this);
}

boost::json::value GenericSignalOperator::operator<<(GenericSignalOperator const& other) const
{
    auto const v = boost::json::value_to<std::uint64_t>(value());
    auto const shift = boost::json::value_to<std::uint64_t>(other.value());
    return v << shift;
}

boost::json::value GenericSignalOperator::operator>>(GenericSignalOperator const& other) const
{
    auto const v = boost::json::value_to<std::uint64_t>(value());
    auto const shift = boost::json::value_to<std::uint64_t>(other.value());
    return v >> shift;
}


boost::json::value GenericSignalOperator::operator-() const
{
    switch (value_.kind())
    {
    case boost::json::kind::uint64:
        return -value_.get_uint64();
    case boost::json::kind::int64:
        return -value_.get_int64();
    case boost::json::kind::double_:
        return -value_.get_double();
    case boost::json::kind::null:
        return nullptr;
    default:
        // TODO: add kind to msg
        throw expression_error("invalid negation operand");
        return nullptr;
    }
}

boost::json::value GenericSignalOperator::operator~() const
{
    switch (value_.kind())
    {
    case boost::json::kind::uint64:
        return ~value_.get_uint64();
    case boost::json::kind::int64:
        return ~value_.get_int64();
    case boost::json::kind::bool_:
        return !value_.get_bool();
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

#define CASE_DOUBLE case boost::json::kind::double_: return real_to_number(boost::json::value_to<double>(value_)       X_OP boost::json::value_to<double>(rhs.value_));
#define CASE_INT64  case boost::json::kind::int64:   return real_to_number(boost::json::value_to<std::int64_t>(value_) X_OP boost::json::value_to<std::int64_t>(rhs.value_));
#define CASE_UINT64 case boost::json::kind::uint64:  return boost::json::value_to<std::size_t>(value_)  X_OP boost::json::value_to<std::size_t>(rhs.value_);
#define STR(a) #a


#define BINOP_IMPL                                      \
if (value_.is_null() || rhs.value_.is_null())           \
{                                                       \
    return nullptr;                                     \
}                                                       \
boost::json::kind result_kind =                         \
common_arithmetic_kind(value_, rhs.value_);             \
if(X_ZERO_DIV &&                                        \
    boost::json::value_to<double>(rhs.value_) == 0)     \
{                                                       \
    throw expression_error("zero division");            \
}                                                       \
switch (result_kind)                                    \
{                                                       \
X_BINOP_CASES                                           \
default:                                                \
    throw expression_error("invalid operands");         \
    return nullptr;                                     \
}


GenericSignalOperator GenericSignalOperator::operator&&(GenericSignalOperator const& rhs) const
{
    return static_cast<bool>(*this) ? rhs : *this;
}

GenericSignalOperator GenericSignalOperator::operator||(GenericSignalOperator const& rhs) const
{
    return not static_cast<bool>(*this) ? rhs : *this;
}

boost::json::value GenericSignalOperator::operator+(GenericSignalOperator const& rhs) const
{
#define X_OP            +
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

boost::json::value GenericSignalOperator::operator-(GenericSignalOperator const& rhs) const
{
#define X_OP            -
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

boost::json::value GenericSignalOperator::operator*(GenericSignalOperator const& rhs) const
{
#define X_OP            *
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64 CASE_DOUBLE
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}


boost::json::value GenericSignalOperator::operator%(GenericSignalOperator const& rhs) const
{
    #define X_OP        %
#define X_ZERO_DIV      true
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

boost::json::value GenericSignalOperator::operator&(GenericSignalOperator const& rhs) const
{
#define X_OP            &
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

boost::json::value GenericSignalOperator::operator|(GenericSignalOperator const& rhs) const
{
#define X_OP            |
#define X_ZERO_DIV      false
#define X_BINOP_CASES   CASE_INT64 CASE_UINT64
    BINOP_IMPL
#undef X_OP
#undef X_ZERO_DIV
#undef X_BINOP_CASES
}

boost::json::value GenericSignalOperator::operator^(GenericSignalOperator const& rhs) const
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

boost::json::value GenericSignalOperator::operator/(GenericSignalOperator const& rhs) const
{
    if (value_.is_null() || rhs.value_.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(value_, rhs.value_);

    if (result_kind == boost::json::kind::null)
    {
        throw expression_error("invalid division operands");
    }
    if(boost::json::value_to<double>(rhs.value_) == 0)
    {
        throw expression_error("zero division");
    }

    double const result_double = boost::json::value_to<double>(value_) / boost::json::value_to<double>(rhs.value_);

    switch (result_kind)
    {
    case boost::json::kind::double_:
        return result_double;
    case boost::json::kind::uint64:
    {
        if(0 == boost::json::value_to<std::uint64_t>(value_) % boost::json::value_to<std::uint64_t>(rhs.value_))
        {
            return boost::json::value_to<std::uint64_t>(value_) / boost::json::value_to<std::uint64_t>(rhs.value_);
        }
        else
        {
            return result_double;
        }
    }
    case boost::json::kind::int64:
    {
        if(0 == boost::json::value_to<std::int64_t>(value_) % boost::json::value_to<std::int64_t>(rhs.value_))
        {
            return boost::json::value_to<std::int64_t>(value_) / boost::json::value_to<std::int64_t>(rhs.value_);
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

boost::json::value GenericSignalOperator::pow(GenericSignalOperator const& rhs) const
{
    boost::json::value ret;
    if (value_.is_number() && rhs.value_.is_number())
    {
        auto x = dejsonize<double>(value_);
        auto y = dejsonize<double>(rhs.value_);
        ret = json_from(std::pow(x, y));
    }
    else
    {
        auto x = dejsonize<std::complex<double>>(value_);
        auto y = dejsonize<std::complex<double>>(rhs.value_);
        ret = json_from(std::pow(x, y));
    }

    if (ret.is_number())
    {
        ret = real_to_number(boost::json::value_to<double>(ret));
    }
    return ret;
}

boost::json::value GenericSignalOperator::log(GenericSignalOperator const& rhs) const
{
    boost::json::value ret;
    if (value_.is_number() && rhs.value_.is_number())
    {
        auto x = dejsonize<double>(value_);
        auto y = dejsonize<double>(rhs.value_);
        ret = json_from(std::log(x) / std::log(y));
    }
    else
    {
        auto x = dejsonize<std::complex<double>>(value_);
        auto y = dejsonize<std::complex<double>>(rhs.value_);
        ret = json_from(std::log(x) / std::log(y));
    }

    if (ret.is_number())
    {
        ret = real_to_number(boost::json::value_to<double>(ret));
    }
    return ret;

}

boost::json::value GenericSignalOperator::quot(GenericSignalOperator const& rhs) const
{
    if (value_.is_null() || rhs.value_.is_null())
    {
        return nullptr;
    }
    boost::json::kind result_kind =
        common_arithmetic_kind(value_, rhs.value_);

    if (result_kind == boost::json::kind::null)
    {
        throw expression_error("invalid division operands");
    }

    if(boost::json::value_to<double>(rhs.value_) == 0)
    {
        throw expression_error("zero division");
    }

    switch (result_kind)
    {
    case boost::json::kind::double_:
    case boost::json::kind::int64:
        return boost::json::value_to<std::int64_t>(value_) / boost::json::value_to<std::int64_t>(rhs.value_);
    case boost::json::kind::uint64:
        return boost::json::value_to<std::uint64_t>(value_) / boost::json::value_to<std::uint64_t>(rhs.value_);
    default:
        return 0;
    }
}


GenericSignalOperator::operator bool() const
{
    boost::json::value const& self = value();
    switch (self.kind())
    {
    case boost::json::kind::null:
        return false;
        break;

    case boost::json::kind::bool_:
        return self.as_bool();
        break;

    case boost::json::kind::string:
        return 0 != self.as_string().size();
        break;

    case boost::json::kind::array:
        return 0 != self.as_array().size();
        break;

    case boost::json::kind::object:
        return 0 != self.as_object().size();
        break;

    case boost::json::kind::int64:
        return 0 != self.as_int64();
        break;

    case boost::json::kind::uint64:
        return 0 != self.as_uint64();
        break;

    case boost::json::kind::double_:
        return 0 != self.as_double();
        break;

    default:
        return false;
        break;
    }
}


} // namespace dsl
} // namespace zmbt
