/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_APPLICATION_INIT_HPP_
#define ZMBT_APPLICATION_INIT_HPP_

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/json.hpp>

#include <boost/program_options.hpp>

#include <zmbt/application.hpp>

#include <zmbt/reflect.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;


namespace zmbt {

void InitZmbt(int argc, char **argv)
{
    fs::path executable_path(argv[0]);

    std::string default_log_level = "INFO";
    std::string default_log_sink = executable_path.filename().string();
    bool default_log_prettify = false;
    bool default_log_notrim = false;
    std::uint64_t default_rng_seed = 0;



#ifndef _MSC_VER
    if (const char* env_log_sink = std::getenv("ZMBT_LOG_SINK"))
    {
        default_log_sink = env_log_sink;
    }
    if (const char* env_log_level = std::getenv("ZMBT_LOG_LEVEL"))
    {
        default_log_level = env_log_level;
    }
    if (std::getenv("ZMBT_LOG_PRETTIFY"))
    {
        default_log_prettify = true;
    }
    if (std::getenv("ZMBT_LOG_NOTRIM"))
    {
        default_log_notrim = true;
    }
#endif

    po::options_description desc("ZMBT test application options");
    desc.add_options()
        ("zmbt_log_level"   , po::value<std::string>()->default_value(default_log_level) , "[trace|debug|info|warning|error|fatal] log level level")
        ("zmbt_log_sink"    , po::value<std::string>()->default_value(default_log_sink)  , "file name")
        ("zmbt_log_notrim"  , po::bool_switch()->default_value(default_log_notrim)       , "turn off line trimming in terminal")
        ("zmbt_log_prettify", po::bool_switch()->default_value(default_log_prettify)     , "pretty print JSON values in failure reports")
        ("zmbt_seed"        , po::value<std::uint64_t>()->default_value(default_rng_seed), "seed for random generators; default zero is for system-time based seed")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
    po::notify(vm);

    std::string   const zmbt_log_level    = vm.at("zmbt_log_level")     .as<std::string>();
    std::string   const zmbt_log_sink     = vm.at("zmbt_log_sink")      .as<std::string>();
    bool          const zmbt_log_prettify = vm.at("zmbt_log_prettify")  .as<bool>();
    bool          const zmbt_log_notrim   = vm.at("zmbt_log_notrim")    .as<bool>();
    std::uint64_t       zmbt_seed         = vm.at("zmbt_seed")          .as<std::uint64_t>();

    auto const maybe_level = boost::json::try_value_to<Logger::Level>(zmbt_log_level.c_str());
    Logger::Level parsed_log_level = maybe_level.has_value() ? maybe_level.value() : Logger::Level::INFO;

    Logger::open_json(zmbt_log_sink + ".json");
    Logger::set_max_level(parsed_log_level);
    if (zmbt_seed == 0)
    {
        zmbt_seed = static_cast<std::uint64_t>(
            std::chrono::system_clock::now().time_since_epoch().count()
        );
    }
    Config().SetRngSeed(zmbt_seed);
    ZMBT_LOG_JSON(INFO) << boost::json::object{
        {"ZMBT TEST SUITE", {
            {"exec", argv[0]},
            {"seed", zmbt_seed},
            {"args", boost::json::array(argv+1, argv+argc)},
        }}
    };
    Logger::set_notrim(zmbt_log_notrim);
    Logger::set_pretty_print(zmbt_log_prettify);
}

} // namespace zmbt


#endif
