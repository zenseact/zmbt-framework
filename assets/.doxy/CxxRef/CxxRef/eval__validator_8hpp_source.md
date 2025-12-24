

# File eval\_validator.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_validator.hpp**](eval__validator_8hpp.md)

[Go to the documentation of this file](eval__validator_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EVAL_VALIDATOR_HPP_
#define ZMBT_EXPR_EVAL_VALIDATOR_HPP_

#include "expression.hpp"
#include "keyword.hpp"


namespace zmbt {
namespace lang {

template <Keyword K>
struct EvalValidator
{

    EvalValidator(ExpressionView const& lhs, ExpressionView const& rhs) {}

    bool is_invalid() const
    {
        return false;
    }

    Expression status() const
    {
        return Expression();
    }
};

}  // namespace lang
}  // namespace zmbt

#endif
```


