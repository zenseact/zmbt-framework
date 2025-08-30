/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_SIGNAL_OPERATOR_HANDLER_HPP_
#define ZMBT_EXPR_SIGNAL_OPERATOR_HANDLER_HPP_

#include <limits>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "keyword.hpp"
#include "lazy_param.hpp"
#include <boost/current_function.hpp>



#define ZMBT_SOH_HANDLE_UNARY_TRANSFORM(OP, TRAIT)       \
template <class T>                                       \
static auto handle_##TRAIT(LV val)                       \
-> mp_if<has_##TRAIT<T>, V>                              \
try {                                                    \
    return json_from(OP dejsonize<T>(val));              \
}                                                        \
catch(const std::exception& e)                           \
{                                                        \
    return detail::make_error_expr(                      \
        e.what(), BOOST_CURRENT_FUNCTION);               \
}                                                        \
template <class T>                                       \
static auto handle_##TRAIT(LV)                           \
-> mp_if<mp_not<has_##TRAIT<T>>, V>                      \
try {                                                    \
    return detail::make_error_expr(                      \
        "invalid operand", BOOST_CURRENT_FUNCTION);      \
}                                                        \
catch(const std::exception& e)                           \
{                                                        \
    return detail::make_error_expr(                      \
        e.what(), BOOST_CURRENT_FUNCTION);               \
}

#define ZMBT_SOH_HANDLE_BIN_TRANSFORM(OP, TRAIT)                 \
template <class T>                                               \
static auto handle_##TRAIT(LV lhs, LV rhs)                       \
-> mp_if<has_##TRAIT<T>, V>                                      \
try {                                                            \
    return json_from(dejsonize<T>(lhs) OP dejsonize<T>(rhs));    \
}                                                                \
catch(const std::exception& e)                                   \
{                                                                \
    return detail::make_error_expr(                              \
        e.what(), BOOST_CURRENT_FUNCTION);                       \
}                                                                \
template <class T>                                               \
static auto handle_##TRAIT(LV, LV)                               \
-> mp_if<mp_not<has_##TRAIT<T>>, V>                              \
try {                                                            \
    return detail::make_error_expr(                              \
        "invalid operands", BOOST_CURRENT_FUNCTION);             \
}                                                                \
catch(const std::exception& e)                                   \
{                                                                \
    return detail::make_error_expr(                              \
        e.what(), BOOST_CURRENT_FUNCTION);                       \
}

namespace zmbt {
namespace lang {

namespace detail
{
/// typetrait to check T::decorated_type
ZMBT_HAS_TYPE(decorated_type)

boost::json::value make_error_expr(boost::json::string_view msg, boost::json::string_view ctx);

}

/// Signal transformation and comparison handler. Enables type erasure.
class Operator
{
  public:

    struct operator_error : public base_error {
        using base_error::base_error;
    };

    using V = boost::json::value;
    using LV = LazyParam;

    enum Config : std::uint32_t
    {
        Null,
        Decor       = 1U << 0,
        Comparison  = 1U << 1,
        Arithmetics = 1U << 2,
        Bitwise     = 1U << 3,
        Shift       = 1U << 4,
        Logic       = 1U << 5,
        Default     = Decor|Comparison|Arithmetics|Bitwise|Shift,
        Full        = Default|Logic,
    };

    static V generic_is_truth   (LV);
    static V generic_decorate   (LV);
    static V generic_undecorate (LV);
    static V generic_negate     (LV);
    static V generic_complement (LV);
    static V generic_logical_not(LV);
    static V generic_equal_to   (LV, LV);
    static V generic_less       (LV, LV);
    static V generic_less_equal (LV, LV);
    static V generic_plus       (LV, LV);
    static V generic_minus      (LV, LV);
    static V generic_multiplies (LV, LV);
    static V generic_divides    (LV, LV);
    static V generic_modulus    (LV, LV);
    static V generic_bit_and    (LV, LV);
    static V generic_bit_or     (LV, LV);
    static V generic_bit_xor    (LV, LV);
    static V generic_left_shift (LV, LV);
    static V generic_right_shift(LV, LV);
    static V generic_logical_and(LV, LV);
    static V generic_logical_or (LV, LV);

    static V generic_pow (LV, LV);
    static V generic_log (LV, LV);
    static V generic_quot(LV, LV);


  private:

    using unary_transform = std::function<V(LV)>;
    using binary_transform = std::function<V(LV, LV)>;

    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(-, negate)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(~, complement)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(!, logical_not)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(==, equal_to)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<, less)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<=, less_equal)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(+, plus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(-, minus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(*, multiplies)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(/, divides)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(%, modulus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&, bit_and)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(|, bit_or)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(^, bit_xor)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<<, left_shift)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(>>, right_shift)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&&, logical_and)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(||, logical_or)

#undef ZMBT_SOH_HANDLE_UNARY_TRANSFORM
#undef ZMBT_SOH_HANDLE_BIN_TRANSFORM


    template <class T>
    static auto handle_decorate(LV v)
        -> mp_if<detail::has_type_decorated_type<T>, boost::json::value>
    try
    {
        using Decorated = typename T::decorated_type;
        return json_from(static_cast<Decorated>(dejsonize<T>(v)));
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), BOOST_CURRENT_FUNCTION);
    }

    template <class T>
    static auto handle_decorate(boost::json::value const& a)
        -> mp_if<mp_not<detail::has_type_decorated_type<T>>, boost::json::value>
    try
    {
        return json_from(dejsonize<T>(a));
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), BOOST_CURRENT_FUNCTION);
    }

    template <class T>
    static auto handle_undecorate(LV const& a)
        -> mp_if<detail::has_type_decorated_type<T>, boost::json::value>
    try
    {
        using Decorated = typename T::decorated_type;
        return json_from(static_cast<T>(dejsonize<Decorated>(a)));
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), BOOST_CURRENT_FUNCTION);
    }

    template <class T>
    static auto handle_undecorate(LV const& a)
        -> mp_if<mp_not<detail::has_type_decorated_type<T>>, boost::json::value>
    try
    {
        return json_from(dejsonize<T>(a));
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), BOOST_CURRENT_FUNCTION);
    }

    template <class T>
    static auto handle_is_truth(LV const& val) -> mp_if<is_convertible<T, bool>, boost::json::value>
    try
    {
        return static_cast<bool>(dejsonize<T>(val));
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), BOOST_CURRENT_FUNCTION);
    }

    template <class T>
    static auto handle_is_truth(LV const&) -> mp_if<mp_not<is_convertible<T, bool>>, boost::json::value>
    {
        return detail::make_error_expr("invalid operand", BOOST_CURRENT_FUNCTION);
    }



    struct Handle {

        boost::json::string annotation{""};
        struct D {
            unary_transform     decorate{generic_decorate};
            unary_transform     undecorate{generic_undecorate};
        } decor;

        struct C {
            binary_transform equal_to{generic_equal_to};
            binary_transform less{generic_less};
            binary_transform less_equal{generic_less_equal};
        } comp;

        struct A {
            unary_transform neg{generic_negate};
            binary_transform add{generic_plus};
            binary_transform sub{generic_minus};
            binary_transform mul{generic_multiplies};
            binary_transform div{generic_divides};
            binary_transform mod{generic_modulus};
        } arithmetics;

        struct B {
            unary_transform compl_{generic_complement};
            binary_transform and_{generic_bit_and};
            binary_transform or_{generic_bit_or};
            binary_transform xor_{generic_bit_xor};
        } bitwise;

        struct S {
            binary_transform left{generic_left_shift};
            binary_transform right{generic_right_shift};
        } shift;

        struct L {
            unary_transform  bool_{generic_is_truth};
            binary_transform and_{generic_logical_and};
            binary_transform or_{generic_logical_or};
        } logic;
    };

    template <class T>
    static Handle makeHandle(type_tag<T>, Config const cfg = Default) {
        return {
            format("%s#%d", type_name<T>(), cfg).c_str(),
            Decor & cfg ? Handle::D{
                handle_decorate<T>,
                handle_undecorate<T>
            } : Handle::D{},
            Comparison & cfg ? Handle::C{
                handle_equal_to<T>,
                handle_less<T>,
                handle_less_equal<T>
            } : Handle::C{},
            Arithmetics & cfg ? Handle::A{
                handle_negate<T>,
                handle_plus<T>,
                handle_minus<T>,
                handle_multiplies<T>,
                handle_divides<T>,
                handle_modulus<T>
            } : Handle::A{},
            Bitwise & cfg ? Handle::B{
                handle_complement<T>,
                handle_bit_and<T>,
                handle_bit_or<T>,
                handle_bit_xor<T>
            } : Handle::B{},
            Shift & cfg ? Handle::S{
                handle_left_shift<T>,
                handle_right_shift<T>
            } : Handle::S{},
            Logic & cfg ? Handle::L{
                handle_is_truth<T>,
                handle_logical_and<T>,
                handle_logical_or<T>
            } : Handle::L{},
        };
    }

    Handle handle_;

    explicit Operator(
        Handle const handle
    );

    static bool exchangeHandle(Handle& handle, bool const retrieve);

public:

    /// Default operator with generic transforms
    Operator();

    /// Operator with T as type decorator
    template <class T>
    Operator(type_tag<T> tag, Config const cfg)
    : Operator{makeHandle(tag, cfg)}
    {
    }

    /// Operator with T as type decorator
    template <class T>
    Operator(type_tag<T> tag)
    : Operator{tag, Config::Default}
    {
    }

    /// Retrieve registered operator instance if it exists, throw otherwise
    Operator(boost::json::string_view annotation);

    Operator(Operator const&) = default;
    Operator(Operator &&) = default;
    virtual ~Operator() = default;
    Operator& operator=(Operator const&) = default;
    Operator& operator=(Operator &&) = default;

    bool is_generic() const
    {
        return annotation().empty();
    }

    /// decorated type name
    boost::json::string annotation() const
    {
        return handle_.annotation;
    }

    /// Reserialize as decorated type
    boost::json::value decorate(boost::json::value const& a) const
    {
        return handle_.decor.decorate(a);
    }

    /// Reserialize as decorated type
    boost::json::value undecorate(boost::json::value const& a) const
    {
        return handle_.decor.undecorate(a);
    }


    /// \brief Apply operands
    /// \details For unary operators, lhs is nullptr
    boost::json::value apply(lang::Keyword const& keyword, LV lhs, LV rhs) const;


private:

    /// Is subset of
    V is_subset(LV const& lhs, LV const& rhs) const;

    /// Is element of
    V contains(LV const& set, LV const& element) const;

};

} // namespace lang
} // namespace zmbt

#endif // ZMBT_EXPR_SIGNAL_OPERATOR_HANDLER_HPP_
