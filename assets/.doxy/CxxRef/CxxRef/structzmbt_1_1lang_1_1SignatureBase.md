

# Struct zmbt::lang::SignatureBase

**template &lt;[**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) K&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md)








Inherits the following classes: [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)


Inherited by the following classes: [zmbt::lang::SignatureBinary](structzmbt_1_1lang_1_1SignatureBinary.md),  [zmbt::lang::SignatureBinary](structzmbt_1_1lang_1_1SignatureBinary.md),  [zmbt::lang::SignatureTernary](structzmbt_1_1lang_1_1SignatureTernary.md),  [zmbt::lang::SignatureTernary](structzmbt_1_1lang_1_1SignatureTernary.md),  [zmbt::lang::SignatureBinary](structzmbt_1_1lang_1_1SignatureBinary.md),  [zmbt::lang::SignatureConst](structzmbt_1_1lang_1_1SignatureConst.md),  [zmbt::lang::SignatureTernary](structzmbt_1_1lang_1_1SignatureTernary.md),  [zmbt::lang::SignatureUnary](structzmbt_1_1lang_1_1SignatureUnary.md),  [zmbt::lang::SignatureVariadic](structzmbt_1_1lang_1_1SignatureVariadic.md)














## Public Types inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
| typedef [**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**Keyword**](classzmbt_1_1lang_1_1Expression.md#typedef-keyword)  <br> |
| typedef boost::json::value | [**V**](classzmbt_1_1lang_1_1Expression.md#typedef-v)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SignatureBase**](#function-signaturebase) () <br> |


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
|  bool | [**is\_const**](classzmbt_1_1lang_1_1Expression.md#function-is_const) () const<br> |
|  bool | [**is\_hiord**](classzmbt_1_1lang_1_1Expression.md#function-is_hiord) () const<br> |
|  bool | [**is\_literal**](classzmbt_1_1lang_1_1Expression.md#function-is_literal) () const<br> |
|  bool | [**is\_noop**](classzmbt_1_1lang_1_1Expression.md#function-is_noop) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**keyword**](classzmbt_1_1lang_1_1Expression.md#function-keyword) () const<br> |
|  bool | [**match**](classzmbt_1_1lang_1_1Expression.md#function-match) (boost::json::value const & observed, [**Operator**](classzmbt_1_1lang_1_1Operator.md) const & op={}) const<br> |
|  bool | [**operator!=**](classzmbt_1_1lang_1_1Expression.md#function-operator) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_1) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) <br> |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) & | [**operator=**](classzmbt_1_1lang_1_1Expression.md#function-operator_2) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) && o) <br> |
|  bool | [**operator==**](classzmbt_1_1lang_1_1Expression.md#function-operator_3) ([**Expression**](classzmbt_1_1lang_1_1Expression.md) const & o) const<br> |
|  boost::json::value const & | [**params**](classzmbt_1_1lang_1_1Expression.md#function-params) () const<br> |
|  boost::json::value const & | [**serialize**](classzmbt_1_1lang_1_1Expression.md#function-serialize) () const<br> |
|  boost::json::value const & | [**subexpr**](classzmbt_1_1lang_1_1Expression.md#function-subexpr) () const<br> |
|  boost::json::value const & | [**underlying**](classzmbt_1_1lang_1_1Expression.md#function-underlying) () const<br> |
|   | [**~Expression**](classzmbt_1_1lang_1_1Expression.md#function-expression) () = default<br> |




## Public Static Functions inherited from zmbt::lang::Expression

See [zmbt::lang::Expression](classzmbt_1_1lang_1_1Expression.md)

| Type | Name |
| ---: | :--- |
|  [**Expression**](classzmbt_1_1lang_1_1Expression.md) | [**asPredicate**](classzmbt_1_1lang_1_1Expression.md#function-aspredicate) (boost::json::value const & underlying) <br> |


















































## Public Functions Documentation




### function SignatureBase 

```C++
inline zmbt::lang::SignatureBase::SignatureBase () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/api_signatures.hpp`

