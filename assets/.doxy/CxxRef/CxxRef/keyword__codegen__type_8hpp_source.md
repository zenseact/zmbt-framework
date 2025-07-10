

# File keyword\_codegen\_type.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**keyword\_codegen\_type.hpp**](keyword__codegen__type_8hpp.md)

[Go to the documentation of this file](keyword__codegen__type_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_
#define ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_

#include <boost/json.hpp>
#include <tuple>

#include "keyword.hpp"

namespace zmbt {
namespace lang {
namespace detail {

bool isConst(Keyword const& k);
bool isHiOrd(Keyword const& k);
bool isVariadic(Keyword const& k);
bool isTernary(Keyword const& k);

enum class CodegenType
{
    None,
    Const,
    UnaryOp,
    BinaryOp,
    CodegenFn,
};

CodegenType getCodegenType(Keyword const& k);




} // namespace detail
} // namespace lang
} // namespace zmbt



#endif // ZMBT_EXPR_KEYWORD_CODEGEN_TYPE_HPP_
```


