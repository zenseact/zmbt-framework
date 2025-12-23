/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/application/test_failure.hpp"
#include "zmbt/model/exceptions.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/model/environment.hpp"





// GCOV_EXCL_START

namespace zmbt {

void format_failure_report(std::ostream& os, boost::json::value const& sts)
{
    auto const pretty_print = Logger::is_pretty_print_enabled();
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

    auto const print_expr = [&os, &sts, prefix](boost::json::string_view label)
    {
        os << prefix << label <<  ": ";
        lang::Expression(sts.at(label)).prettify_to(os);
    };


    os << "\n  - ZMBT " << verdict << ":";
    print_string("model");
    print_string("message");


    auto const& test_row = sts.at("test_row");
    auto const& test_column = sts.at("test_column");
    auto const& pipe_id = sts.at("pipe");

    os << prefix << "pipe: " << pipe_id;
    os << prefix << "channel: " << sts.at("channel");
    os << prefix << "test_row: " << test_row;
    os << prefix << "test_column: " << test_column;


    if (verdict == "ERROR")
    {
        print_expr("error");
    }
    else
    {
        print_expr("expected");
        print_js_section("observed");
        print_string("comment");
    }

    print_string("description");

    auto const& traces = sts.at("traces").as_array();
    if (!traces.empty())
    {
        os << prefix << "traces:";
        for (auto const& msg: traces)
        {
            boost::json::object const* if_object;
            if ((if_object = msg.if_object()) && !if_object->empty())
            {
                os << prefix << "  - " << if_object->begin()->key() << ": " << if_object->begin()->value();
            }
            else
            {
                os << prefix << "  - " << msg;
            }
        }
    }

    auto const& eval_stack = sts.at("eval_stack").as_array();
    if (!eval_stack.empty())
    {
        os << prefix << "evaluation: |-"
            << prefix << "  ---\n"; // needed to keep valid yaml string under |-
        zmbt::lang::EvalLog::format(os, eval_stack, 8);
    }
}


void default_test_failure(boost::json::value const& report)
{
    std::stringstream ss;
    format_failure_report(ss, report);
    throw_exception(test_assertion_failure(ss.str()));
}
} // namespace zmbt

// GCOV_EXCL_END
