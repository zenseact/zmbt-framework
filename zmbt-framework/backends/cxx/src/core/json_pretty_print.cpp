/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>
#include <ostream>
#include <string>

#include "zmbt/core/json_pretty_print.hpp"


namespace  {

std::ostream& pretty_print_impl(std::ostream&, boost::json::value const&, int const);

void print_array_items(std::ostream& os, boost::json::array const& arr, int const indent)
{
    if (arr.empty()) {
        os << "[]";
        return;
    }

    std::string const sep = std::string("\n") + std::string((indent+1)*4, ' ');
    os << '[' << sep;

    auto it = arr.cbegin();
    pretty_print_impl(os, *(it++), indent +1);

    while (it != arr.end())
    {
        os << ',' << sep;
        pretty_print_impl(os, *(it++), indent +1);
    }

    os << '\n' << std::string((indent)*4, ' ') << ']';
}

void print_object_items(std::ostream& os, boost::json::object const& obj, int const indent)
{
    if (obj.empty())
    {
        os << "{}";
        return;
    }
    std::string const sep = std::string("\n") + std::string((indent+1)*4, ' ');
    os << '{' << sep;

    auto it = obj.cbegin();
    os << boost::json::serialize(it->key()) << ": ";
    pretty_print_impl(os, it->value(), indent+1);
    it++;
    while (it != obj.cend())
    {
        os << ',' << sep << boost::json::serialize(it->key()) << ": ";
        pretty_print_impl(os, it->value(), indent+1);
        it++;
    }

    os << '\n' << std::string((indent)*4, ' ') << '}';
}


std::ostream& pretty_print_impl(std::ostream& os, boost::json::value const& jv, int const indent = 0)
{

    switch (jv.kind()) {
        case boost::json::kind::object:
        {
            print_object_items(os, jv.get_object(), indent);
            break;
        }

        case boost::json::kind::array:
        {
            print_array_items(os, jv.get_array(), indent);
            break;
        }

        default:
        {
            os << boost::json::serialize(jv);
            break;
        }
    }

    return os;
}
} // namespace



std::ostream& zmbt::pretty_print(std::ostream& os, boost::json::value const& jv, int const indent)
{
    os << std::string((indent)*4, ' ');
    return pretty_print_impl(os, jv, indent);
    return os;
}
