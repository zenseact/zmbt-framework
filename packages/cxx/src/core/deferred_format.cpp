/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include "zmbt/core/deferred_format.hpp"

#include <boost/format.hpp>
#include <zmbt/core/json_node.hpp>
#include <cstddef>

#include "zmbt/core/format_string.hpp"
#include "zmbt/core/parameter.hpp"


namespace {

boost::json::object make_deferred_params(boost::json::array const& tokens)
{
    boost::json::object map;
    for (std::size_t i = 0; i < tokens.size(); i++)
    {
        boost::json::value const& token = tokens.at(i);
        if (zmbt::Param::isParam(token))
        {
            map.emplace(token.as_string(), zmbt::format("/fmtargs/%d", i));
        }
    }
    return map;
}

}

namespace zmbt {



bool DeferredFormat::isDeferredFormat(boost::json::value const v)
{
    if (v.is_object())
    {
        auto const& o = v.get_object();
        return (o.contains("base") && o.contains("fmtargs") && o.contains("deferred_params"));
    }
    return false;
}




DeferredFormat::DeferredFormat(boost::json::value const& v)
{
    if (v.is_string())
    {
        if (Param::isParam(v))
        {
            data = JsonNode({
                {"base", "%s"},
                {"fmtargs", boost::json::array{v}},
                {"deferred_params", boost::json::object{{v.as_string(), "/fmtargs/0"}}},
            });
        }
        else
        {
            data = JsonNode({
                {"base", v},
                {"fmtargs", boost::json::array{}},
                {"deferred_params", boost::json::object{}},
            });
        }

    }
    else if (v.is_object())
    {
        boost::json::object const& o = v.as_object();
        data = JsonNode({
            {"base"  , o.at("base")},
            {"fmtargs", o.at("fmtargs")},
            {"deferred_params"   , o.at("deferred_params")},
        });
    }
    else if (v.is_array())
    {
        boost::json::array a = v.as_array();
        boost::json::array fmtargs {};
        if (a.size() > 1) {
            fmtargs = boost::json::array(a.cbegin()+1, a.cend());
        }
        data = JsonNode({
            {"base"  , a.at(0).as_string()},
            {"fmtargs", fmtargs},
            {"deferred_params"   , make_deferred_params(fmtargs)},
        });
    }

}

boost::json::object const& DeferredFormat::deferred_params() const&
{
    return data.at("/deferred_params").as_object();
}

DeferredFormat::operator boost::json::value() const
{
    return data.node();
}

boost::json::string DeferredFormat::to_string() const
{
    boost::format out {data.at("/base").as_string().c_str()};
    for (boost::json::value const& p: data.at("/fmtargs").as_array())
    {
        if (p.is_string()) {
            out = out % p.as_string().c_str();
        }
        else {

            out = out % p;
        }
    }
    return out.str().c_str();
}

}
