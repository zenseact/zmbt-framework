

# Class zmbt::JsonIter



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**JsonIter**](classzmbt_1_1JsonIter.md)



_Boost JSON array iterator interface._ 

* `#include <json_iter.hpp>`





Inherited by the following classes: [zmbt::JsonIterZipProdBase](classzmbt_1_1JsonIterZipProdBase.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**JsonIter**](#function-jsoniter-13) () = default<br> |
|   | [**JsonIter**](#function-jsoniter-23) ([**JsonIter**](classzmbt_1_1JsonIter.md) const &) = default<br> |
|   | [**JsonIter**](#function-jsoniter-33) ([**JsonIter**](classzmbt_1_1JsonIter.md) &&) = default<br> |
| virtual boost::json::array | [**get**](#function-get) () = 0<br>_iterator value_  |
| virtual bool | [**halt**](#function-halt) () const = 0<br>_Iterator halted._  |
|  boost::json::array | [**operator\***](#function-operator) () <br>_iterator value_  |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator++**](#function-operator_1) (int) <br>_iterator post-increment_  |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator=**](#function-operator_2) ([**JsonIter**](classzmbt_1_1JsonIter.md) const &) = default<br> |
|  [**JsonIter**](classzmbt_1_1JsonIter.md) & | [**operator=**](#function-operator_3) ([**JsonIter**](classzmbt_1_1JsonIter.md) &&) = default<br> |
| virtual  | [**~JsonIter**](#function-jsoniter) () = default<br> |




























## Public Functions Documentation




### function JsonIter [1/3]

```C++
zmbt::JsonIter::JsonIter () = default
```




<hr>



### function JsonIter [2/3]

```C++
zmbt::JsonIter::JsonIter (
    JsonIter const &
) = default
```




<hr>



### function JsonIter [3/3]

```C++
zmbt::JsonIter::JsonIter (
    JsonIter &&
) = default
```




<hr>



### function get 

_iterator value_ 
```C++
virtual boost::json::array zmbt::JsonIter::get () = 0
```




<hr>



### function halt 

_Iterator halted._ 
```C++
virtual bool zmbt::JsonIter::halt () const = 0
```




<hr>



### function operator\* 

_iterator value_ 
```C++
inline boost::json::array zmbt::JsonIter::operator* () 
```




<hr>



### function operator++ 

_iterator post-increment_ 
```C++
inline JsonIter & zmbt::JsonIter::operator++ (
    int
) 
```




<hr>



### function operator= 

```C++
JsonIter & zmbt::JsonIter::operator= (
    JsonIter const &
) = default
```




<hr>



### function operator= 

```C++
JsonIter & zmbt::JsonIter::operator= (
    JsonIter &&
) = default
```




<hr>



### function ~JsonIter 

```C++
virtual zmbt::JsonIter::~JsonIter () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/json_iter.hpp`

