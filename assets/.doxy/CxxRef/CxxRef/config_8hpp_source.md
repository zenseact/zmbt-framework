

# File config.hpp

[**File List**](files.md) **>** [**application**](dir_0cc19fbf7340471280b165ed90304d9a.md) **>** [**config.hpp**](config_8hpp.md)

[Go to the documentation of this file](config_8hpp.md)


```C++

#ifndef ZMBT_APPLICATION_CONFIG_HPP_
#define ZMBT_APPLICATION_CONFIG_HPP_


#include <boost/json.hpp>
#include <functional>
#include <memory>

#include "test_failure.hpp"


namespace zmbt {

class Config final
{
public:
    using FailureHandler = std::function<void(boost::json::value const&)>;

    Config();

    Config(Config const&) = default;
    Config(Config &&) = default;
    Config& operator=(Config const&) = default;
    Config& operator=(Config &&) = default;

    ~Config() = default;

    Config& SetFailureHandler(FailureHandler const& fn);

    Config& ResetFailureHandler();

    Config& HandleTestFailure(boost::json::value const& diagnostics);

private:
    struct PersistentConfig
    {
        FailureHandler failure_handler {&zmbt::default_test_failure};
    };

    std::shared_ptr<PersistentConfig> config_;
};

}

#endif // ZMBT_APPLICATION_CONFIG_HPP_
```


