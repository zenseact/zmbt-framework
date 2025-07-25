

# File ns.hpp

[**File List**](files.md) **>** [**api**](dir_dac1628bc1b9dc0b2ecee59f2dfa09f4.md) **>** [**ns.hpp**](ns_8hpp.md)

[Go to the documentation of this file](ns_8hpp.md)


```C++

#ifndef ZMBT_API_NS_HPP_
#define ZMBT_API_NS_HPP_

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/model.hpp"
#include "zmbt/decor.hpp"
#include "zmbt/mapping.hpp"

namespace zmbt {
namespace api {

using zmbt::Param;
using zmbt::type;

using zmbt::Environment;
using zmbt::InterfaceRecord;

using lang::Expression;

namespace decor = zmbt::decor;
namespace expr = zmbt::expr;

using zmbt::mapping::SignalMapping;

struct ModelTestFixture;

#ifdef _DOXYGEN_

class Param {};
constexpr auto  type;
class Environment {};
auto InterfaceRecord(...);
class precise {};
class Expression {};
class SignalMapping {};

namespace decor {}

namespace expr {}

#endif

}
}

#endif
```


