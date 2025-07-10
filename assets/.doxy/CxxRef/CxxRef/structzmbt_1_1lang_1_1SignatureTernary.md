

# Struct zmbt::lang::SignatureTernary

**template &lt;[**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) K&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**SignatureTernary**](structzmbt_1_1lang_1_1SignatureTernary.md)



_Ternary expression._ [More...](#detailed-description)

* `#include <api_signatures.hpp>`



Inherits the following classes: [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)


















## Public Types inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](classzmbt_1_1lang_1_1Expression.md#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](classzmbt_1_1lang_1_1Expression.md#typedef-v)  <br> |
























































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**operator()**](#function-operator()) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & expr, [**Expression**](classzmbt_1_1lang_1_1Expression.md) const & param) const<br> |


## Public Functions inherited from zmbt::lang::SignatureBase

See [zmbt::lang::SignatureBase](structzmbt_1_1lang_1_1SignatureBase.md)

| Type | Name |
| ---: | :--- |
|   | [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md#function-signaturebase) () <br> |


## Public Functions inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-411) (std::initializer\_list&lt; boost::json::value\_ref &gt; items) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-511) (boost::json::value const & expr) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-611) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, boost::json::value const & params) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-711) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-811) () <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-911) (T const & sample) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-1011) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) <br> |
|   | [**Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression-1111) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && o) <br> |
|  boost::json::value | [**eval**](classzmbt_1_1lang_1_1Expression.md#function-eval) (boost::json::value const & x=nullptr, [**EvalContext**](structzmbt_1_1lang_1_1Expression_1_1EvalContext.md) const & ctx={}) const<br>_Evaluate expression._  |
|  bool | [**has\_params**](classzmbt_1_1lang_1_1Expression.md#function-has_params) () const<br> |
|  bool | [**is**](classzmbt_1_1lang_1_1Expression.md#function-is) ([**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const kwrd) const<br> |
|  bool | [**is\_boolean**](classzmbt_1_1lang_1_1Expression.md#function-is_boolean) () const<br> |
|  bool | [**is\_compose**](classzmbt_1_1lang_1_1Expression.md#function-is_compose) () const<br> |
|  bool | [**is\_const**](classzmbt_1_1lang_1_1Expression.md#function-is_const) () const<br> |
|  bool | [**is\_fork**](classzmbt_1_1lang_1_1Expression.md#function-is_fork) () const<br> |
|  bool | [**is\_hiord**](classzmbt_1_1lang_1_1Expression.md#function-is_hiord) () const<br> |
|  bool | [**is\_literal**](classzmbt_1_1lang_1_1Expression.md#function-is_literal) () const<br> |
|  bool | [**is\_nonempty\_composition**](classzmbt_1_1lang_1_1Expression.md#function-is_nonempty_composition) () const<br> |
|  bool | [**is\_nonempty\_fork**](classzmbt_1_1lang_1_1Expression.md#function-is_nonempty_fork) () const<br> |
|  bool | [**is\_noop**](classzmbt_1_1lang_1_1Expression.md#function-is_noop) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](classzmbt_1_1lang_1_1Expression.md#function-keyword) () const<br> |
|  std::string | [**keyword\_to\_str**](classzmbt_1_1lang_1_1Expression.md#function-keyword_to_str) () const<br> |
|  bool | [**match**](classzmbt_1_1lang_1_1Expression.md#function-match) (boost::json::value const & observed, [**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) const<br> |
|  bool | [**operator!=**](classzmbt_1_1lang_1_1Expression.md#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_2) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && o) <br> |
|  bool | [**operator==**](classzmbt_1_1lang_1_1Expression.md#function-operator_3) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  std::list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; | [**parameter\_list**](classzmbt_1_1lang_1_1Expression.md#function-parameter_list) () const<br> |
|  boost::json::value const & | [**params**](classzmbt_1_1lang_1_1Expression.md#function-params) () const<br> |
|  std::string | [**prettify**](classzmbt_1_1lang_1_1Expression.md#function-prettify) () const<br> |
|  std::string | [**serialize**](classzmbt_1_1lang_1_1Expression.md#function-serialize) () const<br> |
|  boost::json::value const & | [**subexpr**](classzmbt_1_1lang_1_1Expression.md#function-subexpr) () const<br> |
|  boost::json::value const & | [**underlying**](classzmbt_1_1lang_1_1Expression.md#function-underlying) () const<br> |
|   | [**~Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression) () = default<br> |






## Public Static Functions inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**asPredicate**](classzmbt_1_1lang_1_1Expression.md#function-aspredicate) (boost::json::value const & underlying) <br> |










































































## Detailed Description




**Template parameters:**


* `K` keyword 




##### Syntactic forms



Syntatic form over function f:  



    
## Public Functions Documentation




### function operator() 

```C++
inline Expression zmbt::lang::SignatureTernary::operator() (
    Expression const & expr,
    Expression const & param
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/api_signatures.hpp`

