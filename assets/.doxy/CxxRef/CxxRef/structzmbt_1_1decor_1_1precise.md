

# Struct zmbt::decor::precise

**template &lt;class VT&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**decor**](namespacezmbt_1_1decor.md) **>** [**precise**](structzmbt_1_1decor_1_1precise.md)



_Type decorator for floating point numbers. Throws on precision loss in runtime._ [More...](#detailed-description)

* `#include <precise_real.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef VT | [**decorated\_type**](#typedef-decorated_type)  <br>_The decorated type._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**operator decorated\_type**](#function-operator-decorated_type) () const<br> |
|  bool | [**operator&lt;**](#function-operator) ([**precise**](structzmbt_1_1decor_1_1precise.md) other) const<br> |
|  bool | [**operator&lt;**](#function-operator_1) (boost::json::string\_view str) const<br> |
|  [**precise**](structzmbt_1_1decor_1_1precise.md) & | [**operator=**](#function-operator_2) ([**precise**](structzmbt_1_1decor_1_1precise.md) const &) = default<br> |
|  [**precise**](structzmbt_1_1decor_1_1precise.md) & | [**operator=**](#function-operator_3) ([**precise**](structzmbt_1_1decor_1_1precise.md) &&) = default<br> |
|  [**precise**](structzmbt_1_1decor_1_1precise.md) & | [**operator=**](#function-operator_4) (T const v) <br> |
|  [**precise**](structzmbt_1_1decor_1_1precise.md) & | [**operator=**](#function-operator_5) ([**precise**](structzmbt_1_1decor_1_1precise.md)&lt; T &gt; other) <br> |
|  [**precise**](structzmbt_1_1decor_1_1precise.md) & | [**operator=**](#function-operator_6) (boost::json::string\_view str) <br> |
|  bool | [**operator==**](#function-operator_7) ([**precise**](structzmbt_1_1decor_1_1precise.md) other) const<br> |
|  bool | [**operator==**](#function-operator_8) (boost::json::string\_view str) const<br> |
|   | [**precise**](#function-precise-17) () <br> |
|   | [**precise**](#function-precise-27) ([**precise**](structzmbt_1_1decor_1_1precise.md) const &) = default<br> |
|   | [**precise**](#function-precise-37) ([**precise**](structzmbt_1_1decor_1_1precise.md) &&) = default<br> |
|   | [**precise**](#function-precise-47) ([**decorated\_type**](structzmbt_1_1decor_1_1precise.md#typedef-decorated_type) v) <br> |
|   | [**precise**](#function-precise-57) (T const v) <br> |
|   | [**precise**](#function-precise-67) ([**precise**](structzmbt_1_1decor_1_1precise.md)&lt; T &gt; other) <br> |
|   | [**precise**](#function-precise-77) (boost::json::string\_view str) <br>_Construct value from string. Hex literals are supported._  |
|  std::string | [**stringify**](#function-stringify) () const<br> |
|  [**decorated\_type**](structzmbt_1_1decor_1_1precise.md#typedef-decorated_type) | [**value**](#function-value) () const<br> |
|   | [**~precise**](#function-precise) () = default<br> |




























## Detailed Description


The class will ensure exact value conversion between floating point types of different precision on construction and assignment. In addition to handling the f23 vs f63, this class also can be constructed from string literals, enabling support for hex representation. 


    
## Public Types Documentation




### typedef decorated\_type 

_The decorated type._ 
```C++
using zmbt::decor::precise< VT >::decorated_type =  VT;
```




<hr>
## Public Functions Documentation




### function operator decorated\_type 

```C++
inline zmbt::decor::precise::operator decorated_type () const
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::decor::precise::operator< (
    precise other
) const
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::decor::precise::operator< (
    boost::json::string_view str
) const
```




<hr>



### function operator= 

```C++
precise & zmbt::decor::precise::operator= (
    precise const &
) = default
```




<hr>



### function operator= 

```C++
precise & zmbt::decor::precise::operator= (
    precise &&
) = default
```




<hr>



### function operator= 

```C++
template<class T, class>
inline precise & zmbt::decor::precise::operator= (
    T const v
) 
```




<hr>



### function operator= 

```C++
template<class T>
inline precise & zmbt::decor::precise::operator= (
    precise < T > other
) 
```




<hr>



### function operator= 

```C++
inline precise & zmbt::decor::precise::operator= (
    boost::json::string_view str
) 
```




<hr>



### function operator== 

```C++
inline bool zmbt::decor::precise::operator== (
    precise other
) const
```




<hr>



### function operator== 

```C++
inline bool zmbt::decor::precise::operator== (
    boost::json::string_view str
) const
```




<hr>



### function precise [1/7]

```C++
inline zmbt::decor::precise::precise () 
```




<hr>



### function precise [2/7]

```C++
zmbt::decor::precise::precise (
    precise const &
) = default
```




<hr>



### function precise [3/7]

```C++
zmbt::decor::precise::precise (
    precise &&
) = default
```




<hr>



### function precise [4/7]

```C++
inline explicit zmbt::decor::precise::precise (
    decorated_type v
) 
```




<hr>



### function precise [5/7]

```C++
template<class T, class>
inline zmbt::decor::precise::precise (
    T const v
) 
```




<hr>



### function precise [6/7]

```C++
template<class T>
inline zmbt::decor::precise::precise (
    precise < T > other
) 
```




<hr>



### function precise [7/7]

_Construct value from string. Hex literals are supported._ 
```C++
inline zmbt::decor::precise::precise (
    boost::json::string_view str
) 
```




<hr>



### function stringify 

```C++
inline std::string zmbt::decor::precise::stringify () const
```




<hr>



### function value 

```C++
inline decorated_type zmbt::decor::precise::value () const
```




<hr>



### function ~precise 

```C++
zmbt::decor::precise::~precise () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/decor/precise_real.hpp`

