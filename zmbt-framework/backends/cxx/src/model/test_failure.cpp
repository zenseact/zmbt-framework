/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/test_failure.hpp"
#include "zmbt/model/exceptions.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/model/environment.hpp"





// GCOV_EXCL_START

namespace zmbt {

void format_failure_report(std::ostream& os, boost::json::value const& sts)
{
    auto const pretty_print = Environment().Config().pretty_print;
    std::string const verdict = sts.at("verdict").as_string().c_str();

    boost::json::string_view const prefix {"\n      "};
    auto const print_js_section = [&os, &sts, prefix, pretty_print](boost::json::string_view label)
    {
        os << prefix << label << ":";
        if (pretty_print)
        {
            os << '\n';
            zmbt::pretty_print(os, sts.at(label), 2);
        }
        else
        {
            os << ' ' << sts.at(label);
        }
    };

    auto const print_string = [&os, &sts, prefix](boost::json::string_view label)
    {
        auto const str = sts.at(label).as_string();
        if (str.empty()) return;
        os << prefix << label << ": " << sts.at(label);
    };


    os << "  - ZMBT " << verdict << ":";
    print_string("model");
    print_string("message");

    if (verdict == "ERROR")
    {
        os << prefix << "channel: " << sts.at("channel");
    }
    else
    {
        print_js_section("expected");
        print_js_section("observed");
        os << prefix << "condition: " << sts.at("condition");
        print_string("comment");
        print_js_section("test vector");
    }

    print_string("description");


    auto const& eval_stack = sts.at("eval_stack").as_array();
    if (!eval_stack.empty())
    {
        os << prefix << "expression eval stack: |-"
            << prefix << "  ---\n"; // needed to keep valid yaml string under |-
        zmbt::Expression::EvalLog::format(os, eval_stack, 8);
    }
}


void default_test_failure(boost::json::value const& report)
{
    std::stringstream ss;
    format_failure_report(ss, report);
    throw test_assertion_failure(ss.str());
}
} // namespace zmbt

// GCOV_EXCL_END
