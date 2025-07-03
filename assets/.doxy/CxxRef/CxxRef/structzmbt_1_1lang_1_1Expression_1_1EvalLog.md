

# Struct zmbt::lang::Expression::EvalLog



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Expression**](classzmbt_1_1lang_1_1Expression.md) **>** [**EvalLog**](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md)



[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation log._

* `#include <expression.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; boost::json::array &gt; | [**stack**](#variable-stack)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EvalLog**](#function-evallog) () = default<br>_Default instance with null log stack._  |
|  void | [**push**](#function-push) (boost::json::value const & expr, boost::json::value const & x, boost::json::value const & result, std::uint64\_t const depth) const<br>_Push record to log stack._  |
|  std::string | [**str**](#function-str) () const<br>_Stringify log._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**format**](#function-format) (std::ostream & os, boost::json::array const & log, int const indent=0) <br> |
|  [**EvalLog**](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md) | [**make**](#function-make) () <br>_Make non-empty_ [_**EvalLog**_](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md) _._ |


























## Public Attributes Documentation




### variable stack 

```C++
std::shared_ptr<boost::json::array> zmbt::lang::Expression::EvalLog::stack;
```




<hr>
## Public Functions Documentation




### function EvalLog 

_Default instance with null log stack._ 
```C++
zmbt::lang::Expression::EvalLog::EvalLog () = default
```




<hr>



### function push 

_Push record to log stack._ 
```C++
void zmbt::lang::Expression::EvalLog::push (
    boost::json::value const & expr,
    boost::json::value const & x,
    boost::json::value const & result,
    std::uint64_t const depth
) const
```




<hr>



### function str 

_Stringify log._ 
```C++
std::string zmbt::lang::Expression::EvalLog::str () const
```




<hr>
## Public Static Functions Documentation




### function format 

```C++
static void zmbt::lang::Expression::EvalLog::format (
    std::ostream & os,
    boost::json::array const & log,
    int const indent=0
) 
```




<hr>



### function make 

_Make non-empty_ [_**EvalLog**_](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md) _._
```C++
static EvalLog zmbt::lang::Expression::EvalLog::make () 
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```C++
std::ostream & zmbt::lang::Expression::EvalLog::operator<< (
    std::ostream & os,
    EvalLog const & log
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression.hpp`

