

# Struct zmbt::lang::EvalContext



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation context._

* `#include <eval_context.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; std::deque&lt; boost::json::object &gt; &gt; | [**capture\_links**](#variable-capture_links)  <br> |
|  std::uint64\_t | [**depth**](#variable-depth)  <br>_Evaluation stack depth._  |
|  std::shared\_ptr&lt; std::map&lt; boost::json::string, [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &gt; &gt; | [**expr\_links**](#variable-expr_links)  <br> |
|  [**EvalLog**](structzmbt_1_1lang_1_1EvalLog.md) | [**log**](#variable-log)  <br>_Evaluation log._  |
|  [**Operator**](classzmbt_1_1lang_1_1Operator.md) | [**op**](#variable-op)  <br>[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _._ |
|  std::shared\_ptr&lt; boost::json::array &gt; | [**traces**](#variable-traces)  <br>_reference -&gt; value map_  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EvalContext**](#function-evalcontext) () <br> |
|  [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) | [**operator++**](#function-operator) () const<br>_Copy context with depth increment._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) | [**make**](#function-make) ([**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) <br> |


























## Public Attributes Documentation




### variable capture\_links 

```C++
std::shared_ptr<std::deque<boost::json::object> > zmbt::lang::EvalContext::capture_links;
```




<hr>



### variable depth 

_Evaluation stack depth._ 
```C++
std::uint64_t zmbt::lang::EvalContext::depth;
```




<hr>



### variable expr\_links 

```C++
std::shared_ptr<std::map<boost::json::string, ExpressionView> > zmbt::lang::EvalContext::expr_links;
```




<hr>



### variable log 

_Evaluation log._ 
```C++
EvalLog zmbt::lang::EvalContext::log;
```




<hr>



### variable op 

[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _._
```C++
Operator zmbt::lang::EvalContext::op;
```




<hr>



### variable traces 

_reference -&gt; value map_ 
```C++
std::shared_ptr<boost::json::array> zmbt::lang::EvalContext::traces;
```




<hr>
## Public Functions Documentation




### function EvalContext 

```C++
zmbt::lang::EvalContext::EvalContext () 
```




<hr>



### function operator++ 

_Copy context with depth increment._ 
```C++
EvalContext zmbt::lang::EvalContext::operator++ () const
```




<hr>
## Public Static Functions Documentation




### function make 

```C++
static EvalContext zmbt::lang::EvalContext::make (
    Operator const & op={}
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/eval_context.hpp`

