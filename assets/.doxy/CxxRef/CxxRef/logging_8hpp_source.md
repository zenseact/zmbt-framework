

# File logging.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**logging.hpp**](logging_8hpp.md)

[Go to the documentation of this file](logging_8hpp.md)


```C++

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

    static void open_json(const std::string& filename = "");

    static void set_max_level(Level const max_level);

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
```


