

# Class zmbt::object\_id



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**object\_id**](classzmbt_1_1object__id.md)



_Pointer-based object id with type annotation._ 

* `#include <object_id.hpp>`



Inherits the following classes: [zmbt::entity\_id](classzmbt_1_1entity__id.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**entity\_id**](#function-entity_id-15) () = default<br> |
|   | [**entity\_id**](#function-entity_id-25) (boost::json::string\_view key, boost::json::string\_view type) <br> |
|   | [**entity\_id**](#function-entity_id-35) (boost::json::value const & val) <br> |
|   | [**entity\_id**](#function-entity_id-45) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](#function-entity_id-55) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|   | [**object\_id**](#function-object_id-16) (boost::json::string\_view str) <br> |
|   | [**object\_id**](#function-object_id-26) (std::string const & str) <br> |
|   | [**object\_id**](#function-object_id-36) (char const \* str) <br> |
|   | [**object\_id**](#function-object_id-46) (T obj) <br> |
|   | [**object\_id**](#function-object_id-56) (std::shared\_ptr&lt; T &gt; const & ptr) <br> |
|   | [**object\_id**](#function-object_id-66) (T const & ref) <br> |
|  bool | [**operator!=**](#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;**](#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;=**](#function-operator_2) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator==**](#function-operator_3) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;**](#function-operator_4) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;=**](#function-operator_5) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |


## Public Functions inherited from zmbt::entity_id

See [zmbt::entity\_id](classzmbt_1_1entity__id.md)

| Type | Name |
| ---: | :--- |
|  boost::json::string | [**annotation**](classzmbt_1_1entity__id.md#function-annotation) () const<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-15) () = default<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-25) (boost::json::string\_view key, boost::json::string\_view type) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-35) (boost::json::value const & val) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-45) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-55) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  boost::json::string | [**key**](classzmbt_1_1entity__id.md#function-key) () const<br> |
|  bool | [**operator!=**](classzmbt_1_1entity__id.md#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;**](classzmbt_1_1entity__id.md#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;=**](classzmbt_1_1entity__id.md#function-operator_2) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_3) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_4) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_5) (boost::json::value const & v) <br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_6) (boost::json::value && v) <br> |
|  bool | [**operator==**](classzmbt_1_1entity__id.md#function-operator_7) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;**](classzmbt_1_1entity__id.md#function-operator_8) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;=**](classzmbt_1_1entity__id.md#function-operator_9) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  boost::json::string | [**str**](classzmbt_1_1entity__id.md#function-str) () const<br> |
| virtual  | [**~entity\_id**](classzmbt_1_1entity__id.md#function-entity_id) () = default<br> |






















































## Public Functions Documentation




### function entity\_id [1/5]

```C++
zmbt::object_id::entity_id () = default
```




<hr>



### function entity\_id [2/5]

```C++
zmbt::object_id::entity_id (
    boost::json::string_view key,
    boost::json::string_view type
) 
```




<hr>



### function entity\_id [3/5]

```C++
explicit zmbt::object_id::entity_id (
    boost::json::value const & val
) 
```




<hr>



### function entity\_id [4/5]

```C++
zmbt::object_id::entity_id (
    entity_id const &
) = default
```




<hr>



### function entity\_id [5/5]

```C++
zmbt::object_id::entity_id (
    entity_id &&
) = default
```




<hr>



### function object\_id [1/6]

```C++
inline zmbt::object_id::object_id (
    boost::json::string_view str
) 
```




<hr>



### function object\_id [2/6]

```C++
inline zmbt::object_id::object_id (
    std::string const & str
) 
```




<hr>



### function object\_id [3/6]

```C++
inline zmbt::object_id::object_id (
    char const * str
) 
```




<hr>



### function object\_id [4/6]

```C++
template<class T, is_ptr< T >>
inline zmbt::object_id::object_id (
    T obj
) 
```




<hr>



### function object\_id [5/6]

```C++
template<class T>
inline zmbt::object_id::object_id (
    std::shared_ptr< T > const & ptr
) 
```




<hr>



### function object\_id [6/6]

```C++
template<class T, is_not_ptr< T >>
inline zmbt::object_id::object_id (
    T const & ref
) 
```




<hr>



### function operator!= 

```C++
inline bool zmbt::object_id::operator!= (
    entity_id const & other
) const
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::object_id::operator< (
    entity_id const & other
) const
```




<hr>



### function operator&lt;= 

```C++
inline bool zmbt::object_id::operator<= (
    entity_id const & other
) const
```




<hr>



### function operator== 

```C++
inline bool zmbt::object_id::operator== (
    entity_id const & other
) const
```




<hr>



### function operator&gt; 

```C++
inline bool zmbt::object_id::operator> (
    entity_id const & other
) const
```




<hr>



### function operator&gt;= 

```C++
inline bool zmbt::object_id::operator>= (
    entity_id const & other
) const
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```C++
inline std::ostream & zmbt::object_id::operator<< (
    std::ostream & os,
    object_id const & id
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/object_id.hpp`

