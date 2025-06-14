/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_GENERIC_SIGNAL_OPERATOR_HPP_
#define ZMBT_EXPR_GENERIC_SIGNAL_OPERATOR_HPP_


#include <boost/json.hpp>
#include <zmbt/core/type_tag.hpp>
#include <zmbt/reflect/serialization.hpp>
#include <zmbt/reflect/signal_traits.hpp>
#include <iostream>

namespace zmbt {


/// @brief Generic operator for signal in JSON representation, enabling type erasure.
/// @details Provides default operators based on signal type, known at the construction.
/// Operator call will return null if it is not supported for the given signal type.
class GenericSignalOperator
{
    boost::json::value value_;

public:

    GenericSignalOperator() = default;
    explicit GenericSignalOperator(boost::json::value const& value);
    explicit GenericSignalOperator(boost::json::array const& value);
    explicit GenericSignalOperator(boost::json::object const& value);

    template <class T>
    GenericSignalOperator(T const& sample)
        : GenericSignalOperator{json_from(sample)}
    {
    }

    // template<class T>
    // GenericSignalOperator(type_tag<T>)
    //     : GenericSignalOperator(reflect::signal_traits<T>::init())
    // {
    // }

    bool operator==(GenericSignalOperator const& other) const;
    bool operator!=(GenericSignalOperator const& rhs) const;
    bool operator<(GenericSignalOperator const& other) const;
    bool operator>=(GenericSignalOperator const& rhs) const;
    bool operator>(GenericSignalOperator const& rhs) const;
    bool operator<=(GenericSignalOperator const& rhs) const;

    GenericSignalOperator operator&&(GenericSignalOperator const& rhs) const;
    GenericSignalOperator operator||(GenericSignalOperator const& rhs) const;

    boost::json::value operator-() const;
    boost::json::value operator~() const;

    boost::json::value operator+(GenericSignalOperator const& rhs) const;
    boost::json::value operator-(GenericSignalOperator const& rhs) const;
    boost::json::value operator*(GenericSignalOperator const& rhs) const;
    boost::json::value operator/(GenericSignalOperator const& rhs) const;
    boost::json::value operator%(GenericSignalOperator const& rhs) const;
    boost::json::value operator&(GenericSignalOperator const& rhs) const;
    boost::json::value operator|(GenericSignalOperator const& rhs) const;
    boost::json::value operator^(GenericSignalOperator const& rhs) const;
    boost::json::value operator<<(GenericSignalOperator const& rhs) const;
    boost::json::value operator>>(GenericSignalOperator const& rhs) const;

    boost::json::value pow(GenericSignalOperator const& rhs) const;
    boost::json::value log(GenericSignalOperator const& rhs) const;
    boost::json::value quot(GenericSignalOperator const& rhs) const;



    operator bool() const;


    boost::json::value const& value() const& {
        return value_;
    }


    operator boost::json::value() const& {
        return value();
    }

    friend std::ostream& operator<<(std::ostream& os, GenericSignalOperator const& sample)
    {
        os << sample.value();
        return os;
    }

};


}

#endif // ZMBT_EXPR_GENERIC_SIGNAL_OPERATOR_HPP_
