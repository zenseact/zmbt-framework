

# File config.hpp

[**File List**](files.md) **>** [**application**](dir_0cc19fbf7340471280b165ed90304d9a.md) **>** [**config.hpp**](config_8hpp.md)

[Go to the documentation of this file](config_8hpp.md)


```C++

#ifndef ZMBT_APPLICATION_CONFIG_HPP_
#define ZMBT_APPLICATION_CONFIG_HPP_


#include <boost/json.hpp>
#include <atomic>
#include <functional>
#include <memory>
#include <random>

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

    Config& SetRngSeed(std::uint64_t const seed);

    Config& ResetRng();

    std::mt19937& Rng();

    Config& SetFailureHandler(FailureHandler const& fn);


    Config& ResetFailureHandler();

    Config& HandleTestFailure(boost::json::value const& diagnostics);

    std::uint64_t RngSeed() const;

private:
    struct PersistentConfig
    {
        std::uint64_t rng_seed{0x6a09e667f3bcc909ULL};
        std::atomic<std::uint64_t> rng_counter{0};
        std::atomic<std::uint64_t> rng_epoch{0};
        FailureHandler failure_handler {&zmbt::default_test_failure};
    };

    std::shared_ptr<PersistentConfig> config_;
};

}

#endif // ZMBT_APPLICATION_CONFIG_HPP_
```


