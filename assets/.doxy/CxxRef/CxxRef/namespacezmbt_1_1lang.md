

# Namespace zmbt::lang



[**Namespace List**](namespaces.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _Language implementation._
















## Classes

| Type | Name |
| ---: | :--- |
| class | [**Expression**](classzmbt_1_1lang_1_1Expression.md) <br> |
| struct | [**ExpressionGrammar**](structzmbt_1_1lang_1_1ExpressionGrammar.md) &lt;typename OutputIterator&gt;<br> |
| struct | [**KeywordGrammar**](structzmbt_1_1lang_1_1KeywordGrammar.md) <br>_Boost.Sirit.Qi Grammar for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._ |
| struct | [**KeywordSymbol**](structzmbt_1_1lang_1_1KeywordSymbol.md) <br>_Boost.Sirit.Qi Symbol for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._ |
| class | [**Operator**](classzmbt_1_1lang_1_1Operator.md) <br>_Signal transformation and comparison handler. Enables type erasure._  |
| struct | [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md) &lt;K&gt;<br> |
| struct | [**SignatureBinary**](structzmbt_1_1lang_1_1SignatureBinary.md) &lt;K&gt;<br>_Binary expression._  |
| struct | [**SignatureConst**](structzmbt_1_1lang_1_1SignatureConst.md) &lt;K&gt;<br>_Const expression._  |
| struct | [**SignatureDebug**](structzmbt_1_1lang_1_1SignatureDebug.md) <br> |
| struct | [**SignatureDecorate**](structzmbt_1_1lang_1_1SignatureDecorate.md) <br> |
| struct | [**SignatureError**](structzmbt_1_1lang_1_1SignatureError.md) <br> |
| struct | [**SignatureOverload**](structzmbt_1_1lang_1_1SignatureOverload.md) <br> |
| struct | [**SignatureTernary**](structzmbt_1_1lang_1_1SignatureTernary.md) &lt;K&gt;<br>_Ternary expression._  |
| struct | [**SignatureUnary**](structzmbt_1_1lang_1_1SignatureUnary.md) &lt;K&gt;<br>_Unary expression._  |
| struct | [**SignatureUndecorate**](structzmbt_1_1lang_1_1SignatureUndecorate.md) <br> |
| struct | [**SignatureVariadic**](structzmbt_1_1lang_1_1SignatureVariadic.md) &lt;K&gt;<br>_Variadic expression._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Keyword**](#enum-keyword)  <br>_ZMBT_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _keyword._ |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**tag\_invoke**](#function-tag_invoke) (boost::json::value\_from\_tag const &, boost::json::value &, [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & t) <br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**tag\_invoke**](#function-tag_invoke) (boost::json::value\_to\_tag&lt; [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) &gt; const &, boost::json::value const &) <br> |




























## Public Types Documentation




### enum Keyword 

_ZMBT_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _keyword._
```C++
enum zmbt::lang::Keyword {
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
```




<hr>
## Public Functions Documentation




### function tag\_invoke 

```C++
void zmbt::lang::tag_invoke (
    boost::json::value_from_tag const &,
    boost::json::value &,
    Keyword const & t
) 
```




<hr>



### function tag\_invoke 

```C++
Keyword zmbt::lang::tag_invoke (
    boost::json::value_to_tag< Keyword > const &,
    boost::json::value const &
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/api_signatures.hpp`

