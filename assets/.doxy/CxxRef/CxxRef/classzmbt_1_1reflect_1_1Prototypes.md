

# Class zmbt::reflect::Prototypes



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md) **>** [**Prototypes**](classzmbt_1_1reflect_1_1Prototypes.md)



_Interface prototypes in JSON as {"args": [...], "return": ... }._ 

* `#include <prototypes.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Prototypes**](#function-prototypes-13) (boost::json::array && args, boost::json::value && ret) <br> |
|   | [**Prototypes**](#function-prototypes-23) () <br> |
|   | [**Prototypes**](#function-prototypes-33) (T const &) <br> |
|  boost::json::array const & | [**args**](#function-args) () const<br> |
|  boost::json::value const & | [**ret**](#function-ret) () const<br> |




























## Public Functions Documentation




### function Prototypes [1/3]

```C++
inline zmbt::reflect::Prototypes::Prototypes (
    boost::json::array && args,
    boost::json::value && ret
) 
```




<hr>



### function Prototypes [2/3]

```C++
inline zmbt::reflect::Prototypes::Prototypes () 
```




<hr>



### function Prototypes [3/3]

```C++
template<class T>
inline zmbt::reflect::Prototypes::Prototypes (
    T const &
) 
```




<hr>



### function args 

```C++
inline boost::json::array const & zmbt::reflect::Prototypes::args () const
```




<hr>



### function ret 

```C++
inline boost::json::value const & zmbt::reflect::Prototypes::ret () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/prototypes.hpp`

