

# Struct zmbt::lang::EvalLog



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**EvalLog**](structzmbt_1_1lang_1_1EvalLog.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation log._

* `#include <eval_log.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; boost::json::array &gt; | [**stack**](#variable-stack)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EvalLog**](#function-evallog) () = default<br>_Default instance with null log stack._  |
|  void | [**push**](#function-push) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & expr, [**Expression**](classzmbt_1_1lang_1_1Expression.md) const & x, [**Expression**](classzmbt_1_1lang_1_1Expression.md) const & result, std::uint64\_t const depth) const<br>_Push record to log stack._  |
|  boost::json::string | [**str**](#function-str) (int const indent=0) const<br>_Stringify log._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**format**](#function-format) (std::ostream & os, boost::json::array const & log, int const indent=0) <br> |
|  [**EvalLog**](structzmbt_1_1lang_1_1EvalLog.md) | [**make**](#function-make) () <br>_Make non-empty_ [_**EvalLog**_](structzmbt_1_1lang_1_1EvalLog.md) _._ |


























## Public Attributes Documentation




### variable stack 

```C++
std::shared_ptr<boost::json::array> zmbt::lang::EvalLog::stack;
```




<hr>
## Public Functions Documentation




### function EvalLog 

_Default instance with null log stack._ 
```C++
zmbt::lang::EvalLog::EvalLog () = default
```




<hr>



### function push 

_Push record to log stack._ 
```C++
void zmbt::lang::EvalLog::push (
    Expression const & expr,
    Expression const & x,
    Expression const & result,
    std::uint64_t const depth
) const
```




<hr>



### function str 

_Stringify log._ 
```C++
boost::json::string zmbt::lang::EvalLog::str (
    int const indent=0
) const
```




<hr>
## Public Static Functions Documentation




### function format 

```C++
static void zmbt::lang::EvalLog::format (
    std::ostream & os,
    boost::json::array const & log,
    int const indent=0
) 
```




<hr>



### function make 

_Make non-empty_ [_**EvalLog**_](structzmbt_1_1lang_1_1EvalLog.md) _._
```C++
static EvalLog zmbt::lang::EvalLog::make () 
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```C++
std::ostream & zmbt::lang::EvalLog::operator<< (
    std::ostream & os,
    EvalLog const & log
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/eval_log.hpp`

