

# Class zmbt::lang::Operator



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Operator**](classzmbt_1_1lang_1_1Operator.md)



_Signal transformation and comparison handler. Enables type erasure._ 

* `#include <operator.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint32\_t | [**Config**](#enum-config)  <br> |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Operator**](#function-operator-27) () <br>_Default operator with generic transforms._  |
|   | [**Operator**](#function-operator-37) ([**type\_tag**](structzmbt_1_1type__tag.md)&lt; T &gt; tag, Config const cfg) <br>[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _with T as type decorator._ |
|   | [**Operator**](#function-operator-47) ([**type\_tag**](structzmbt_1_1type__tag.md)&lt; T &gt; tag) <br>[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _with T as type decorator._ |
|   | [**Operator**](#function-operator-57) (boost::json::string\_view annotation) <br>_Retrieve registered operator instance if it exists, throw otherwise._  |
|   | [**Operator**](#function-operator-67) ([**Operator**](classzmbt_1_1lang_1_1Operator.md) const &) = default<br> |
|   | [**Operator**](#function-operator-77) ([**Operator**](classzmbt_1_1lang_1_1Operator.md) &&) = default<br> |
|  boost::json::string | [**annotation**](#function-annotation) () const<br>_decorated type name_  |
|  boost::json::value | [**apply**](#function-apply) ([**lang::Keyword**](namespacezmbt_1_1lang.md#enum-keyword) const & keyword, boost::json::value const & lhs, boost::json::value const & rhs) const<br>_Apply operands._  |
|  boost::json::value | [**decorate**](#function-decorate) (boost::json::value const & a) const<br>_Reserialize as decorated type._  |
|  bool | [**is\_truth**](#function-is_truth) (boost::json::value const & a) const<br>_Is true._  |
|  [**Operator**](classzmbt_1_1lang_1_1Operator.md) & | [**operator=**](#function-operator) ([**Operator**](classzmbt_1_1lang_1_1Operator.md) const &) = default<br> |
|  [**Operator**](classzmbt_1_1lang_1_1Operator.md) & | [**operator=**](#function-operator_1) ([**Operator**](classzmbt_1_1lang_1_1Operator.md) &&) = default<br> |
|  boost::json::value | [**undecorate**](#function-undecorate) (boost::json::value const & a) const<br>_Reserialize as decorated type._  |
| virtual  | [**~Operator**](#function-operator) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  V | [**generic\_bit\_and**](#function-generic_bit_and) (V const &, V const &) <br> |
|  V | [**generic\_bit\_or**](#function-generic_bit_or) (V const &, V const &) <br> |
|  V | [**generic\_bit\_xor**](#function-generic_bit_xor) (V const &, V const &) <br> |
|  V | [**generic\_complement**](#function-generic_complement) (V const &) <br> |
|  V | [**generic\_decorate**](#function-generic_decorate) (V const & a) <br> |
|  V | [**generic\_divides**](#function-generic_divides) (V const &, V const &) <br> |
|  bool | [**generic\_equal\_to**](#function-generic_equal_to) (V const &, V const &) <br> |
|  bool | [**generic\_is\_truth**](#function-generic_is_truth) (V const & a) <br> |
|  V | [**generic\_left\_shift**](#function-generic_left_shift) (V const &, V const &) <br> |
|  bool | [**generic\_less**](#function-generic_less) (V const &, V const &) <br> |
|  bool | [**generic\_less\_equal**](#function-generic_less_equal) (V const &, V const &) <br> |
|  V | [**generic\_log**](#function-generic_log) (V const &, V const &) <br> |
|  V | [**generic\_logical\_and**](#function-generic_logical_and) (V const &, V const &) <br> |
|  bool | [**generic\_logical\_not**](#function-generic_logical_not) (V const &) <br> |
|  V | [**generic\_logical\_or**](#function-generic_logical_or) (V const &, V const &) <br> |
|  V | [**generic\_minus**](#function-generic_minus) (V const &, V const &) <br> |
|  V | [**generic\_modulus**](#function-generic_modulus) (V const &, V const &) <br> |
|  V | [**generic\_multiplies**](#function-generic_multiplies) (V const &, V const &) <br> |
|  V | [**generic\_negate**](#function-generic_negate) (V const &) <br> |
|  V | [**generic\_plus**](#function-generic_plus) (V const &, V const &) <br> |
|  V | [**generic\_pow**](#function-generic_pow) (V const &, V const &) <br> |
|  V | [**generic\_quot**](#function-generic_quot) (V const &, V const &) <br> |
|  V | [**generic\_right\_shift**](#function-generic_right_shift) (V const &, V const &) <br> |
|  V | [**generic\_undecorate**](#function-generic_undecorate) (V const & a) <br> |


























## Public Types Documentation




### enum Config 

```C++
enum zmbt::lang::Operator::Config {
    Null,
    Decor = 1U << 0,
    Comparison = 1U << 1,
    Arithmetics = 1U << 2,
    Bitwise = 1U << 3,
    Shift = 1U << 4,
    Logic = 1U << 5,
    Default = Decor|Comparison|Arithmetics|Bitwise|Shift,
    Full = Default|Logic
};
```




<hr>



### typedef V 

```C++
using zmbt::lang::Operator::V =  boost::json::value;
```




<hr>
## Public Functions Documentation




### function Operator [2/7]

_Default operator with generic transforms._ 
```C++
zmbt::lang::Operator::Operator () 
```




<hr>



### function Operator [3/7]

[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _with T as type decorator._
```C++
template<class T>
inline zmbt::lang::Operator::Operator (
    type_tag < T > tag,
    Config const cfg
) 
```




<hr>



### function Operator [4/7]

[_**Operator**_](classzmbt_1_1lang_1_1Operator.md) _with T as type decorator._
```C++
template<class T>
inline zmbt::lang::Operator::Operator (
    type_tag < T > tag
) 
```




<hr>



### function Operator [5/7]

_Retrieve registered operator instance if it exists, throw otherwise._ 
```C++
zmbt::lang::Operator::Operator (
    boost::json::string_view annotation
) 
```




<hr>



### function Operator [6/7]

```C++
zmbt::lang::Operator::Operator (
    Operator const &
) = default
```




<hr>



### function Operator [7/7]

```C++
zmbt::lang::Operator::Operator (
    Operator &&
) = default
```




<hr>



### function annotation 

_decorated type name_ 
```C++
inline boost::json::string zmbt::lang::Operator::annotation () const
```




<hr>



### function apply 

_Apply operands._ 
```C++
boost::json::value zmbt::lang::Operator::apply (
    lang::Keyword const & keyword,
    boost::json::value const & lhs,
    boost::json::value const & rhs
) const
```



For unary operators, lhs is nullptr 


        

<hr>



### function decorate 

_Reserialize as decorated type._ 
```C++
inline boost::json::value zmbt::lang::Operator::decorate (
    boost::json::value const & a
) const
```




<hr>



### function is\_truth 

_Is true._ 
```C++
inline bool zmbt::lang::Operator::is_truth (
    boost::json::value const & a
) const
```




<hr>



### function operator= 

```C++
Operator & zmbt::lang::Operator::operator= (
    Operator const &
) = default
```




<hr>



### function operator= 

```C++
Operator & zmbt::lang::Operator::operator= (
    Operator &&
) = default
```




<hr>



### function undecorate 

_Reserialize as decorated type._ 
```C++
inline boost::json::value zmbt::lang::Operator::undecorate (
    boost::json::value const & a
) const
```




<hr>



### function ~Operator 

```C++
virtual zmbt::lang::Operator::~Operator () = default
```




<hr>
## Public Static Functions Documentation




### function generic\_bit\_and 

```C++
static V zmbt::lang::Operator::generic_bit_and (
    V const &,
    V const &
) 
```




<hr>



### function generic\_bit\_or 

```C++
static V zmbt::lang::Operator::generic_bit_or (
    V const &,
    V const &
) 
```




<hr>



### function generic\_bit\_xor 

```C++
static V zmbt::lang::Operator::generic_bit_xor (
    V const &,
    V const &
) 
```




<hr>



### function generic\_complement 

```C++
static V zmbt::lang::Operator::generic_complement (
    V const &
) 
```




<hr>



### function generic\_decorate 

```C++
static V zmbt::lang::Operator::generic_decorate (
    V const & a
) 
```




<hr>



### function generic\_divides 

```C++
static V zmbt::lang::Operator::generic_divides (
    V const &,
    V const &
) 
```




<hr>



### function generic\_equal\_to 

```C++
static bool zmbt::lang::Operator::generic_equal_to (
    V const &,
    V const &
) 
```




<hr>



### function generic\_is\_truth 

```C++
static bool zmbt::lang::Operator::generic_is_truth (
    V const & a
) 
```




<hr>



### function generic\_left\_shift 

```C++
static V zmbt::lang::Operator::generic_left_shift (
    V const &,
    V const &
) 
```




<hr>



### function generic\_less 

```C++
static bool zmbt::lang::Operator::generic_less (
    V const &,
    V const &
) 
```




<hr>



### function generic\_less\_equal 

```C++
static bool zmbt::lang::Operator::generic_less_equal (
    V const &,
    V const &
) 
```




<hr>



### function generic\_log 

```C++
static V zmbt::lang::Operator::generic_log (
    V const &,
    V const &
) 
```




<hr>



### function generic\_logical\_and 

```C++
static V zmbt::lang::Operator::generic_logical_and (
    V const &,
    V const &
) 
```




<hr>



### function generic\_logical\_not 

```C++
static bool zmbt::lang::Operator::generic_logical_not (
    V const &
) 
```




<hr>



### function generic\_logical\_or 

```C++
static V zmbt::lang::Operator::generic_logical_or (
    V const &,
    V const &
) 
```




<hr>



### function generic\_minus 

```C++
static V zmbt::lang::Operator::generic_minus (
    V const &,
    V const &
) 
```




<hr>



### function generic\_modulus 

```C++
static V zmbt::lang::Operator::generic_modulus (
    V const &,
    V const &
) 
```




<hr>



### function generic\_multiplies 

```C++
static V zmbt::lang::Operator::generic_multiplies (
    V const &,
    V const &
) 
```




<hr>



### function generic\_negate 

```C++
static V zmbt::lang::Operator::generic_negate (
    V const &
) 
```




<hr>



### function generic\_plus 

```C++
static V zmbt::lang::Operator::generic_plus (
    V const &,
    V const &
) 
```




<hr>



### function generic\_pow 

```C++
static V zmbt::lang::Operator::generic_pow (
    V const &,
    V const &
) 
```




<hr>



### function generic\_quot 

```C++
static V zmbt::lang::Operator::generic_quot (
    V const &,
    V const &
) 
```




<hr>



### function generic\_right\_shift 

```C++
static V zmbt::lang::Operator::generic_right_shift (
    V const &,
    V const &
) 
```




<hr>



### function generic\_undecorate 

```C++
static V zmbt::lang::Operator::generic_undecorate (
    V const & a
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/operator.hpp`

