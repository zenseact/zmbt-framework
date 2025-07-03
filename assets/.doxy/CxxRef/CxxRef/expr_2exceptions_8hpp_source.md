

# File exceptions.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**exceptions.hpp**](expr_2exceptions_8hpp.md)

[Go to the documentation of this file](expr_2exceptions_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EXCEPTIONS_HPP_
#define ZMBT_EXPR_EXCEPTIONS_HPP_

#include "zmbt/core/exceptions.hpp"

namespace zmbt {

struct expression_error : public base_error {
    using base_error::base_error;
};

struct expression_not_implemented : public expression_error {
    using expression_error::expression_error;
};

}  // namespace zmbt

#endif
```


