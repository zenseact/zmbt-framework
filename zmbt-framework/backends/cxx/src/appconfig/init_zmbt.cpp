/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_APPLICATION_INIT_HPP_
#define ZMBT_APPLICATION_INIT_HPP_

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/json.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/program_options.hpp>

#include <zmbt/logging.hpp>
#include <zmbt/reflect.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/time_formatters_limited.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace logging = boost::log;
namespace attrs = logging::attributes;
namespace expr = logging::expressions;
namespace keywords = logging::keywords;
namespace sinks = logging::sinks;


namespace
{

void open_json_log(sinks::text_file_backend::stream_type& file)
{
    file << "[\n";
}

void close_json_log(sinks::text_file_backend::stream_type& file)
{
    file << "]\n";
}


void format_json_log(logging::record_view const& rec, logging::formatting_ostream& strm)
{
    boost::json::object obj {};

    if (auto timestamp = logging::extract<boost::posix_time::ptime>("TimeStamp", rec))
    {
        obj.emplace("timestamp", boost::posix_time::to_iso_extended_string(timestamp.get()));
    }

#if !defined(BOOST_LOG_NO_THREADS)
    if (auto thread_id = logging::extract<attrs::current_thread_id::value_type>("ThreadID", rec))
    {
        obj.emplace("thread", thread_id->native_id());
    }
#endif

    if (auto timestamp = logging::extract<boost::posix_time::ptime>("TimeStamp", rec))
    {
        obj.emplace("timestamp", boost::posix_time::to_iso_extended_string(timestamp.get()));
    }
    if (auto filename = logging::extract<std::string>("File", rec))
    {
        obj.emplace("file", filename.get());
    }
    if (auto line = logging::extract<int>("Line", rec))
    {
        obj.emplace("line", line.get());
    }
    if (auto funcname = logging::extract<std::string>("Func", rec))
    {
        obj.emplace("func", funcname.get());
    }
    if (auto severity = rec[logging::trivial::severity])
    {
        if (auto severity_str = logging::trivial::to_string(severity.get()))
        {
            obj.emplace("severity", severity_str);
        }
    }
    if (auto smessage = rec[expr::smessage])
    {
        // TODO: refine to pass json value ref instead of reserializing it

        boost::json::error_code ec;
        boost::json::value smessage_v = boost::json::parse(smessage.get(), ec);
        if (!ec.failed())
        {
            obj.emplace("message", smessage_v);
        }
        else {
            obj.emplace("message", smessage.get());
        }
    }

    strm << obj << ",";
}


void init_logging(std::string log_lvl, std::string log_sink)
{

    logging::trivial::severity_level severity;
    if (!from_string(log_lvl.c_str(), log_lvl.size(), severity))
    {
        severity = logging::trivial::severity_level::warning;
        BOOST_LOG_TRIVIAL(warning) << "invalid option for zmbt_log_level: " << log_lvl << ", using default warning";
    }


    logging::core::get()->add_thread_attribute("Tag", boost::log::attributes::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("File", boost::log::attributes::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("Func", boost::log::attributes::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("Line", boost::log::attributes::mutable_constant<int>(0));

    logging::add_console_log(std::cerr,
        logging::keywords::format = ZMBT_LOG_FORMAT,
        logging::keywords::filter = (logging::trivial::severity >= severity)
    );

    if ("" == log_sink) return;

    using file_sink = sinks::synchronous_sink<sinks::text_file_backend>;
    boost::shared_ptr<file_sink> sink(new file_sink(keywords::file_name = log_sink + ".log.json"));

    sink->set_filter(expr::attr<std::string>("Tag") == "JSON");

    sink->set_formatter(&format_json_log);
    sink->locked_backend()->set_open_handler(&open_json_log);
    sink->locked_backend()->set_close_handler(&close_json_log);
    sink->locked_backend()->auto_flush(true);
    logging::core::get()->add_sink(sink);

    boost::log::add_common_attributes();
}


} // namespace


namespace zmbt {
namespace appconfig {

void InitZmbt(int argc, char **argv)
{
    std::string default_log_level;
    std::string default_log_sink;

    if (const char* env_log_sink = std::getenv("ZMBT_LOG_SINK"))
    {
        default_log_sink = env_log_sink;
    }
    else
    {
        fs::path executable_path(argv[0]);
        default_log_sink = executable_path.filename().string();
    }

    if (const char* env_log_level = std::getenv("ZMBT_LOG_LEVEL"))
    {
        default_log_level = env_log_level;
    }
    else
    {
        default_log_level = "warning";
    }

    po::options_description desc("ZMBT test appconfig options");
    desc.add_options()
        ("zmbt_log_level", po::value<std::string>()->default_value(default_log_level), "[trace|debug|info|warning|error|fatal] log severity level")
        ("zmbt_log_sink", po::value<std::string>()->default_value(default_log_sink), "file name")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
    po::notify(vm);

    std::string zmbt_log_level = vm.at("zmbt_log_level").as<std::string>();
    std::string zmbt_log_sink  = vm.at("zmbt_log_sink").as<std::string>();
    init_logging(zmbt_log_level, zmbt_log_sink);
    // ZMBT_LOG_JSON(info) << "ZMBT initialized";
}

} // namespace appconfig
} // namespace zmbt


#endif
