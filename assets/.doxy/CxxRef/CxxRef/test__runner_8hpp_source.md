

# File test\_runner.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**test\_runner.hpp**](test__runner_8hpp.md)

[Go to the documentation of this file](test__runner_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_TEST_RUNNER_HPP_
#define ZMBT_MAPPING_TEST_RUNNER_HPP_

#include <memory>
#include "zmbt/core.hpp"

namespace zmbt {
namespace mapping {

class TestRunner
{
    class Impl;

    std::unique_ptr<Impl> impl_;

public:
    TestRunner(JsonNode const& model);
    void Run();
    ~TestRunner();
};

} // namespace mapping
} // namespace zmbt

#endif
```


