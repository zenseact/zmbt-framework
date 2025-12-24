

# Class zmbt::JsonIterZipProdBase



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md)



_Boost JSON array iterator base for zip or cartesian product._ 

* `#include <json_iter.hpp>`



Inherits the following classes: [zmbt::JsonIter](classzmbt_1_1JsonIter.md)


Inherited by the following classes: [zmbt::JsonProdIter](classzmbt_1_1JsonProdIter.md),  [zmbt::JsonZipIter](classzmbt_1_1JsonZipIter.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-15) (boost::json::array const & sequences) <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-25) (boost::json::array && seqences) <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-35) () <br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-45) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) const &) = default<br> |
|   | [**JsonIterZipProdBase**](#function-jsoniterzipprodbase-55) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) &&) = default<br> |
| virtual boost::json::array | [**get**](#function-get) () <br>_iterator value_  |
| virtual bool | [**halt**](#function-halt) () const<br>_Iterator halted._  |
|  [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) & | [**operator=**](#function-operator) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) const &) = default<br> |
|  [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) & | [**operator=**](#function-operator_1) ([**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) &&) = default<br> |
| virtual  | [**~JsonIterZipProdBase**](#function-jsoniterzipprodbase) () = default<br> |


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














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**begins\_**](#variable-begins_)   = `{}`<br> |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**ends\_**](#variable-ends_)   = `{}`<br> |
|  bool | [**halt\_**](#variable-halt_)   = `{false}`<br> |
|  std::vector&lt; boost::json::array::const\_iterator &gt; | [**its\_**](#variable-its_)   = `{}`<br> |
|  boost::json::array | [**owned\_sequences\_**](#variable-owned_sequences_)  <br> |








































## Public Functions Documentation




### function JsonIterZipProdBase [1/5]

```C++
inline explicit zmbt::JsonIterZipProdBase::JsonIterZipProdBase (
    boost::json::array const & sequences
) 
```




<hr>



### function JsonIterZipProdBase [2/5]

```C++
inline explicit zmbt::JsonIterZipProdBase::JsonIterZipProdBase (
    boost::json::array && seqences
) 
```




<hr>



### function JsonIterZipProdBase [3/5]

```C++
inline zmbt::JsonIterZipProdBase::JsonIterZipProdBase () 
```




<hr>



### function JsonIterZipProdBase [4/5]

```C++
zmbt::JsonIterZipProdBase::JsonIterZipProdBase (
    JsonIterZipProdBase const &
) = default
```




<hr>



### function JsonIterZipProdBase [5/5]

```C++
zmbt::JsonIterZipProdBase::JsonIterZipProdBase (
    JsonIterZipProdBase &&
) = default
```




<hr>



### function get 

_iterator value_ 
```C++
virtual boost::json::array zmbt::JsonIterZipProdBase::get () 
```



Implements [*zmbt::JsonIter::get*](classzmbt_1_1JsonIter.md#function-get)


<hr>



### function halt 

_Iterator halted._ 
```C++
inline virtual bool zmbt::JsonIterZipProdBase::halt () const
```



Implements [*zmbt::JsonIter::halt*](classzmbt_1_1JsonIter.md#function-halt)


<hr>



### function operator= 

```C++
JsonIterZipProdBase & zmbt::JsonIterZipProdBase::operator= (
    JsonIterZipProdBase const &
) = default
```




<hr>



### function operator= 

```C++
JsonIterZipProdBase & zmbt::JsonIterZipProdBase::operator= (
    JsonIterZipProdBase &&
) = default
```




<hr>



### function ~JsonIterZipProdBase 

```C++
virtual zmbt::JsonIterZipProdBase::~JsonIterZipProdBase () = default
```




<hr>
## Protected Attributes Documentation




### variable begins\_ 

```C++
std::vector<boost::json::array::const_iterator> zmbt::JsonIterZipProdBase::begins_;
```




<hr>



### variable ends\_ 

```C++
std::vector<boost::json::array::const_iterator> zmbt::JsonIterZipProdBase::ends_;
```




<hr>



### variable halt\_ 

```C++
bool zmbt::JsonIterZipProdBase::halt_;
```




<hr>



### variable its\_ 

```C++
std::vector<boost::json::array::const_iterator> zmbt::JsonIterZipProdBase::its_;
```




<hr>



### variable owned\_sequences\_ 

```C++
boost::json::array zmbt::JsonIterZipProdBase::owned_sequences_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/json_iter.hpp`

