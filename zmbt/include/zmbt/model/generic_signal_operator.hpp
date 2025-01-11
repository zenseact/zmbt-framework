/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_GENERIC_SIGNAL_OPERATOR_HPP_
#define ZMBT_MODEL_GENERIC_SIGNAL_OPERATOR_HPP_


#include <boost/json.hpp>
#include <zmbt/core/type_tag.hpp>
#include <zmbt/reflect/initialization_policy.hpp>
#include <iostream>

namespace zmbt {



class GenericSignalOperator
{
    boost::json::value value_;

public:

    explicit GenericSignalOperator(boost::json::value const& value);
    explicit GenericSignalOperator(boost::json::array const& value);
    explicit GenericSignalOperator(boost::json::object const& value);

    template <class T>
    GenericSignalOperator(T const& sample)
        : GenericSignalOperator{boost::json::value_from(sample)}
    {
    }

    template<class T>
    GenericSignalOperator(type_tag<T>)
        : GenericSignalOperator(reflect::initialization<T>::init())
    {
    }

    bool operator==(GenericSignalOperator const& other) const;
    bool operator!=(GenericSignalOperator const& rhs) const;
    bool operator<(GenericSignalOperator const& other) const;
    bool operator>=(GenericSignalOperator const& rhs) const;
    bool operator>(GenericSignalOperator const& rhs) const;
    bool operator<=(GenericSignalOperator const& rhs) const;

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


    boost::json::value pow(GenericSignalOperator const& rhs) const;


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

#endif // ZMBT_MODEL_GENERIC_SIGNAL_OPERATOR_HPP_
