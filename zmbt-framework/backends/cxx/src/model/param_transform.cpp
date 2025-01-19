/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/param_transform.hpp"

#include <string>


namespace zmbt {



detail::param_transform_base::param_transform_base(Param const& p, boost::json::array const& ptrs)
    : param{p}
    , pointers{ptrs}
{
    if (pointers.empty()) {
        return;
    }
    if ("/trigger"     == *pointers.cbegin()) { pkind |= Kind::trig_node; }
    if ("/trigger/obj" == *pointers.cbegin()) { pkind |= Kind::trig_obj; }
    for (auto const& jptr: pointers) {
        if ("/trigger/ifc" == jptr) { pkind |= Kind::trig_ifc; continue; }
        auto const& str = jptr.as_string();
        if (str.find("/interface/obj") != std::string::npos)
        {
            pkind |= Kind::ifc_obj; continue;
        }
        if (str.find("/interface/ifc") != std::string::npos)
        {
            pkind |= Kind::ifc_ifc; continue;
        }
        if (str.find("/interface") != std::string::npos)
        {
            pkind |= Kind::ifc_node; continue;
        }
    }
}


}
