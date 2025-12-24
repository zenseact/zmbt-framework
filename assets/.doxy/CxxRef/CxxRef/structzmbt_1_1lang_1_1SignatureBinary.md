

# Struct zmbt::lang::SignatureBinary

**template &lt;[**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) K&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**SignatureBinary**](structzmbt_1_1lang_1_1SignatureBinary.md)



_Binary expression._ [More...](#detailed-description)

* `#include <api_signatures.hpp>`



Inherits the following classes: [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)




















## Public Types inherited from zmbt::lang::ExpressionView

See [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](classzmbt_1_1lang_1_1ExpressionView.md#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](classzmbt_1_1lang_1_1ExpressionView.md#typedef-v)  <br> |










































































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**operator()**](#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & param) const<br>_Make parametrized expression._  |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**operator()**](#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && param) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**operator()**](#function-operator_2) (std::initializer\_list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; param) const<br>_Make parametrized expression with initializer list._  |


## Public Functions inherited from zmbt::lang::SignatureBase

See [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)

| Type | Name |
| ---: | :--- |
|   | [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md#function-signaturebase) () <br> |


## Public Functions inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-111) () <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-211) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) && encoding) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-311) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & encoding) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-411) (boost::json::value const & expr) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-511) (boost::json::value && expr) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-611) (std::initializer\_list&lt; boost::json::value\_ref &gt; items) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-711) (T sample) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-811) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-911) ([**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md) const & view) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-1011) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & other) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-1111) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && other) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encoding**](classzmbt_1_1lang_1_1Expression.md#function-encoding) () const<br> |
|  bool | [**operator!=**](classzmbt_1_1lang_1_1Expression.md#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) v) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & other) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_2) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && other) <br> |
|  bool | [**operator==**](classzmbt_1_1lang_1_1Expression.md#function-operator_3) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) v) const<br> |
|   | [**~Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression) () = default<br> |


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






## Public Static Functions inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeLiteral**](classzmbt_1_1lang_1_1Expression.md#function-encodeliteral) (boost::json::value const & params) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeNested**](classzmbt_1_1lang_1_1Expression.md#function-encodenested-23) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, std::initializer\_list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; subexpressions) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodeNested**](classzmbt_1_1lang_1_1Expression.md#function-encodenested-33) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, std::vector&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; && subexpressions) <br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**encodePreProc**](classzmbt_1_1lang_1_1Expression.md#function-encodepreproc) (boost::json::value const & params) <br> |
|  bool | [**to\_predicate\_if\_const**](classzmbt_1_1lang_1_1Expression.md#function-to_predicate_if_const) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) & e) <br> |


























## Protected Attributes inherited from zmbt::lang::ExpressionView

See [zmbt::lang::ExpressionView](classzmbt_1_1lang_1_1ExpressionView.md)

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; | [**const\_predicate\_cache\_**](classzmbt_1_1lang_1_1ExpressionView.md#variable-const_predicate_cache_)  <br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**encoding\_view\_**](classzmbt_1_1lang_1_1ExpressionView.md#variable-encoding_view_)  <br> |










































































## Detailed Description




**Template parameters:**


* `K` keyword 




##### Syntactic forms



Syntatic forms over operator `*`:
* 
  \[E    \mapsto ([x, y] \mapsto x * y      )\]

* 
  \[E    \mapsto (x      \mapsto x * default)\]

* 
  \[E(y) \mapsto (x      \mapsto x * y      )\]





The second form is used in place of first for expressions that have default value defined, see documentation for specific expression.


In the third form the expression parameter is used as the right-hand side operand. 



    
## Public Functions Documentation




### function operator() 

_Make parametrized expression._ 
```C++
inline Expression zmbt::lang::SignatureBinary::operator() (
    Expression const & param
) const
```




<hr>



### function operator() 

```C++
inline Expression zmbt::lang::SignatureBinary::operator() (
    Expression && param
) const
```




<hr>



### function operator() 

_Make parametrized expression with initializer list._ 
```C++
inline Expression zmbt::lang::SignatureBinary::operator() (
    std::initializer_list< Expression > param
) const
```



Interpret {x} as single-element array instead of using default boost::json::value ctor 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/api_signatures.hpp`

