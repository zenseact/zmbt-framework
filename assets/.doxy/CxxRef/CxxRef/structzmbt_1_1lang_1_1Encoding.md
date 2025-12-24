

# Struct zmbt::lang::Encoding



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**Encoding**](structzmbt_1_1lang_1_1Encoding.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**K**](#typedef-k)  <br> |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; V &gt; | [**data**](#variable-data)  <br> |
|  std::vector&lt; std::size\_t &gt; | [**depth**](#variable-depth)  <br> |
|  std::vector&lt; [**K**](namespacezmbt_1_1lang.md#enum-keyword) &gt; | [**keywords**](#variable-keywords)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Encoding**](#function-encoding-13) () = default<br> |
|   | [**Encoding**](#function-encoding-23) (boost::json::value const & value) <br> |
|   | [**Encoding**](#function-encoding-33) (boost::json::value && value) <br> |
|  void | [**append\_to\_root**](#function-append_to_root-12) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & tail) <br> |
|  void | [**append\_to\_root**](#function-append_to_root-22) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) && tail) <br> |
|  bool | [**operator!=**](#function-operator) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & o) const<br> |
|  bool | [**operator==**](#function-operator_1) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & o) const<br> |
|  bool | [**preprocess**](#function-preprocess) () <br>_Single-pass preprocessing, return true if no multipass tokens left._  |
|  void | [**push\_back**](#function-push_back-12) ([**K**](namespacezmbt_1_1lang.md#enum-keyword) const & k, std::size\_t const d, V const & v) <br> |
|  void | [**push\_back**](#function-push_back-22) ([**K**](namespacezmbt_1_1lang.md#enum-keyword) const & k, std::size\_t const d, V && v) <br> |
|  std::size\_t | [**size**](#function-size) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**is\_link\_token**](#function-is_link_token) (boost::json::value const & value) <br> |
|  bool | [**is\_preproc\_token**](#function-is_preproc_token) (boost::json::value const & value) <br> |


























## Public Types Documentation




### typedef K 

```C++
using zmbt::lang::Encoding::K =  Keyword;
```




<hr>



### typedef V 

```C++
using zmbt::lang::Encoding::V =  boost::json::value;
```




<hr>
## Public Attributes Documentation




### variable data 

```C++
std::vector<V> zmbt::lang::Encoding::data;
```




<hr>



### variable depth 

```C++
std::vector<std::size_t> zmbt::lang::Encoding::depth;
```




<hr>



### variable keywords 

```C++
std::vector<K> zmbt::lang::Encoding::keywords;
```




<hr>
## Public Functions Documentation




### function Encoding [1/3]

```C++
zmbt::lang::Encoding::Encoding () = default
```




<hr>



### function Encoding [2/3]

```C++
explicit zmbt::lang::Encoding::Encoding (
    boost::json::value const & value
) 
```




<hr>



### function Encoding [3/3]

```C++
explicit zmbt::lang::Encoding::Encoding (
    boost::json::value && value
) 
```




<hr>



### function append\_to\_root [1/2]

```C++
void zmbt::lang::Encoding::append_to_root (
    Encoding const & tail
) 
```




<hr>



### function append\_to\_root [2/2]

```C++
void zmbt::lang::Encoding::append_to_root (
    Encoding && tail
) 
```




<hr>



### function operator!= 

```C++
bool zmbt::lang::Encoding::operator!= (
    Encoding const & o
) const
```




<hr>



### function operator== 

```C++
bool zmbt::lang::Encoding::operator== (
    Encoding const & o
) const
```




<hr>



### function preprocess 

_Single-pass preprocessing, return true if no multipass tokens left._ 
```C++
bool zmbt::lang::Encoding::preprocess () 
```




<hr>



### function push\_back [1/2]

```C++
void zmbt::lang::Encoding::push_back (
    K const & k,
    std::size_t const d,
    V const & v
) 
```




<hr>



### function push\_back [2/2]

```C++
void zmbt::lang::Encoding::push_back (
    K const & k,
    std::size_t const d,
    V && v
) 
```




<hr>



### function size 

```C++
std::size_t zmbt::lang::Encoding::size () const
```




<hr>
## Public Static Functions Documentation




### function is\_link\_token 

```C++
static bool zmbt::lang::Encoding::is_link_token (
    boost::json::value const & value
) 
```




<hr>



### function is\_preproc\_token 

```C++
static bool zmbt::lang::Encoding::is_preproc_token (
    boost::json::value const & value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/encoding.hpp`

