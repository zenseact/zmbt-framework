

# Namespace zmbt::lang



[**Namespace List**](namespaces.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _Language implementation._














## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**attr**](namespacezmbt_1_1lang_1_1attr.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) <br> |
| class | [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) <br> |
| struct | [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation context._ |
| struct | [**EvalImpl**](structzmbt_1_1lang_1_1EvalImpl.md) &lt;K&gt;<br> |
| class | [**EvalImplBase**](classzmbt_1_1lang_1_1EvalImplBase.md) &lt;K, class Validator&gt;<br> |
| struct | [**EvalLog**](structzmbt_1_1lang_1_1EvalLog.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation log._ |
| class | [**EvalParams**](classzmbt_1_1lang_1_1EvalParams.md) <br> |
| struct | [**EvalValidator**](structzmbt_1_1lang_1_1EvalValidator.md) &lt;K&gt;<br> |
| class | [**Expression**](classzmbt_1_1lang_1_1Expression.md) <br> |
| struct | [**ExpressionGrammar**](structzmbt_1_1lang_1_1ExpressionGrammar.md) &lt;typename OutputIterator&gt;<br> |
| class | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) <br> |
| struct | [**FixedEvalParams**](structzmbt_1_1lang_1_1FixedEvalParams.md) &lt;K&gt;<br> |
| class | [**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) <br>_Global Storage env for_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _side effects._ |
| struct | [**KeywordGrammar**](structzmbt_1_1lang_1_1KeywordGrammar.md) <br>_Boost.Sirit.Qi Grammar for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._ |
| struct | [**KeywordInfo**](structzmbt_1_1lang_1_1KeywordInfo.md) &lt;k&gt;<br> |
| struct | [**KeywordSymbol**](structzmbt_1_1lang_1_1KeywordSymbol.md) <br>_Boost.Sirit.Qi Symbol for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._ |
| class | [**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) <br> |
| class | [**Operator**](classzmbt_1_1lang_1_1Operator.md) <br>_Signal transformation and comparison handler. Enables type erasure._  |
| struct | [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md) &lt;K&gt;<br> |
| struct | [**SignatureBinary**](structzmbt_1_1lang_1_1SignatureBinary.md) &lt;K&gt;<br>_Binary expression._  |
| struct | [**SignatureBinary&lt; Keyword::PreProc &gt;**](structzmbt_1_1lang_1_1SignatureBinary_3_01Keyword_1_1PreProc_01_4.md) &lt;&gt;<br> |
| struct | [**SignatureCast**](structzmbt_1_1lang_1_1SignatureCast.md) <br> |
| struct | [**SignatureConst**](structzmbt_1_1lang_1_1SignatureConst.md) &lt;K&gt;<br>_Const expression._  |
| struct | [**SignatureErr**](structzmbt_1_1lang_1_1SignatureErr.md) <br> |
| struct | [**SignatureOp**](structzmbt_1_1lang_1_1SignatureOp.md) <br> |
| struct | [**SignatureUnary**](structzmbt_1_1lang_1_1SignatureUnary.md) &lt;K&gt;<br>_Unary expression._  |
| struct | [**SignatureUncast**](structzmbt_1_1lang_1_1SignatureUncast.md) <br> |
| struct | [**SignatureVariadic**](structzmbt_1_1lang_1_1SignatureVariadic.md) &lt;K&gt;<br>_Variadic expression._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint64\_t | [**Keyword**](#enum-keyword)  <br>_ZMBT_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _keyword._ |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr std::uint32\_t | [**attributes**](#function-attributes) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & k) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**dispatch\_eval**](#function-dispatch_eval) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & self, [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) context) <br> |
|  boost::json::string\_view | [**keyword\_to\_str**](#function-keyword_to_str) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const &) <br> |
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
```




<hr>
## Public Functions Documentation




### function attributes 

```C++
constexpr std::uint32_t zmbt::lang::attributes (
    Keyword const & k
) 
```




<hr>



### function dispatch\_eval 

```C++
template<Keyword K>
Expression zmbt::lang::dispatch_eval (
    ExpressionView const & self,
    ExpressionView const & x,
    EvalContext context
) 
```




<hr>



### function keyword\_to\_str 

```C++
boost::json::string_view zmbt::lang::keyword_to_str (
    Keyword const &
) 
```




<hr>



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

