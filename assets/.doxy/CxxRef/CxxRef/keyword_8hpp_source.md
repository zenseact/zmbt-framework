

# File keyword.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**keyword.hpp**](keyword_8hpp.md)

[Go to the documentation of this file](keyword_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_HPP_
#define ZMBT_EXPR_KEYWORD_HPP_

#include <boost/json.hpp>

#include "zmbt/reflect.hpp"


namespace zmbt {
namespace lang {


enum class Keyword : std::uint64_t
{
    Undefined,

    Literal,

    Void,

    LazyToken,

    _Continue,

    _Resolve,

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

    Lshift,

    Rshift,

    Eq,

    Ne,

    Lt,

    Le,

    Gt,

    Ge,

    Near,

    Union,

    Intersect,

    Diff,

    SetEq,

    Subset,

    Superset,

    PSubset,

    PSuperset,

    In,

    NotIn,

    Ni,

    NotNi,

    Bool,

    Not,

    And,

    Or,

    If,

    Elif,

    Else,

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

    First,

    Last,

    Rand,

    RandInt,

    Sequence,

    Arange,

    Parse,

    Str,

    Re,

    Fmt,

    Capitalize,

    LowerCase,

    UpperCase,

    Card,

    Size,

    Sum,

    Prod,

    Avg,

    Slide,

    Stride,

    Chunks,

    Repeat,

    Cat,

    Push,

    At,

    Del,

    Lookup,

    Cast,

    Uncast,

    Fold,

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

    Op,

    Bind,

    Fn,

    Link,

    Get,

    EnvLoad,

    EnvStore,

    Any,

    All,

    Saturate,

    Pipe,

    Tuple,

    Fork,

    Flip,

    Dbg,

    Eval,

    Try,

    Kwrd,

    Prms,

    Q,

    D,

    Err,

    IsErr,

    Assert,

    Trace,

    PreProc,

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


