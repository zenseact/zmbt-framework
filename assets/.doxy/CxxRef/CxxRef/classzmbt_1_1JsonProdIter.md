

# Class zmbt::JsonProdIter



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**JsonProdIter**](classzmbt_1_1JsonProdIter.md)



_Cartesian Product Iterator._ 

* `#include <json_iter.hpp>`



Inherits the following classes: [zmbt::JsonIterZipProdBase](classzmbt_1_1JsonIterZipProdBase.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-15) (boost::json::array const & sequences) <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-25) (boost::json::array && seqences) <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-35) () <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-45) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) const &) = default<br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-55) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) &&) = default<br> |


## Public Functions inherited from zmbt::JsonIterZipProdBase

See [zmbt::JsonIterZipProdBase](classzmbt_1_1JsonIterZipProdBase.md)

| Type | Name |
| ---: | :--- |
|   | [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase-15) (boost::json::array const & sequences) <br> |
|   | [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase-25) (boost::json::array && seqences) <br> |
|   | [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase-35) () <br> |
|   | [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase-45) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) const &) = default<br> |
|   | [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase-55) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) &&) = default<br> |
| virtual boost::json::array | [**get**](classzmbt_1_1JsonIterZipProdBase.md#function-get) () <br>_iterator value_  |
| virtual bool | [**halt**](classzmbt_1_1JsonIterZipProdBase.md#function-halt) () const<br>_Iterator halted._  |
|  [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) & | [**operator=**](classzmbt_1_1JsonIterZipProdBase.md#function-operator) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) const &) = default<br> |
|  [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) & | [**operator=**](classzmbt_1_1JsonIterZipProdBase.md#function-operator_1) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) &&) = default<br> |
| virtual  | [**~JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md#function-jsoniterzipprodbase) () = default<br> |


## Public Functions inherited from zmbt::JsonIter

See [zmbt::JsonIter](classzmbt_1_1JsonIter.md)

| Type | Name |
| ---: | :--- |
|   | [**JsonIter**](classzmbt_1_1JsonIter.md#function-jsoniter-13) () = default<br> |
|   | [**JsonIter**](classzmbt_1_1JsonIter.md#function-jsoniter-23) ([**JsonIter**](classzmbt_1_1JsonIter.md) const &) = default<br> |
|   | [**JsonIter**](classzmbt_1_1JsonIter.md#function-jsoniter-33) ([**JsonIter**](classzmbt_1_1JsonIter.md) &&) = default<br> |
| virtual boost::json::array | [**get**](classzmbt_1_1JsonIter.md#function-get) () = 0<br>_iterator value_  |
| virtual bool | [**halt**](classzmbt_1_1JsonIter.md#function-halt) () const = 0<br>_Iterator halted._  |
|  boost::json::array | [**operator\***](classzmbt_1_1JsonIter.md#function-operator) () <br>_iterator value_  |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator++**](classzmbt_1_1JsonIter.md#function-operator_1) (int) <br>_iterator post-increment_  |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator=**](classzmbt_1_1JsonIter.md#function-operator_2) ([**JsonIter**](classzmbt_1_1JsonIter.md) const &) = default<br> |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator=**](classzmbt_1_1JsonIter.md#function-operator_3) ([**JsonIter**](classzmbt_1_1JsonIter.md) &&) = default<br> |
| virtual  | [**~JsonIter**](classzmbt_1_1JsonIter.md#function-jsoniter) () = default<br> |






















## Protected Attributes inherited from zmbt::JsonIterZipProdBase

See [zmbt::JsonIterZipProdBase](classzmbt_1_1JsonIterZipProdBase.md)

| Type | Name |
| ---: | :--- |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**begins\_**](classzmbt_1_1JsonIterZipProdBase.md#variable-begins_)   = `{}`<br> |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**ends\_**](classzmbt_1_1JsonIterZipProdBase.md#variable-ends_)   = `{}`<br> |
|  bool | [**halt\_**](classzmbt_1_1JsonIterZipProdBase.md#variable-halt_)   = `{false}`<br> |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**its\_**](classzmbt_1_1JsonIterZipProdBase.md#variable-its_)   = `{}`<br> |
|  boost::json::array | [**owned\_sequences\_**](classzmbt_1_1JsonIterZipProdBase.md#variable-owned_sequences_)  <br> |


























































## Public Functions Documentation




### function JsonIterZipProdBase [1/5]

```C++
inline explicit zmbt::JsonProdIter::JsonIterZipProdBase (
    boost::json::array const & sequences
) 
```




<hr>



### function JsonIterZipProdBase [2/5]

```C++
inline explicit zmbt::JsonProdIter::JsonIterZipProdBase (
    boost::json::array && seqences
) 
```




<hr>



### function JsonIterZipProdBase [3/5]

```C++
inline zmbt::JsonProdIter::JsonIterZipProdBase () 
```




<hr>



### function JsonIterZipProdBase [4/5]

```C++
zmbt::JsonProdIter::JsonIterZipProdBase (
    JsonIterZipProdBase const &
) = default
```




<hr>



### function JsonIterZipProdBase [5/5]

```C++
zmbt::JsonProdIter::JsonIterZipProdBase (
    JsonIterZipProdBase &&
) = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/json_iter.hpp`

