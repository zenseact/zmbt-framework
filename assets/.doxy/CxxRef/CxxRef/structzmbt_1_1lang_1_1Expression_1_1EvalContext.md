

# Struct zmbt::lang::Expression::EvalContext



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Expression**](classzmbt_1_1lang_1_1Expression.md) **>** [**EvalContext**](structzmbt_1_1lang_1_1Expression_1_1EvalContext.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation context._

* `#include <expression.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::uint64\_t const | [**depth**](#variable-depth)  <br>_Evaluation stack depth._  |
|  [**EvalLog**](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md) | [**log**](#variable-log)  <br>_Evaluation log._  |
|  [**Operator**](classzmbt_1_1lang_1_1Operator.md) | [**op**](#variable-op)  <br>[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _._ |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**EvalContext**](structzmbt_1_1lang_1_1Expression_1_1EvalContext.md) | [**operator++**](#function-operator) (int) const<br>_Copy context with depth increment._  |




























## Public Attributes Documentation




### variable depth 

_Evaluation stack depth._ 
```C++
std::uint64_t const zmbt::lang::Expression::EvalContext::depth;
```




<hr>



### variable log 

_Evaluation log._ 
```C++
EvalLog zmbt::lang::Expression::EvalContext::log;
```




<hr>



### variable op 

[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _._
```C++
Operator zmbt::lang::Expression::EvalContext::op;
```




<hr>
## Public Functions Documentation




### function operator++ 

_Copy context with depth increment._ 
```C++
EvalContext zmbt::lang::Expression::EvalContext::operator++ (
    int
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression.hpp`

