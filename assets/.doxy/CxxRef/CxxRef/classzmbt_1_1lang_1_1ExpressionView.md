

# Class zmbt::lang::ExpressionView



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md)



[More...](#detailed-description)

* `#include <expression.hpp>`





Inherited by the following classes: [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)












## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionView**](#function-expressionview-15) () <br> |
|   | [**ExpressionView**](#function-expressionview-25) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const &) = default<br> |
|   | [**ExpressionView**](#function-expressionview-35) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &&) = default<br> |
|   | [**ExpressionView**](#function-expressionview-45) ([**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) v) <br> |
|   | [**ExpressionView**](#function-expressionview-55) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) enc) <br> |
|  boost::json::array const & | [**as\_array**](#function-as_array) () const<br> |
|  bool | [**as\_bool**](#function-as_bool) () const<br> |
|  boost::json::object const & | [**as\_object**](#function-as_object) () const<br> |
|  boost::json::string const & | [**as\_string**](#function-as_string) () const<br> |
|  boost::json::value const & | [**data**](#function-data) () const<br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**encoding\_view**](#function-encoding_view) () const<br> |
|  std::string | [**error\_id**](#function-error_id) () const<br> |
|  boost::json::value | [**eval**](#function-eval) (boost::json::value const & x=nullptr, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx={}) const<br>_Evaluate expression._  |
|  bool | [**eval\_as\_predicate**](#function-eval_as_predicate-12) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**Expression**](classzmbt_1_1lang_1_1Expression.md) & err\_sts, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  bool | [**eval\_as\_predicate**](#function-eval_as_predicate-22) (boost::json::value const & x, [**Expression**](classzmbt_1_1lang_1_1Expression.md) & err\_sts, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**eval\_e**](#function-eval_e) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br>_Evaluate expression._  |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**eval\_maybe\_predicate**](#function-eval_maybe_predicate) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  bool | [**has\_subexpr**](#function-has_subexpr) () const<br> |
|  boost::json::array const \* | [**if\_array**](#function-if_array) () const<br> |
|  bool const \* | [**if\_bool**](#function-if_bool) () const<br> |
|  boost::json::object const \* | [**if\_object**](#function-if_object) () const<br> |
|  boost::json::string const \* | [**if\_string**](#function-if_string) () const<br> |
|  bool | [**is**](#function-is) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const kwrd) const<br> |
|  bool | [**is\_boolean**](#function-is_boolean) () const<br> |
|  bool | [**is\_complete\_flip**](#function-is_complete_flip) () const<br> |
|  bool | [**is\_compose**](#function-is_compose) () const<br> |
|  bool | [**is\_const**](#function-is_const) () const<br> |
|  bool | [**is\_error**](#function-is_error) () const<br> |
|  bool | [**is\_fork**](#function-is_fork) () const<br> |
|  bool | [**is\_identity**](#function-is_identity) () const<br> |
|  bool | [**is\_infix\_fork**](#function-is_infix_fork) () const<br> |
|  bool | [**is\_infix\_pipe**](#function-is_infix_pipe) () const<br> |
|  bool | [**is\_infix\_tuple**](#function-is_infix_tuple) () const<br> |
|  bool | [**is\_link**](#function-is_link) () const<br> |
|  bool | [**is\_literal**](#function-is_literal) () const<br> |
|  bool | [**is\_noop**](#function-is_noop) () const<br> |
|  bool | [**is\_null**](#function-is_null) () const<br> |
|  bool | [**is\_preproc**](#function-is_preproc) () const<br> |
|  bool | [**is\_tuple**](#function-is_tuple) () const<br> |
|  bool | [**is\_valid\_link**](#function-is_valid_link) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](#function-keyword) () const<br> |
|  boost::json::string\_view | [**keyword\_to\_str**](#function-keyword_to_str) () const<br> |
|  std::vector&lt; [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &gt; | [**link\_parameters**](#function-link_parameters) () const<br> |
|  bool | [**match**](#function-match) (boost::json::value const & x, [**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) const<br>_Eval and cast to boolean, return false on error._  |
|  bool | [**operator!=**](#function-operator) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & o) const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) & | [**operator=**](#function-operator_1) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const &) = default<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) & | [**operator=**](#function-operator_2) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &&) = default<br> |
|  bool | [**operator==**](#function-operator_3) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & o) const<br> |
|  std::list&lt; std::pair&lt; std::string, std::string &gt; &gt; | [**preprocessing\_parameters**](#function-preprocessing_parameters) () const<br>_List of [param, json ptr]._  |
|  std::string | [**prettify**](#function-prettify) () const<br> |
|  std::ostream & | [**prettify\_to**](#function-prettify_to-13) (std::ostream & os) const<br> |
|  void | [**prettify\_to**](#function-prettify_to-23) (char(&) buff) const<br> |
|  void | [**prettify\_to**](#function-prettify_to-33) (char \* buff, std::size\_t n) const<br> |
|  std::string | [**serialize**](#function-serialize) () const<br> |
|  std::vector&lt; [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &gt; | [**subexpressions\_list**](#function-subexpressions_list) () const<br>_Subexpressions._  |
|  boost::json::value | [**to\_json**](#function-to_json) () const<br> |
| virtual  | [**~ExpressionView**](#function-expressionview) () = default<br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; | [**const\_predicate\_cache\_**](#variable-const_predicate_cache_)  <br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**encoding\_view\_**](#variable-encoding_view_)  <br> |




















## Detailed Description


[**Expression**](classzmbt_1_1lang_1_1Expression.md) Language implementation class.




**See also:** [Expression Language documentation](/user-guide/expressions/). 



    
## Public Types Documentation




### typedef Keyword 

```C++
using zmbt::lang::ExpressionView::Keyword =  lang::Keyword;
```




<hr>



### typedef V 

```C++
using zmbt::lang::ExpressionView::V =  boost::json::value;
```




<hr>
## Public Functions Documentation




### function ExpressionView [1/5]

```C++
zmbt::lang::ExpressionView::ExpressionView () 
```




<hr>



### function ExpressionView [2/5]

```C++
zmbt::lang::ExpressionView::ExpressionView (
    ExpressionView const &
) = default
```




<hr>



### function ExpressionView [3/5]

```C++
zmbt::lang::ExpressionView::ExpressionView (
    ExpressionView &&
) = default
```




<hr>



### function ExpressionView [4/5]

```C++
inline explicit zmbt::lang::ExpressionView::ExpressionView (
    EncodingView v
) 
```




<hr>



### function ExpressionView [5/5]

```C++
inline explicit zmbt::lang::ExpressionView::ExpressionView (
    Encoding enc
) 
```




<hr>



### function as\_array 

```C++
inline boost::json::array const & zmbt::lang::ExpressionView::as_array () const
```




<hr>



### function as\_bool 

```C++
inline bool zmbt::lang::ExpressionView::as_bool () const
```




<hr>



### function as\_object 

```C++
inline boost::json::object const & zmbt::lang::ExpressionView::as_object () const
```




<hr>



### function as\_string 

```C++
inline boost::json::string const & zmbt::lang::ExpressionView::as_string () const
```




<hr>



### function data 

```C++
boost::json::value const & zmbt::lang::ExpressionView::data () const
```




<hr>



### function encoding\_view 

```C++
inline EncodingView zmbt::lang::ExpressionView::encoding_view () const
```




<hr>



### function error\_id 

```C++
std::string zmbt::lang::ExpressionView::error_id () const
```



Internal error identifier. Returns empty string when is\_error() equals false. 


        

<hr>



### function eval 

_Evaluate expression._ 
```C++
boost::json::value zmbt::lang::ExpressionView::eval (
    boost::json::value const & x=nullptr,
    EvalContext ctx={}
) const
```





**Parameters:**


* `x` run-time argument 
* `ctx` evaluation context 



**Returns:**







        

<hr>



### function eval\_as\_predicate [1/2]

```C++
bool zmbt::lang::ExpressionView::eval_as_predicate (
    ExpressionView const & x,
    Expression & err_sts,
    EvalContext ctx
) const
```



Eval const expressions as Eq(expr), except for Noop, otherwise eval expr. 


        

<hr>



### function eval\_as\_predicate [2/2]

```C++
bool zmbt::lang::ExpressionView::eval_as_predicate (
    boost::json::value const & x,
    Expression & err_sts,
    EvalContext ctx
) const
```



Eval const expressions as Eq(expr), except for Noop, otherwise eval expr. Store result in arg reference and return error status. 


        

<hr>



### function eval\_e 

_Evaluate expression._ 
```C++
Expression zmbt::lang::ExpressionView::eval_e (
    ExpressionView const & x,
    EvalContext ctx
) const
```





**Parameters:**


* `x` run-time argument 
* `ctx` evaluation context 



**Returns:**







        

<hr>



### function eval\_maybe\_predicate 

```C++
Expression zmbt::lang::ExpressionView::eval_maybe_predicate (
    ExpressionView const & x,
    EvalContext ctx
) const
```




<hr>



### function has\_subexpr 

```C++
inline bool zmbt::lang::ExpressionView::has_subexpr () const
```




<hr>



### function if\_array 

```C++
inline boost::json::array const * zmbt::lang::ExpressionView::if_array () const
```




<hr>



### function if\_bool 

```C++
inline bool const * zmbt::lang::ExpressionView::if_bool () const
```




<hr>



### function if\_object 

```C++
inline boost::json::object const * zmbt::lang::ExpressionView::if_object () const
```




<hr>



### function if\_string 

```C++
inline boost::json::string const * zmbt::lang::ExpressionView::if_string () const
```




<hr>



### function is 

```C++
inline bool zmbt::lang::ExpressionView::is (
    Keyword const kwrd
) const
```




<hr>



### function is\_boolean 

```C++
bool zmbt::lang::ExpressionView::is_boolean () const
```




<hr>



### function is\_complete\_flip 

```C++
inline bool zmbt::lang::ExpressionView::is_complete_flip () const
```




<hr>



### function is\_compose 

```C++
inline bool zmbt::lang::ExpressionView::is_compose () const
```




<hr>



### function is\_const 

```C++
bool zmbt::lang::ExpressionView::is_const () const
```




<hr>



### function is\_error 

```C++
inline bool zmbt::lang::ExpressionView::is_error () const
```




<hr>



### function is\_fork 

```C++
inline bool zmbt::lang::ExpressionView::is_fork () const
```




<hr>



### function is\_identity 

```C++
inline bool zmbt::lang::ExpressionView::is_identity () const
```




<hr>



### function is\_infix\_fork 

```C++
bool zmbt::lang::ExpressionView::is_infix_fork () const
```




<hr>



### function is\_infix\_pipe 

```C++
bool zmbt::lang::ExpressionView::is_infix_pipe () const
```




<hr>



### function is\_infix\_tuple 

```C++
bool zmbt::lang::ExpressionView::is_infix_tuple () const
```




<hr>



### function is\_link 

```C++
inline bool zmbt::lang::ExpressionView::is_link () const
```




<hr>



### function is\_literal 

```C++
inline bool zmbt::lang::ExpressionView::is_literal () const
```




<hr>



### function is\_noop 

```C++
inline bool zmbt::lang::ExpressionView::is_noop () const
```




<hr>



### function is\_null 

```C++
inline bool zmbt::lang::ExpressionView::is_null () const
```




<hr>



### function is\_preproc 

```C++
inline bool zmbt::lang::ExpressionView::is_preproc () const
```




<hr>



### function is\_tuple 

```C++
inline bool zmbt::lang::ExpressionView::is_tuple () const
```




<hr>



### function is\_valid\_link 

```C++
bool zmbt::lang::ExpressionView::is_valid_link () const
```




<hr>



### function keyword 

```C++
inline Keyword zmbt::lang::ExpressionView::keyword () const
```




<hr>



### function keyword\_to\_str 

```C++
boost::json::string_view zmbt::lang::ExpressionView::keyword_to_str () const
```




<hr>



### function link\_parameters 

```C++
std::vector< ExpressionView > zmbt::lang::ExpressionView::link_parameters () const
```




<hr>



### function match 

_Eval and cast to boolean, return false on error._ 
```C++
bool zmbt::lang::ExpressionView::match (
    boost::json::value const & x,
    Operator const & op={}
) const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::lang::ExpressionView::operator!= (
    ExpressionView const & o
) const
```




<hr>



### function operator= 

```C++
ExpressionView & zmbt::lang::ExpressionView::operator= (
    ExpressionView const &
) = default
```




<hr>



### function operator= 

```C++
ExpressionView & zmbt::lang::ExpressionView::operator= (
    ExpressionView &&
) = default
```




<hr>



### function operator== 

```C++
inline bool zmbt::lang::ExpressionView::operator== (
    ExpressionView const & o
) const
```




<hr>



### function preprocessing\_parameters 

_List of [param, json ptr]._ 
```C++
std::list< std::pair< std::string, std::string > > zmbt::lang::ExpressionView::preprocessing_parameters () const
```




<hr>



### function prettify 

```C++
std::string zmbt::lang::ExpressionView::prettify () const
```




<hr>



### function prettify\_to [1/3]

```C++
std::ostream & zmbt::lang::ExpressionView::prettify_to (
    std::ostream & os
) const
```




<hr>



### function prettify\_to [2/3]

```C++
template<std::size_t N>
inline void zmbt::lang::ExpressionView::prettify_to (
    char(&) buff
) const
```




<hr>



### function prettify\_to [3/3]

```C++
void zmbt::lang::ExpressionView::prettify_to (
    char * buff,
    std::size_t n
) const
```




<hr>



### function serialize 

```C++
inline std::string zmbt::lang::ExpressionView::serialize () const
```




<hr>



### function subexpressions\_list 

_Subexpressions._ 
```C++
std::vector< ExpressionView > zmbt::lang::ExpressionView::subexpressions_list () const
```




<hr>



### function to\_json 

```C++
boost::json::value zmbt::lang::ExpressionView::to_json () const
```




<hr>



### function ~ExpressionView 

```C++
virtual zmbt::lang::ExpressionView::~ExpressionView () = default
```




<hr>
## Protected Attributes Documentation




### variable const\_predicate\_cache\_ 

```C++
std::shared_ptr<Expression> zmbt::lang::ExpressionView::const_predicate_cache_;
```




<hr>



### variable encoding\_view\_ 

```C++
EncodingView zmbt::lang::ExpressionView::encoding_view_;
```




<hr>## Friends Documentation





### friend operator\* 

_Evaluate x to lhs expression._ 
```C++
V zmbt::lang::ExpressionView::operator* (
    ExpressionView expr,
    ExpressionView const & x
) 
```



Equivalent to expr.eval(x). 


        

<hr>



### friend operator\* 

_Evaluate expression._ 
```C++
V zmbt::lang::ExpressionView::operator* (
    ExpressionView expr
) 
```



Equivalent to expr.eval(). 


        

<hr>



### friend operator&lt;&lt; 

```C++
std::ostream & zmbt::lang::ExpressionView::operator<< (
    std::ostream & os,
    ExpressionView const & expr
) 
```




<hr>



### friend operator&lt;&lt; 

```C++
zmbt::Logger & zmbt::lang::ExpressionView::operator<< (
    zmbt::Logger & logger,
    ExpressionView const & expr
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression.hpp`

