

# Class zmbt::Param



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Param**](classzmbt_1_1Param.md)



_Generic Parameter Placeholder._ 

* `#include <parameter.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Param**](#function-param-24) (boost::json::value v) <br> |
|   | [**Param**](#function-param-34) ([**Param**](classzmbt_1_1Param.md) const &) = default<br> |
|   | [**Param**](#function-param-44) ([**Param**](classzmbt_1_1Param.md) &&) = default<br> |
|  [**Param**](classzmbt_1_1Param.md) & | [**operator=**](#function-operator) ([**Param**](classzmbt_1_1Param.md) const &) = default<br> |
|  [**Param**](classzmbt_1_1Param.md) & | [**operator=**](#function-operator_1) ([**Param**](classzmbt_1_1Param.md) &&) = default<br> |
|  boost::json::value | [**serialize**](#function-serialize) () const<br> |
|  boost::json::string | [**to\_string**](#function-to_string) () const<br> |
|   | [**~Param**](#function-param) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**isParam**](#function-isparam) (boost::json::value const & v) <br> |
|  [**Param**](classzmbt_1_1Param.md) | [**parse**](#function-parse) (boost::json::value const & v) <br> |


























## Public Functions Documentation




### function Param [2/4]

```C++
inline zmbt::Param::Param (
    boost::json::value v
) 
```




<hr>



### function Param [3/4]

```C++
zmbt::Param::Param (
    Param const &
) = default
```




<hr>



### function Param [4/4]

```C++
zmbt::Param::Param (
    Param &&
) = default
```




<hr>



### function operator= 

```C++
Param & zmbt::Param::operator= (
    Param const &
) = default
```




<hr>



### function operator= 

```C++
Param & zmbt::Param::operator= (
    Param &&
) = default
```




<hr>



### function serialize 

```C++
inline boost::json::value zmbt::Param::serialize () const
```




<hr>



### function to\_string 

```C++
inline boost::json::string zmbt::Param::to_string () const
```




<hr>



### function ~Param 

```C++
zmbt::Param::~Param () = default
```




<hr>
## Public Static Functions Documentation




### function isParam 

```C++
static bool zmbt::Param::isParam (
    boost::json::value const & v
) 
```




<hr>



### function parse 

```C++
static Param zmbt::Param::parse (
    boost::json::value const & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/parameter.hpp`

