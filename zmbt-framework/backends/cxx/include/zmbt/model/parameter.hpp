/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_PARAMETER_HPP_
#define ZMBT_MODEL_PARAMETER_HPP_

#include <boost/json.hpp>
#include <zmbt/expr.hpp>


namespace zmbt {


/// Generic Parameter Placeholder
class Param final : public zmbt::lang::Expression
{
    boost::json::string value_;

    Param() = default;
public:


    static bool isParam(boost::json::value const& v);

    // static Param parse(boost::json::value const& v);

    Param(boost::json::value v)
        : Expression(Expression::encodeTerminal(Keyword::PreProc, zmbt::format("$[%s]", v).c_str()))
    {}

    Param(Param const&) = default;
    Param(Param &&) = default;
    Param& operator=(Param const&) = default;
    Param& operator=(Param &&) = default;
    ~Param() = default;
};


}

#endif