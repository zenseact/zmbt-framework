/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/core/parameter.hpp"
#include "zmbt/core/format_string.hpp"

namespace zmbt {



bool Param::isParam(boost::json::value const& v)
{
    if (not v.is_string()) return false;
    auto const& s = v.as_string();
    if (s.size() < 3) return false;
    if (s.front() != '$') return false;
    if (s.at(1) != '{') return false;
    if (s.back() != '}') return false;
    return true;
}

Param Param::parse(boost::json::value const& v)
{
    auto p = Param();
    p.value_ = isParam(v) ? v.get_string() : boost::json::string{format("${%d}", v)};
    return p;
}

}
