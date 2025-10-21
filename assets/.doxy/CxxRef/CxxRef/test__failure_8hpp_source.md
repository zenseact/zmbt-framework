

# File test\_failure.hpp

[**File List**](files.md) **>** [**application**](dir_0cc19fbf7340471280b165ed90304d9a.md) **>** [**test\_failure.hpp**](test__failure_8hpp.md)

[Go to the documentation of this file](test__failure_8hpp.md)


```C++

#ifndef ZMBT_APPLICATION_TEST_FAILURE_HPP_
#define ZMBT_APPLICATION_TEST_FAILURE_HPP_

#include <sstream>

#include <boost/json.hpp>

#include "zmbt/core/json_pretty_print.hpp"

namespace zmbt {

void format_failure_report(std::ostream& os, boost::json::value const& report);

void default_test_failure(boost::json::value const& report);

}

#endif
```


