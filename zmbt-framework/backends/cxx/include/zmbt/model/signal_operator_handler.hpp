/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_MODEL_SIGNAL_OPERATOR_HANDLER_HPP_
#define ZMBT_MODEL_SIGNAL_OPERATOR_HANDLER_HPP_

#include <limits>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "keyword.hpp"
#include "exceptions.hpp"


#define ZMBT_SOH_HANDLE_UNARY_TRANSFORM(OP, TRAIT)                                              \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const& val)                                    \
-> mp_if<TRAIT<T>, boost::json::value>                                                          \
{                                                                                               \
    return json_from(OP dejsonize<T>(val));                                   \
}                                                                                               \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const&)                                        \
-> mp_if<mp_not<TRAIT<T>>, boost::json::value>                                                  \
{                                                                                               \
    throw expression_error("%s has no defined " #OP " operator", type_name<T>());               \
    return nullptr;                                                                             \
}


#define ZMBT_SOH_HANDLE_BIN_TRANSFORM(OP, TRAIT)                                                \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const& lhs, boost::json::value const& rhs)     \
-> mp_if<TRAIT<T>, boost::json::value>                                                          \
{                                                                                               \
    return json_from(dejsonize<T>(lhs) OP dejsonize<T>(rhs));        \
}                                                                                               \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const&, boost::json::value const&)             \
-> mp_if<mp_not<TRAIT<T>>, boost::json::value>                                                  \
{                                                                                               \
    throw expression_error("%s has no defined " #OP " operator", type_name<T>());               \
    return nullptr;                                                                             \
}


#define ZMBT_SOH_HANDLE_RELATION(OP, TRAIT)                                                     \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const& lhs, boost::json::value const& rhs)     \
-> mp_if<TRAIT<T>, bool>                                                                        \
{                                                                                               \
    return dejsonize<T>(lhs) OP dejsonize<T>(rhs);                            \
}                                                                                               \
template <class T>                                                                              \
static auto handle_if_##TRAIT(boost::json::value const&, boost::json::value const&)             \
-> mp_if<mp_not<TRAIT<T>>, bool>                                                                \
{                                                                                               \
    throw expression_error("%s has no defined " #OP " operator", type_name<T>());               \
    return false;                                                                               \
}

namespace zmbt {

namespace detail
{
/// typetrait to check T::decorated_type
ZMBT_HAS_TYPE(decorated_type)
}


/// Signal transformation and comparison handler. Enables type erasure.
class SignalOperatorHandler
{
    using decorate_fn = std::function<boost::json::value(boost::json::value const&)>;
    using unary_predicate = std::function<bool(boost::json::value const&)>;
    using binary_predicate = std::function<bool(boost::json::value const&, boost::json::value const&)>;
    using unary_transform = std::function<boost::json::value(boost::json::value const&)>;
    using binary_transform = std::function<boost::json::value(boost::json::value const&, boost::json::value const&)>;

    ZMBT_SOH_HANDLE_RELATION(==, has_equal_to)
    ZMBT_SOH_HANDLE_RELATION(<, has_less)
    ZMBT_SOH_HANDLE_RELATION(<=, has_less_equal)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(-, has_negate)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(~, has_complement)
    ZMBT_SOH_HANDLE_UNARY_TRANSFORM(!, has_logical_not)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(+, has_plus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(-, has_minus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(*, has_multiplies)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(/, has_divides)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(%, has_modulus)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&, has_bit_and)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(|, has_bit_or)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(^, has_bit_xor)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(&&, has_logical_and)
    ZMBT_SOH_HANDLE_BIN_TRANSFORM(||, has_logical_or)

#undef ZMBT_SOH_HANDLE_RELATION
#undef ZMBT_SOH_HANDLE_UNARY_TRANSFORM
#undef ZMBT_SOH_HANDLE_BIN_TRANSFORM


    template <class T>
    static auto decorate(boost::json::value const& a)
        -> mp_if<detail::has_type_decorated_type<T>, boost::json::value>
    {
        using Decorated = typename T::decorated_type;
        return json_from(static_cast<Decorated>(dejsonize<T>(a)));
    }

    template <class T>
    static auto decorate(boost::json::value const& a)
        -> mp_if<mp_not<detail::has_type_decorated_type<T>>, boost::json::value>
    {
        return json_from(dejsonize<T>(a));
    }

    template <class T>
    static auto is_truth(boost::json::value const& val) -> mp_if<is_convertible<T, bool>, bool>
    {
        return static_cast<bool>(dejsonize<T>(val));
    }

    template <class T>
    static auto is_truth(boost::json::value const&) -> mp_if<mp_not<is_convertible<T, bool>>, bool>
    {
        throw expression_error("%s has no boolean conversion defined", type_name<T>());
        return false;
    }


    boost::json::string      annotation_;
    struct operators_t {
        decorate_fn     decorate_;
        unary_predicate  is_truth_;
        binary_predicate is_equal_;
        binary_predicate is_lt_; // lesser than
        binary_predicate is_le_; // lesser or equal

        unary_transform neg_;
        unary_transform compl_;
        unary_transform not_;

        binary_transform add_;
        binary_transform sub_;
        binary_transform mul_;
        binary_transform div_;
        binary_transform mod_;

        binary_transform conj_;
        binary_transform disj_;
        binary_transform bxor_;
        binary_transform land_;
        binary_transform lor_;
    } operators;


    SignalOperatorHandler(
        boost::json::string_view  annotation,
        operators_t op
    );

public:

    /// Default operator with GenericSignalOperator as type decorator
    SignalOperatorHandler();

    /// SignalOperatorHandler with T as type decorator
    template <class T>
    SignalOperatorHandler(type_tag<T>)
        : SignalOperatorHandler{
            type_name<T>(),
            operators_t {
                decorate<T>,
                is_truth<T>,
                handle_if_has_equal_to<T>,
                handle_if_has_less<T>,
                handle_if_has_less_equal<T>,
                handle_if_has_negate<T>,
                handle_if_has_complement<T>,
                handle_if_has_logical_not<T>,
                handle_if_has_plus<T>,
                handle_if_has_minus<T>,
                handle_if_has_multiplies<T>,
                handle_if_has_divides<T>,
                handle_if_has_modulus<T>,
                handle_if_has_bit_and<T>,
                handle_if_has_bit_or<T>,
                handle_if_has_bit_xor<T>,
                handle_if_has_logical_and<T>,
                handle_if_has_logical_or<T>
            }
        }
    {
    }

    SignalOperatorHandler(SignalOperatorHandler const&) = default;
    SignalOperatorHandler(SignalOperatorHandler &&) = default;
    virtual ~SignalOperatorHandler() = default;
    SignalOperatorHandler& operator=(SignalOperatorHandler const&) = default;
    SignalOperatorHandler& operator=(SignalOperatorHandler &&) = default;


    /// decorated type name
    boost::json::string annotation() const
    {
        return annotation_;
    }

    /// Reserialize as decorated type
    boost::json::value decorate(boost::json::value const& a) const
    {
        return operators.decorate_(a);
    }

    /// \brief Apply operands
    /// \details For unary operators, lhs is nullptr
    boost::json::value apply(Keyword const& keyword, boost::json::value const& lhs, boost::json::value const& rhs) const;

    /// Is true
    bool is_truth(boost::json::value const& a) const
    {
        return apply(Keyword::Bool, nullptr, a).get_bool();
    }

private:

    /// Is approximately equal (only applicable to floating point numbers)
    virtual bool is_approx(boost::json::value const& sample, boost::json::value const& expr) const;

    /// Is subset of
    virtual bool is_subset(boost::json::value const& lhs, boost::json::value const& rhs) const;

    /// Is element of
    virtual bool contains(boost::json::value const& set, boost::json::value const& element) const;

};

} // namespace zmbt

#endif // ZMBT_MODEL_SIGNAL_OPERATOR_HANDLER_HPP_
