

# File keyword.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**keyword.hpp**](keyword_8hpp.md)

[Go to the documentation of this file](keyword_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_HPP_
#define ZMBT_EXPR_KEYWORD_HPP_

#include <boost/json.hpp>

#include "zmbt/reflect.hpp"

#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {
namespace lang {


enum class Keyword
{
    Undefined,

    Literal,

    Void,

    Noop,

    Null,

    True,

    False,

    Pi,

    E,

    Inf,

    Eps,

    NaN,

    Thread,

    Sin,

    Cos,

    Tan,

    Asin,

    Acos,

    Atan,

    Sinh,

    Cosh,

    Tanh,

    Asinh,

    Acosh,

    Atanh,

    Exp,

    Erf,

    Erfc,

    Gamma,

    Abs,

    Ceil,

    Floor,

    Round,

    Sqrt,

    Sign,

    Neg,

    Add,

    Sub,

    Mul,

    Div,

    Pow,

    Log,

    Mod,

    Quot,

    BitNot,

    BitAnd,

    BitOr,

    BitXor,

    BitLshift,

    BitRshift,

    Eq,

    Ne,

    Lt,

    Le,

    Gt,

    Ge,

    Approx,

    Union,

    Intersect,

    Diff,

    SetEq,

    Subset,

    Superset,

    ProperSubset,

    ProperSuperset,

    In,

    NotIn,

    Ni,

    NotNi,

    Bool,

    Not,

    And,

    Or,

    Id,

    Transp,

    Cartesian,

    Reverse,

    Uniques,

    Items,

    Keys,

    Values,

    Enumerate,

    Flatten,

    ToList,

    Arange,

    Parse,

    Serialize,

    Re,

    Format,

    Card,

    Size,

    Sum,

    Prod,

    Avg,

    Slide,

    Stride,

    Chunks,

    Repeat,

    Concat,

    Push,

    At,

    Delete,

    Lookup,

    C,

    Decorate,

    Undecorate,

    Reduce,

    Map,

    Filter,

    Count,

    Each,

    Sort,

    Find,

    FindPtr,

    FindIdx,

    Min,

    Max,

    Argmin,

    Argmax,

    Recur,

    Unfold,

    Overload,

    Bind,

    Any,

    All,

    Saturate,

    Compose,

    Fork,

    Flip,

    Debug,

    Eval,

    Try,

    TryCatch,

    Default,

    Error,

    _count
};

namespace detail
{
bool isBinary(Keyword const& k);
bool isBoolean(Keyword const& k);
} // namespace detail


} // namespace lang
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_HPP_
```


