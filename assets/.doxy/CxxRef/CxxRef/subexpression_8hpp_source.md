

# File subexpression.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**subexpression.hpp**](subexpression_8hpp.md)

[Go to the documentation of this file](subexpression_8hpp.md)


```C++

#ifndef ZMBT_EXPR_SUBEXPRESSION_HPP_
#define ZMBT_EXPR_SUBEXPRESSION_HPP_

#include <ostream>

#include "expression.hpp"


namespace zmbt {
namespace lang {


class Expression::Subexpression : Expression
{
    EncodingView ev_;
public:
    Subexpression(EncodingView const& ev)
        : ev_{ev}
    {
    }
};

}  // namespace lang
}  // namespace zmbt

#endif
```


