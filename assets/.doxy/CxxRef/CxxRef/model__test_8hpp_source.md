

# File model\_test.hpp

[**File List**](files.md) **>** [**api**](dir_dac1628bc1b9dc0b2ecee59f2dfa09f4.md) **>** [**model\_test.hpp**](model__test_8hpp.md)

[Go to the documentation of this file](model__test_8hpp.md)


```C++

#ifndef ZMBT_FIXTURE_MODEL_TEST_HPP_
#define ZMBT_FIXTURE_MODEL_TEST_HPP_

#include <array>
#include <tuple>

#include "ns.hpp"


namespace zmbt {

namespace api {

struct ModelTestFixture : public Environment {

    ModelTestFixture() = default;
    virtual ~ModelTestFixture() = default;
    ModelTestFixture(ModelTestFixture&&) = default;
    ModelTestFixture& operator=(ModelTestFixture&&) = default;

    ModelTestFixture(const ModelTestFixture&) = delete;
    ModelTestFixture& operator=(const ModelTestFixture&) = delete;

}; // class ModelTestFixture


}  // namespace api
}  // namespace zmbt

#endif  // ZMBT_FIXTURE_MODEL_TEST_HPP_
```


