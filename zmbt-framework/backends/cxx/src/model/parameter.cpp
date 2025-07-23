/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/model/parameter.hpp"

namespace zmbt {



bool Param::isParam(boost::json::value const& v)
{
    if (not v.is_string()) return false;
    return zmbt::lang::Expression(v).is_preproc();
}

}
