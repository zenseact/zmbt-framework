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
#include "exceptions.hpp"



#define ZMBT_SOH_HANDLE_UNARY_TRANSFORM(OP, TRAIT, R)                               \
template <class T>                                                                  \
static auto handle_##TRAIT(V const& val)                                            \
-> mp_if<has_##TRAIT<T>, R>                                                         \
{                                                                                   \
    return boost::json::value_to<R>(json_from(OP dejsonize<T>(val)));               \
}                                                                                   \
template <class T>                                                                  \
static auto handle_##TRAIT(V const&)                                                \
-> mp_if<mp_not<has_##TRAIT<T>>, R>                                                 \
{                                                                                   \
    throw expression_error("%s has no defined " #OP " operator", type_name<T>());   \
    return {};                                                                      \
}

#define ZMBT_SOH_HANDLE_BIN_TRANSFORM(OP, TRAIT, R)                                     \
template <class T>                                                                      \
static auto handle_##TRAIT(V const& lhs, V const& rhs)                                  \
-> mp_if<has_##TRAIT<T>, R>                                                             \
{                                                                                       \
    return boost::json::value_to<R>(json_from(dejsonize<T>(lhs) OP dejsonize<T>(rhs))); \
}                                                                                       \
template <class T>                                                                      \
static auto handle_##TRAIT(V const&, V const&)                                          \
-> mp_if<mp_not<has_##TRAIT<T>>, R>                                                     \
{                                                                                       \
    throw expression_error("%s has no defined " #OP " operator", type_name<T>());       \
    return {};                                                                          \
}


namespace zmbt {
namespace lang {

namespace detail
{
/// typetrait to check T::decorated_type
ZMBT_HAS_TYPE(decorated_type)
}

/// Signal transformation and comparison handler. Enables type erasure.
class Operator
{
  public:

    using V = boost::json::value;

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

    static bool generic_is_truth(V const& a);
    static V generic_decorate(V const& a);
    static V generic_undecorate(V const& a);
    static V generic_negate(V const&);
    static V generic_complement(V const&);
    static bool generic_logical_not(V const&);
    static bool generic_equal_to(V const&, V const&);
    static bool generic_less(V const&, V const&);
    static bool generic_less_equal(V const&, V const&);
    static V generic_plus(V const&, V const&);
    static V generic_minus(V const&, V const&);
    static V generic_multiplies(V const&, V const&);
    static V generic_divides(V const&, V const&);
    static V generic_modulus(V const&, V const&);
    static V generic_bit_and(V const&, V const&);
    static V generic_bit_or(V const&, V const&);
    static V generic_bit_xor(V const&, V const&);
    static V generic_left_shift(V const&, V const&);
    static V generic_right_shift(V const&, V const&);
    static V generic_logical_and(V const&, V const&);
    static V generic_logical_or(V const&, V const&);

    static V generic_pow(V const&, V const&);
    static V generic_log(V const&, V const&);
    static V generic_quot(V const&, V const&);


  private:

    // boost::json::string_view config2Str() const

    using unary_transform = std::function<V(V const&)>;
    using binary_transform = std::function<V(V const&, V const&)>;
    using unary_predicate = std::function<bool(V const&)>;
    using binary_predicate = std::function<bool(V const&, V const&)>;

    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(-, negate, V)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(~, complement, V)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(!, logical_not, bool)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(==, equal_to, bool)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<, less, bool)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<=, less_equal, bool)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(+, plus, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(-, minus, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(*, multiplies, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(/, divides, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(%, modulus, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&, bit_and, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(|, bit_or, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(^, bit_xor, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(<<, left_shift, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(>>, right_shift, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&&, logical_and, V)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(||, logical_or, V)

#undef ZMBT_SOH_HANDLE_UNARY_TRANSFORM
#undef ZMBT_SOH_HANDLE_BIN_TRANSFORM


    template <class T>
    static auto handle_decorate(boost::json::value const& a)
        -> mp_if<detail::has_type_decorated_type<T>, boost::json::value>
    {
        using Decorated = typename T::decorated_type;
        return json_from(static_cast<Decorated>(dejsonize<T>(a)));
    }

    template <class T>
    static auto handle_decorate(boost::json::value const& a)
        -> mp_if<mp_not<detail::has_type_decorated_type<T>>, boost::json::value>
    {
        return json_from(dejsonize<T>(a));
    }

    template <class T>
    static auto handle_undecorate(boost::json::value const& a)
        -> mp_if<detail::has_type_decorated_type<T>, boost::json::value>
    {
        using Decorated = typename T::decorated_type;
        return json_from(static_cast<T>(dejsonize<Decorated>(a)));
    }

    template <class T>
    static auto handle_undecorate(boost::json::value const& a)
        -> mp_if<mp_not<detail::has_type_decorated_type<T>>, boost::json::value>
    {
        return json_from(dejsonize<T>(a));
    }

    template <class T>
    static auto handle_is_truth(boost::json::value const& val) -> mp_if<is_convertible<T, bool>, bool>
    {
        return static_cast<bool>(dejsonize<T>(val));
    }

    template <class T>
    static auto handle_is_truth(boost::json::value const&) -> mp_if<mp_not<is_convertible<T, bool>>, bool>
    {
        throw expression_error("%s has no boolean conversion defined", type_name<T>());
        return false;
    }



    struct Handle {

        boost::json::string annotation{""};
        struct D {
            unary_transform     decorate{generic_decorate};
            unary_transform     undecorate{generic_undecorate};
        } decor;

        struct C {
            binary_predicate equal_to{generic_equal_to};
            binary_predicate less{generic_less};
            binary_predicate less_equal{generic_less_equal};
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
            unary_predicate  bool_{generic_is_truth};
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
    boost::json::value apply(lang::Keyword const& keyword, boost::json::value const& lhs, boost::json::value const& rhs) const;

    /// Is true
    bool is_truth(boost::json::value const& a) const
    {
        return apply(lang::Keyword::Bool, nullptr, a).get_bool();
    }

private:

    /// Is approximately equal (only applicable to floating point numbers)
    virtual bool is_approx(boost::json::value const& sample, boost::json::value const& expr) const;

    /// Is subset of
    virtual bool is_subset(boost::json::value const& lhs, boost::json::value const& rhs) const;

    /// Is element of
    virtual bool contains(boost::json::value const& set, boost::json::value const& element) const;

};

} // namespace lang
} // namespace zmbt

#endif // ZMBT_EXPR_SIGNAL_OPERATOR_HANDLER_HPP_
