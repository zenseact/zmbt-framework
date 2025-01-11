/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_PARAMETER_HPP_
#define ZMBT_CORE_PARAMETER_HPP_

#include <boost/json.hpp>

namespace zmbt {


/// Generic Parameter Placeholder
class Param final
{
    boost::json::string value_;

    Param() = default;
public:


    static bool isParam(boost::json::value const& v);

    static Param parse(boost::json::value const& v);

    boost::json::value serialize() const
    {
        return value_;
    }

    Param(boost::json::value v) : Param{parse(v)} {}

    Param(Param const&) = default;
    Param(Param &&) = default;
    Param& operator=(Param const&) = default;
    Param& operator=(Param &&) = default;
    ~Param() = default;

    operator boost::json::value() const
    {
        return serialize();
    }

    boost::json::string to_string() const
    {
        return value_;
    }

};


}

#endif