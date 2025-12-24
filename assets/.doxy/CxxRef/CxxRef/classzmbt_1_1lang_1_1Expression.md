

# Class zmbt::lang::Expression



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Expression**](classzmbt_1_1lang_1_1Expression.md)








Inherits the following classes: [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)


Inherited by the following classes: [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md),  [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md),  [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md),  [zmbt::Param](classzmbt_1_1Param.md),  [zmbt::lang::Expression::Subexpression](classzmbt_1_1lang_1_1Expression_1_1Subexpression.md),  [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)










## Classes

| Type | Name |
| ---: | :--- |
| class | [**Subexpression**](classzmbt_1_1lang_1_1Expression_1_1Subexpression.md) <br> |




## Public Types inherited from zmbt::lang::ExpressionView

See [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](classzmbt_1_1lang_1_1ExpressionView.md#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](classzmbt_1_1lang_1_1ExpressionView.md#typedef-v)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Expression**](#function-expression-111) () <br> |
|   | [**Expression**](#function-expression-211) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) && encoding) <br> |
|   | [**Expression**](#function-expression-311) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & encoding) <br> |
|   | [**Expression**](#function-expression-411) (boost::json::value const & expr) <br> |
|   | [**Expression**](#function-expression-511) (boost::json::value && expr) <br> |
|   | [**Expression**](#function-expression-611) (std::initializer\_list&lt; boost::json::value\_ref &gt; items) <br> |
|   | [**Expression**](#function-expression-711) (T sample) <br> |
|   | [**Expression**](#function-expression-811) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword) <br> |
|   | [**Expression**](#function-expression-911) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & view) <br> |
|   | [**Expression**](#function-expression-1011) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & other) <br> |
|   | [**Expression**](#function-expression-1111) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && other) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encoding**](#function-encoding) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) v) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & other) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](#function-operator_2) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && other) <br> |
|  bool | [**operator==**](#function-operator_3) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) v) const<br> |
|   | [**~Expression**](#function-expression) () = default<br> |


## Public Functions inherited from zmbt::lang::ExpressionView

See [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)

| Type | Name |
| ---: | :--- |
|   | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview-15) () <br> |
|   | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview-25) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const &) = default<br> |
|   | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview-35) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &&) = default<br> |
|   | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview-45) ([**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) v) <br> |
|   | [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview-55) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) enc) <br> |
|  boost::json::array const & | [**as\_array**](classzmbt_1_1lang_1_1ExpressionView.md#function-as_array) () const<br> |
|  bool | [**as\_bool**](classzmbt_1_1lang_1_1ExpressionView.md#function-as_bool) () const<br> |
|  boost::json::object const & | [**as\_object**](classzmbt_1_1lang_1_1ExpressionView.md#function-as_object) () const<br> |
|  boost::json::string const & | [**as\_string**](classzmbt_1_1lang_1_1ExpressionView.md#function-as_string) () const<br> |
|  boost::json::value const & | [**data**](classzmbt_1_1lang_1_1ExpressionView.md#function-data) () const<br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**encoding\_view**](classzmbt_1_1lang_1_1ExpressionView.md#function-encoding_view) () const<br> |
|  std::string | [**error\_id**](classzmbt_1_1lang_1_1ExpressionView.md#function-error_id) () const<br> |
|  boost::json::value | [**eval**](classzmbt_1_1lang_1_1ExpressionView.md#function-eval) (boost::json::value const & x=nullptr, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx={}) const<br>_Evaluate expression._  |
|  bool | [**eval\_as\_predicate**](classzmbt_1_1lang_1_1ExpressionView.md#function-eval_as_predicate-12) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**Expression**](classzmbt_1_1lang_1_1Expression.md) & err\_sts, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  bool | [**eval\_as\_predicate**](classzmbt_1_1lang_1_1ExpressionView.md#function-eval_as_predicate-22) (boost::json::value const & x, [**Expression**](classzmbt_1_1lang_1_1Expression.md) & err\_sts, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**eval\_e**](classzmbt_1_1lang_1_1ExpressionView.md#function-eval_e) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br>_Evaluate expression._  |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**eval\_maybe\_predicate**](classzmbt_1_1lang_1_1ExpressionView.md#function-eval_maybe_predicate) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & x, [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) ctx) const<br> |
|  bool | [**has\_subexpr**](classzmbt_1_1lang_1_1ExpressionView.md#function-has_subexpr) () const<br> |
|  boost::json::array const \* | [**if\_array**](classzmbt_1_1lang_1_1ExpressionView.md#function-if_array) () const<br> |
|  bool const \* | [**if\_bool**](classzmbt_1_1lang_1_1ExpressionView.md#function-if_bool) () const<br> |
|  boost::json::object const \* | [**if\_object**](classzmbt_1_1lang_1_1ExpressionView.md#function-if_object) () const<br> |
|  boost::json::string const \* | [**if\_string**](classzmbt_1_1lang_1_1ExpressionView.md#function-if_string) () const<br> |
|  bool | [**is**](classzmbt_1_1lang_1_1ExpressionView.md#function-is) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const kwrd) const<br> |
|  bool | [**is\_boolean**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_boolean) () const<br> |
|  bool | [**is\_complete\_flip**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_complete_flip) () const<br> |
|  bool | [**is\_compose**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_compose) () const<br> |
|  bool | [**is\_const**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_const) () const<br> |
|  bool | [**is\_error**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_error) () const<br> |
|  bool | [**is\_fork**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_fork) () const<br> |
|  bool | [**is\_identity**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_identity) () const<br> |
|  bool | [**is\_infix\_fork**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_infix_fork) () const<br> |
|  bool | [**is\_infix\_pipe**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_infix_pipe) () const<br> |
|  bool | [**is\_infix\_tuple**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_infix_tuple) () const<br> |
|  bool | [**is\_link**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_link) () const<br> |
|  bool | [**is\_literal**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_literal) () const<br> |
|  bool | [**is\_noop**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_noop) () const<br> |
|  bool | [**is\_null**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_null) () const<br> |
|  bool | [**is\_preproc**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_preproc) () const<br> |
|  bool | [**is\_tuple**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_tuple) () const<br> |
|  bool | [**is\_valid\_link**](classzmbt_1_1lang_1_1ExpressionView.md#function-is_valid_link) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](classzmbt_1_1lang_1_1ExpressionView.md#function-keyword) () const<br> |
|  boost::json::string\_view | [**keyword\_to\_str**](classzmbt_1_1lang_1_1ExpressionView.md#function-keyword_to_str) () const<br> |
|  std::vector&lt; [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &gt; | [**link\_parameters**](classzmbt_1_1lang_1_1ExpressionView.md#function-link_parameters) () const<br> |
|  bool | [**match**](classzmbt_1_1lang_1_1ExpressionView.md#function-match) (boost::json::value const & x, [**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) const<br>_Eval and cast to boolean, return false on error._  |
|  bool | [**operator!=**](classzmbt_1_1lang_1_1ExpressionView.md#function-operator) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & o) const<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) & | [**operator=**](classzmbt_1_1lang_1_1ExpressionView.md#function-operator_1) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const &) = default<br> |
|  [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) & | [**operator=**](classzmbt_1_1lang_1_1ExpressionView.md#function-operator_2) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &&) = default<br> |
|  bool | [**operator==**](classzmbt_1_1lang_1_1ExpressionView.md#function-operator_3) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & o) const<br> |
|  std::list&lt; std::pair&lt; std::string, std::string &gt; &gt; | [**preprocessing\_parameters**](classzmbt_1_1lang_1_1ExpressionView.md#function-preprocessing_parameters) () const<br>_List of [param, json ptr]._  |
|  std::string | [**prettify**](classzmbt_1_1lang_1_1ExpressionView.md#function-prettify) () const<br> |
|  std::ostream & | [**prettify\_to**](classzmbt_1_1lang_1_1ExpressionView.md#function-prettify_to-13) (std::ostream & os) const<br> |
|  void | [**prettify\_to**](classzmbt_1_1lang_1_1ExpressionView.md#function-prettify_to-23) (char(&) buff) const<br> |
|  void | [**prettify\_to**](classzmbt_1_1lang_1_1ExpressionView.md#function-prettify_to-33) (char \* buff, std::size\_t n) const<br> |
|  std::string | [**serialize**](classzmbt_1_1lang_1_1ExpressionView.md#function-serialize) () const<br> |
|  std::vector&lt; [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) &gt; | [**subexpressions\_list**](classzmbt_1_1lang_1_1ExpressionView.md#function-subexpressions_list) () const<br>_Subexpressions._  |
|  boost::json::value | [**to\_json**](classzmbt_1_1lang_1_1ExpressionView.md#function-to_json) () const<br> |
| virtual  | [**~ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md#function-expressionview) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeLiteral**](#function-encodeliteral) (boost::json::value const & params) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeNested**](#function-encodenested-23) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, std::initializer\_list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; subexpressions) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeNested**](#function-encodenested-33) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, std::vector&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; && subexpressions) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodePreProc**](#function-encodepreproc) (boost::json::value const & params) <br> |
|  bool | [**to\_predicate\_if\_const**](#function-to_predicate_if_const) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) & e) <br> |














## Protected Attributes inherited from zmbt::lang::ExpressionView

See [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; | [**const\_predicate\_cache\_**](classzmbt_1_1lang_1_1ExpressionView.md#variable-const_predicate_cache_)  <br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**encoding\_view\_**](classzmbt_1_1lang_1_1ExpressionView.md#variable-encoding_view_)  <br> |






































## Public Functions Documentation




### function Expression [1/11]

```C++
zmbt::lang::Expression::Expression () 
```




<hr>



### function Expression [2/11]

```C++
explicit zmbt::lang::Expression::Expression (
    Encoding && encoding
) 
```




<hr>



### function Expression [3/11]

```C++
explicit zmbt::lang::Expression::Expression (
    Encoding const & encoding
) 
```




<hr>



### function Expression [4/11]

```C++
zmbt::lang::Expression::Expression (
    boost::json::value const & expr
) 
```




<hr>



### function Expression [5/11]

```C++
zmbt::lang::Expression::Expression (
    boost::json::value && expr
) 
```




<hr>



### function Expression [6/11]

```C++
zmbt::lang::Expression::Expression (
    std::initializer_list< boost::json::value_ref > items
) 
```




<hr>



### function Expression [7/11]

```C++
template<class T>
inline zmbt::lang::Expression::Expression (
    T sample
) 
```




<hr>



### function Expression [8/11]

```C++
explicit zmbt::lang::Expression::Expression (
    Keyword const & keyword
) 
```




<hr>



### function Expression [9/11]

```C++
explicit zmbt::lang::Expression::Expression (
    ExpressionView const & view
) 
```




<hr>



### function Expression [10/11]

```C++
zmbt::lang::Expression::Expression (
    Expression const & other
) 
```




<hr>



### function Expression [11/11]

```C++
zmbt::lang::Expression::Expression (
    Expression && other
) 
```




<hr>



### function encoding 

```C++
inline Encoding zmbt::lang::Expression::encoding () const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::lang::Expression::operator!= (
    Expression v
) const
```




<hr>



### function operator= 

```C++
Expression & zmbt::lang::Expression::operator= (
    Expression const & other
) 
```




<hr>



### function operator= 

```C++
Expression & zmbt::lang::Expression::operator= (
    Expression && other
) 
```




<hr>



### function operator== 

```C++
inline bool zmbt::lang::Expression::operator== (
    Expression v
) const
```




<hr>



### function ~Expression 

```C++
zmbt::lang::Expression::~Expression () = default
```




<hr>
## Public Static Functions Documentation




### function encodeLiteral 

```C++
static Encoding zmbt::lang::Expression::encodeLiteral (
    boost::json::value const & params
) 
```




<hr>



### function encodeNested [2/3]

```C++
static Encoding zmbt::lang::Expression::encodeNested (
    Keyword const & keyword,
    std::initializer_list< Expression > subexpressions
) 
```




<hr>



### function encodeNested [3/3]

```C++
static Encoding zmbt::lang::Expression::encodeNested (
    Keyword const & keyword,
    std::vector< Expression > && subexpressions
) 
```




<hr>



### function encodePreProc 

```C++
static Encoding zmbt::lang::Expression::encodePreProc (
    boost::json::value const & params
) 
```




<hr>



### function to\_predicate\_if\_const 

```C++
static bool zmbt::lang::Expression::to_predicate_if_const (
    Expression & e
) 
```




<hr>## Friends Documentation





### friend operator& 

_Pack expression results into an array._ 
```C++
Expression zmbt::lang::Expression::operator& (
    Expression lhs,
    Expression rhs
) 
```





**See also:** [**zmbt::expr::Fork**](namespacezmbt_1_1expr.md#variable-fork). 



        

<hr>



### friend operator, 

_Pack expression into a tuple without evaluation._ 
```C++
Expression zmbt::lang::Expression::operator, (
    Expression lhs,
    Expression rhs
) 
```





**See also:** [**zmbt::expr::Tuple**](namespacezmbt_1_1expr.md#variable-tuple). 



        

<hr>



### friend operator&lt;&lt; 

_Inline named function, equivalent to Fn(link + expr)_ 
```C++
Expression zmbt::lang::Expression::operator<< (
    Expression link,
    Expression expr
) 
```




<hr>



### friend operator\| 

```C++
Expression zmbt::lang::Expression::operator| (
    Expression lhs,
    Expression rhs
) 
```



Pipe expressions left-to-right


Pipe functional expressions in composition, s.t. `a | b` is equivalent to `Pipe(a, b)`. 

**See also:** [**zmbt::expr::Pipe**](namespacezmbt_1_1expr.md#variable-pipe) 



        

<hr>



### friend operator~ 

_Flip design-time and eval-time parameters._ 
```C++
Expression zmbt::lang::Expression::operator~ (
    Expression expr
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression.hpp`

