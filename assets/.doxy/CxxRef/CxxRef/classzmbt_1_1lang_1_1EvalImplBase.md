

# Class zmbt::lang::EvalImplBase

**template &lt;[**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) K, class Validator&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**EvalImplBase**](classzmbt_1_1lang_1_1EvalImplBase.md)








Inherits the following classes: [zmbt::lang::FixedEvalParams](structzmbt_1_1lang_1_1FixedEvalParams.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EvalImplBase**](#function-evalimplbase) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & e, [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) <br> |
|  [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) | [**curr\_ctx**](#function-curr_ctx) () const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**operator()**](#function-operator) () <br> |


## Public Functions inherited from zmbt::lang::FixedEvalParams

See [zmbt::lang::FixedEvalParams](structzmbt_1_1lang_1_1FixedEvalParams.md)

| Type | Name |
| ---: | :--- |
|   | [**EvalParams**](structzmbt_1_1lang_1_1FixedEvalParams.md#function-evalparams) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & e, [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) <br> |


## Public Functions inherited from zmbt::lang::EvalParams

See [zmbt::lang::EvalParams](classzmbt_1_1lang_1_1EvalParams.md)

| Type | Name |
| ---: | :--- |
|   | [**EvalParams**](classzmbt_1_1lang_1_1EvalParams.md#function-evalparams) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & e, [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) <br> |
|  [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) | [**context**](classzmbt_1_1lang_1_1EvalParams.md#function-context) () const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & | [**lhs**](classzmbt_1_1lang_1_1EvalParams.md#function-lhs) () const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & | [**rhs**](classzmbt_1_1lang_1_1EvalParams.md#function-rhs) () const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & | [**self**](classzmbt_1_1lang_1_1EvalParams.md#function-self) () const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & | [**x**](classzmbt_1_1lang_1_1EvalParams.md#function-x) () const<br> |


## Public Functions inherited from zmbt::lang::KeywordInfo

See [zmbt::lang::KeywordInfo](structzmbt_1_1lang_1_1KeywordInfo.md)

| Type | Name |
| ---: | :--- |
|  constexpr std::uint32\_t | [**attributes**](structzmbt_1_1lang_1_1KeywordInfo.md#function-attributes) () <br> |
|  constexpr [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](structzmbt_1_1lang_1_1KeywordInfo.md#function-keyword) () <br> |








## Public Static Functions inherited from zmbt::lang::KeywordInfo

See [zmbt::lang::KeywordInfo](structzmbt_1_1lang_1_1KeywordInfo.md)

| Type | Name |
| ---: | :--- |
|  boost::json::string\_view | [**keyword\_to\_str**](structzmbt_1_1lang_1_1KeywordInfo.md#function-keyword_to_str) () <br> |


































































































## Public Functions Documentation




### function EvalImplBase 

```C++
inline zmbt::lang::EvalImplBase::EvalImplBase (
    ExpressionView const & e,
    ExpressionView const & x,
    EvalContext ctx
) 
```




<hr>



### function curr\_ctx 

```C++
inline EvalContext zmbt::lang::EvalImplBase::curr_ctx () const
```




<hr>



### function operator() 

```C++
inline Expression zmbt::lang::EvalImplBase::operator() () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/eval_impl.hpp`

