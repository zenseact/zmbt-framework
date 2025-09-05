/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/lockfree/spsc_queue.hpp>
#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

#include <zmbt/application/log.hpp>

namespace{
using Level = zmbt::Logger::Level;

static Level g_max_log_level = Level::INFO;
static bool g_notrim = true;
static bool g_pretty_print = false;


class AsyncJsonLogger {
public:

    static AsyncJsonLogger& instance()
    {
        static AsyncJsonLogger logger;  // change file name as needed
        return logger;
    }

    explicit AsyncJsonLogger()
        : queue_(1024)
    {
    }

    /// Attach logger to file sink
    void open(const std::string& filename)
    {
        if (!filename.empty()) {
            out_stream_.open(filename);
            out_stream_ << "[\n";
            running_ = true;
            worker_ = std::thread(&AsyncJsonLogger::async_write, this);
        } else {
            running_ = false;
        }
    }

    ~AsyncJsonLogger() {
        running_ = false;
        if (worker_.joinable()) worker_.join();
        out_stream_ << R"({"messages lost":)" << lost_message_count_ << "}\n]\n";
        out_stream_.flush();
        out_stream_.close();

        ZMBT_DEBUG;
    }

    void push(boost::json::object const& record) {

        std::lock_guard<std::mutex> lock(mutex_);
        if (!queue_.push(record)) {
            ++lost_message_count_;
        }
    }


private:

    std::mutex mutex_{};
    std::size_t lost_message_count_{0};
    std::atomic<bool> running_{false};
    boost::json::serializer serializer_{};
    std::ofstream out_stream_;
    std::thread worker_;
    boost::lockfree::spsc_queue<boost::json::object> queue_;

    void async_write() {
        boost::json::object record;
        while (running_ || !queue_.empty()) {
            while (queue_.pop(record)) {
                serializer_.reset(&record);
                while (!serializer_.done()) {
                    char buf[4000];
                    out_stream_ << serializer_.read(buf);
                }
                out_stream_ << ",\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
};



}

namespace zmbt {

void Logger::open_json(const std::string& filename)
{
   AsyncJsonLogger::instance().open(filename);
}

void Logger::set_max_level(Level const max_level)
{
    g_max_log_level = max_level;
}

void Logger::set_notrim(bool const notrim)
{
    g_notrim = notrim;
}

bool Logger::is_notrim_enabled()
{
    return g_notrim;
}

Level Logger::max_level()
{
    return g_max_log_level;
}


void Logger::set_pretty_print(bool const pretty_print)
{
    g_pretty_print = pretty_print;
}

bool Logger::is_pretty_print_enabled()
{
    return g_pretty_print;
}

Logger::Logger()
{
   set_timestamp();
}

Logger& Logger::WithOutput(Logger::Output const output)
{
    output_ = output;
    return *this;
}

Logger& Logger::WithLevel(Level const level)
{
    level_ = level;
    return *this;
}

Logger& Logger::WithSrcLoc(boost::json::string_view const src_loc)
{
    src_loc_ = src_loc;
    return *this;
}


Logger& Logger::operator<<(boost::json::value const& value)
{
    payload_cache_.emplace_back(value);
    return *this;
}


Logger::~Logger()
{
    if (payload_cache_.empty()) return;
    if (level_ > g_max_log_level) return;

    auto const level = boost::json::value_from(level_).as_string();
    boost::json::string_view const timestamp{timestamp_};

    if (JSON & output_)
    {
        boost::json::value const& payload = payload_cache_.size() == 1 ? payload_cache_.front() : payload_cache_;
        AsyncJsonLogger::instance().push({
            {"timestamp", timestamp },
            {"level" , level  },
            {"src_loc"  , src_loc_  },
            {"payload"  , payload   },
        });
    }
    if (STDERR & output_)
    {
        std::stringstream ss;
        ss << timestamp
        << ' ' << level.c_str()
        << ' ' << src_loc_
        << ' ';
        for (auto const& record: payload_cache_)
        {
            if (auto const as_string =record.if_string())
            {
                ss << as_string->c_str();
            }
            else
            {
                ss << record;
            }
        }
        std::cerr << ss.str() << '\n';
    }
}

// ISO8601 + nanoseconds
void Logger::set_timestamp()
{

    using namespace std::chrono;
    std::memset(timestamp_, 0, sizeof(timestamp_));
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    auto duration = now.time_since_epoch();

    auto nanos = duration_cast<nanoseconds>(duration).count() % 1'000'000'000;

    // Convert to UTC time
    std::tm* tm_ptr = std::gmtime(&time);  // UTC version

    if (tm_ptr) {
        // Format with milliseconds (e.g. 2025-07-08T18:12:45.123Z)
        std::snprintf(timestamp_, sizeof(timestamp_),
                    "%04d-%02d-%02dT%02d:%02d:%02d.%09lldZ",
                    tm_ptr->tm_year + 1900,
                    tm_ptr->tm_mon + 1,
                    tm_ptr->tm_mday,
                    tm_ptr->tm_hour,
                    tm_ptr->tm_min,
                    tm_ptr->tm_sec,
                    static_cast<long long>(nanos));  // or use nanos for .%09lld
    }
}

} // namespace zmbt

