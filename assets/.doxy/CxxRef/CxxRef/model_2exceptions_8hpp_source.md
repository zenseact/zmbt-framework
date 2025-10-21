

# File exceptions.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**exceptions.hpp**](model_2exceptions_8hpp.md)

[Go to the documentation of this file](model_2exceptions_8hpp.md)


```C++

#ifndef ZMBT_MODEL_EXCEPTIONS_HPP_
#define ZMBT_MODEL_EXCEPTIONS_HPP_

#include "zmbt/core/exceptions.hpp"

namespace zmbt {


struct test_assertion_failure : public base_error {
    using base_error::base_error;
};

struct model_error : public base_error {
    using base_error::base_error;
};


}  // namespace zmbt

#endif
```


