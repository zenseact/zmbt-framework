/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_LOGGING_HPP_
#define ZMBT_LOGGING_HPP_

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <zmbt/core/preprocessor.hpp>


#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>


#ifdef NDEBUG
    #define ZMBT_DEBUG (std::cerr << ZMBT_CUR_LOC << ": ")
#else
    #define ZMBT_DEBUG ::zmbt::StubLogger()
#endif

#define ZMBT_LOG(lvl) ::zmbt::Logger().WithSrcLoc(ZMBT_CUR_LOC).WithLevel(::zmbt::Logger::lvl)
#define ZMBT_LOG_CERR(lvl) ZMBT_LOG(lvl).WithOutput(::zmbt::Logger::STDERR)
#define ZMBT_LOG_JSON(lvl) ZMBT_LOG(lvl).WithOutput(::zmbt::Logger::JSON)

namespace zmbt {

#ifndef NDEBUG
struct StubLogger
{
    StubLogger& operator<<(boost::json::value const&)
    {
        return *this;
    }
};
#endif

class Logger {
  public:

    enum Output
    {
        NIL = 0,
        STDERR = 1 << 0,
        JSON   = 1 << 1,
    };

    enum Level
    {
        FATAL,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    };

    /// Attach logger to file sink ("" means stdout)
    static void open_json(const std::string& filename = "");

    static void set_max_level(Level const max_level);
    static void set_notrim(bool const trim_line);

    static bool get_notrim();
    static Level get_max_level();


    Logger();

    Logger& WithLevel(Level const level);
    Logger& WithOutput(Output const output);
    Logger& WithSrcLoc(boost::json::string_view const src_loc);

    Logger& operator<<(boost::json::value const& value);

    ~Logger();

  private:

    Level level_{INFO};
    int output_{STDERR | JSON};
    boost::json::string_view src_loc_{"unknown"};
    char timestamp_[64];
    boost::json::array payload_cache_;

    void set_timestamp();

    BOOST_DESCRIBE_NESTED_ENUM(Level,
        FATAL,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    )
};


}  // namespace zmbt

#endif
