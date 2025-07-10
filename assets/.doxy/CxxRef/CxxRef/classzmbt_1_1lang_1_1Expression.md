

# Class zmbt::lang::Expression



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Expression**](classzmbt_1_1lang_1_1Expression.md)



[More...](#detailed-description)

* `#include <expression.hpp>`





Inherited by the following classes: [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md),  [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)










## Classes

| Type | Name |
| ---: | :--- |
| struct | [**EvalContext**](structzmbt_1_1lang_1_1Expression_1_1EvalContext.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation context._ |
| struct | [**EvalLog**](structzmbt_1_1lang_1_1Expression_1_1EvalLog.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation log._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Expression**](#function-expression-411) (std::initializer\_list&lt; boost::json::value\_ref &gt; items) <br> |
|   | [**Expression**](#function-expression-511) (boost::json::value const & expr) <br> |
|   | [**Expression**](#function-expression-611) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, boost::json::value const & params) <br> |
|   | [**Expression**](#function-expression-711) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword) <br> |
|   | [**Expression**](#function-expression-811) () <br> |
|   | [**Expression**](#function-expression-911) (T const & sample) <br> |
|   | [**Expression**](#function-expression-1011) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) <br> |
|   | [**Expression**](#function-expression-1111) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && o) <br> |
|  boost::json::value | [**eval**](#function-eval) (boost::json::value const & x=nullptr, [**EvalContext**](structzmbt_1_1lang_1_1Expression_1_1EvalContext.md) const & ctx={}) const<br>_Evaluate expression._  |
|  bool | [**has\_params**](#function-has_params) () const<br> |
|  bool | [**is**](#function-is) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const kwrd) const<br> |
|  bool | [**is\_boolean**](#function-is_boolean) () const<br> |
|  bool | [**is\_compose**](#function-is_compose) () const<br> |
|  bool | [**is\_const**](#function-is_const) () const<br> |
|  bool | [**is\_fork**](#function-is_fork) () const<br> |
|  bool | [**is\_hiord**](#function-is_hiord) () const<br> |
|  bool | [**is\_literal**](#function-is_literal) () const<br> |
|  bool | [**is\_nonempty\_composition**](#function-is_nonempty_composition) () const<br> |
|  bool | [**is\_nonempty\_fork**](#function-is_nonempty_fork) () const<br> |
|  bool | [**is\_noop**](#function-is_noop) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](#function-keyword) () const<br> |
|  std::string | [**keyword\_to\_str**](#function-keyword_to_str) () const<br> |
|  bool | [**match**](#function-match) (boost::json::value const & observed, [**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) const<br> |
|  bool | [**operator!=**](#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](#function-operator_2) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && o) <br> |
|  bool | [**operator==**](#function-operator_3) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  std::list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; | [**parameter\_list**](#function-parameter_list) () const<br> |
|  boost::json::value const & | [**params**](#function-params) () const<br> |
|  std::string | [**prettify**](#function-prettify) () const<br> |
|  std::string | [**serialize**](#function-serialize) () const<br> |
|  boost::json::value const & | [**subexpr**](#function-subexpr) () const<br> |
|  boost::json::value const & | [**underlying**](#function-underlying) () const<br> |
|   | [**~Expression**](#function-expression) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**asPredicate**](#function-aspredicate) (boost::json::value const & underlying) <br> |


























## Detailed Description


[**Expression**](classzmbt_1_1lang_1_1Expression.md) Language implementation class.




**See also:** [Expression Language documentation](/user-guide/expressions/). 



    
## Public Types Documentation




### typedef Keyword 

```C++
using zmbt::lang::Expression::Keyword =  lang::Keyword;
```




<hr>



### typedef V 

```C++
using zmbt::lang::Expression::V =  boost::json::value;
```




<hr>
## Public Functions Documentation




### function Expression [4/11]

```C++
zmbt::lang::Expression::Expression (
    std::initializer_list< boost::json::value_ref > items
) 
```




<hr>



### function Expression [5/11]

```C++
zmbt::lang::Expression::Expression (
    boost::json::value const & expr
) 
```




<hr>



### function Expression [6/11]

```C++
zmbt::lang::Expression::Expression (
    Keyword const & keyword,
    boost::json::value const & params
) 
```




<hr>



### function Expression [7/11]

```C++
explicit zmbt::lang::Expression::Expression (
    Keyword const & keyword
) 
```




<hr>



### function Expression [8/11]

```C++
zmbt::lang::Expression::Expression () 
```




<hr>



### function Expression [9/11]

```C++
template<class T>
inline zmbt::lang::Expression::Expression (
    T const & sample
) 
```




<hr>



### function Expression [10/11]

```C++
zmbt::lang::Expression::Expression (
    Expression const & o
) 
```




<hr>



### function Expression [11/11]

```C++
zmbt::lang::Expression::Expression (
    Expression && o
) 
```




<hr>



### function eval 

_Evaluate expression._ 
```C++
boost::json::value zmbt::lang::Expression::eval (
    boost::json::value const & x=nullptr,
    EvalContext const & ctx={}
) const
```





**Parameters:**


* `x` run-time argument 
* `config` evaluation config 



**Returns:**







        

<hr>



### function has\_params 

```C++
inline bool zmbt::lang::Expression::has_params () const
```




<hr>



### function is 

```C++
inline bool zmbt::lang::Expression::is (
    Keyword const kwrd
) const
```




<hr>



### function is\_boolean 

```C++
bool zmbt::lang::Expression::is_boolean () const
```




<hr>



### function is\_compose 

```C++
inline bool zmbt::lang::Expression::is_compose () const
```




<hr>



### function is\_const 

```C++
bool zmbt::lang::Expression::is_const () const
```




<hr>



### function is\_fork 

```C++
inline bool zmbt::lang::Expression::is_fork () const
```




<hr>



### function is\_hiord 

```C++
bool zmbt::lang::Expression::is_hiord () const
```




<hr>



### function is\_literal 

```C++
inline bool zmbt::lang::Expression::is_literal () const
```




<hr>



### function is\_nonempty\_composition 

```C++
inline bool zmbt::lang::Expression::is_nonempty_composition () const
```




<hr>



### function is\_nonempty\_fork 

```C++
inline bool zmbt::lang::Expression::is_nonempty_fork () const
```




<hr>



### function is\_noop 

```C++
inline bool zmbt::lang::Expression::is_noop () const
```




<hr>



### function keyword 

```C++
inline Keyword zmbt::lang::Expression::keyword () const
```




<hr>



### function keyword\_to\_str 

```C++
std::string zmbt::lang::Expression::keyword_to_str () const
```




<hr>



### function match 

```C++
bool zmbt::lang::Expression::match (
    boost::json::value const & observed,
    Operator const & op={}
) const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::lang::Expression::operator!= (
    Expression const & o
) const
```




<hr>



### function operator= 

```C++
Expression & zmbt::lang::Expression::operator= (
    Expression const & o
) 
```




<hr>



### function operator= 

```C++
Expression & zmbt::lang::Expression::operator= (
    Expression && o
) 
```




<hr>



### function operator== 

```C++
inline bool zmbt::lang::Expression::operator== (
    Expression const & o
) const
```




<hr>



### function parameter\_list 

```C++
std::list< Expression > zmbt::lang::Expression::parameter_list () const
```




<hr>



### function params 

```C++
inline boost::json::value const & zmbt::lang::Expression::params () const
```




<hr>



### function prettify 

```C++
std::string zmbt::lang::Expression::prettify () const
```




<hr>



### function serialize 

```C++
inline std::string zmbt::lang::Expression::serialize () const
```




<hr>



### function subexpr 

```C++
inline boost::json::value const & zmbt::lang::Expression::subexpr () const
```




<hr>



### function underlying 

```C++
inline boost::json::value const & zmbt::lang::Expression::underlying () const
```




<hr>



### function ~Expression 

```C++
zmbt::lang::Expression::~Expression () = default
```




<hr>
## Public Static Functions Documentation




### function asPredicate 

```C++
static Expression zmbt::lang::Expression::asPredicate (
    boost::json::value const & underlying
) 
```



Return const expressions as Eq(underlying), except for Noop, otherwise return Expression(underlying) unchanged 


        

<hr>## Friends Documentation





### friend operator& 

_Pack expression results into an array._ 
```C++
Expression zmbt::lang::Expression::operator& (
    Expression const & lhs,
    Expression const & rhs
) 
```





**See also:** [**zmbt::expr::Fork**](namespacezmbt_1_1expr.md#variable-fork). 



        

<hr>



### friend operator\* 

_Evaluate x to lhs expression._ 
```C++
V zmbt::lang::Expression::operator* (
    Expression const & expr,
    Expression const & x
) 
```



Equivalent to expr.eval(x). 


        

<hr>



### friend operator\* 

_Evaluate expression._ 
```C++
V zmbt::lang::Expression::operator* (
    Expression const & expr
) 
```



Equivalent to expr.eval(). 


        

<hr>



### friend operator\| 

_Compose expressions left-to-right._ 
```C++
Expression zmbt::lang::Expression::operator| (
    Expression const & lhs,
    Expression const & rhs
) 
```



Pipe functional expressions in composition, s.t. `a | b` is equivalent to `Compose(b, a)`. 

**See also:** [**zmbt::expr::Compose**](namespacezmbt_1_1expr.md#variable-compose) 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression.hpp`

