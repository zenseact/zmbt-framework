

# File test\_failure.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**test\_failure.hpp**](test__failure_8hpp.md)

[Go to the documentation of this file](test__failure_8hpp.md)


```C++

#ifndef ZMBT_MODEL_TEST_FAILURE_HPP_
#define ZMBT_MODEL_TEST_FAILURE_HPP_

#include <sstream>

#include <boost/json.hpp>

#include "zmbt/core/json_pretty_print.hpp"

namespace zmbt {

void format_failure_report(std::ostream& os, boost::json::value const& report);

void default_test_failure(boost::json::value const& report);

}

#endif
```


