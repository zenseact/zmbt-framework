/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/core/json_traverse.hpp"
#include "zmbt/core/format_string.hpp"


namespace zmbt {

bool JsonTraverse::traverse(boost::json::value const& v, std::string const jptr) const
{
    if (visit_(v, jptr)) return true;

    if (v.is_array())
    {
        size_t i {0};
        for (auto const& el: v.get_array())
        {
            if (traverse(el, format("%s/%i", jptr, i++))) return true;
        }
    }
    else if (v.is_object())
    {
        for (auto const& el: v.get_object())
        {
            if (traverse(el.value(), format("%s/%i", jptr, el.key()))) return true;
        }
    }
    return false;
}

} // namespace zmbt
