

# Class zmbt::JsonTraverse



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**JsonTraverse**](classzmbt_1_1JsonTraverse.md)



_Boost JSON value depth-first traverser._ 

* `#include <json_traverse.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; bool(boost::json::value const  &, std::string const)&gt; | [**visitor\_fn**](#typedef-visitor_fn)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**JsonTraverse**](#function-jsontraverse) (visitor\_fn visit) <br>_Construct JSON traverser._  |
|  void | [**operator()**](#function-operator) (boost::json::value const & v) const<br> |




























## Public Types Documentation




### typedef visitor\_fn 

```C++
typedef std::function<bool(boost::json::value const&, std::string const)> zmbt::JsonTraverse::visitor_fn;
```




<hr>
## Public Functions Documentation




### function JsonTraverse 

_Construct JSON traverser._ 
```C++
inline zmbt::JsonTraverse::JsonTraverse (
    visitor_fn visit
) 
```





**Parameters:**


* `visit` JSON node visitor: (Node, JSON Pointer) -&gt; node traverse stop 




        

<hr>



### function operator() 

```C++
inline void zmbt::JsonTraverse::operator() (
    boost::json::value const & v
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/json_traverse.hpp`

