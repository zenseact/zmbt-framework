

# Class zmbt::lang::LazyParam



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LazyParam**](#function-lazyparam-19) () <br> |
|   | [**LazyParam**](#function-lazyparam-29) (std::function&lt; V()&gt; getter) <br> |
|   | [**LazyParam**](#function-lazyparam-39) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md), [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md)) <br> |
|   | [**LazyParam**](#function-lazyparam-49) (V && v) <br> |
|   | [**LazyParam**](#function-lazyparam-59) (V const & v) <br> |
|   | [**LazyParam**](#function-lazyparam-69) (std::reference\_wrapper&lt; V const &gt; v) <br> |
|   | [**LazyParam**](#function-lazyparam-79) (T const & v) <br> |
|   | [**LazyParam**](#function-lazyparam-89) ([**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) const &) = default<br> |
|   | [**LazyParam**](#function-lazyparam-99) ([**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) &&) = default<br> |
|   | [**operator V**](#function-operator-v) () const<br> |
|  V | [**operator()**](#function-operator) () const<br> |
|  [**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) & | [**operator=**](#function-operator_1) ([**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) const &) = default<br> |
|  [**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) & | [**operator=**](#function-operator_2) ([**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md) &&) = default<br> |
|   | [**~LazyParam**](#function-lazyparam) () = default<br> |




























## Public Types Documentation




### typedef V 

```C++
using zmbt::lang::LazyParam::V =  boost::json::value;
```




<hr>
## Public Functions Documentation




### function LazyParam [1/9]

```C++
zmbt::lang::LazyParam::LazyParam () 
```




<hr>



### function LazyParam [2/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    std::function< V()> getter
) 
```




<hr>



### function LazyParam [3/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    ExpressionView,
    EvalContext
) 
```




<hr>



### function LazyParam [4/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    V && v
) 
```




<hr>



### function LazyParam [5/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    V const & v
) 
```




<hr>



### function LazyParam [6/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    std::reference_wrapper< V const > v
) 
```




<hr>



### function LazyParam [7/9]

```C++
template<class T>
inline zmbt::lang::LazyParam::LazyParam (
    T const & v
) 
```




<hr>



### function LazyParam [8/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    LazyParam const &
) = default
```




<hr>



### function LazyParam [9/9]

```C++
zmbt::lang::LazyParam::LazyParam (
    LazyParam &&
) = default
```




<hr>



### function operator V 

```C++
zmbt::lang::LazyParam::operator V () const
```




<hr>



### function operator() 

```C++
V zmbt::lang::LazyParam::operator() () const
```




<hr>



### function operator= 

```C++
LazyParam & zmbt::lang::LazyParam::operator= (
    LazyParam const &
) = default
```




<hr>



### function operator= 

```C++
LazyParam & zmbt::lang::LazyParam::operator= (
    LazyParam &&
) = default
```




<hr>



### function ~LazyParam 

```C++
zmbt::lang::LazyParam::~LazyParam () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/lazy_param.hpp`

