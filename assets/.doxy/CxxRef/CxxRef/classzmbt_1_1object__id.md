

# Class zmbt::object\_id



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**object\_id**](classzmbt_1_1object__id.md)



_Pointer-based object id with type annotation._ 

* `#include <object_id.hpp>`



Inherits the following classes: [zmbt::entity\_id](classzmbt_1_1entity__id.md)












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**string\_key**](structzmbt_1_1object__id_1_1string__key.md) <br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**entity\_id**](#function-entity_id-17) () = default<br> |
|   | [**entity\_id**](#function-entity_id-27) (boost::json::string\_view key, std::type\_index type\_index) <br> |
|   | [**entity\_id**](#function-entity_id-37) (boost::json::string\_view key, std::size\_t type\_index\_hash) <br> |
|   | [**entity\_id**](#function-entity_id-47) (boost::json::array const & str) <br> |
|   | [**entity\_id**](#function-entity_id-57) (boost::json::value const & val) <br> |
|   | [**entity\_id**](#function-entity_id-67) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](#function-entity_id-77) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|   | [**object\_id**](#function-object_id-17) (boost::json::string\_view str) <br> |
|   | [**object\_id**](#function-object_id-27) (boost::json::string const & str) <br> |
|   | [**object\_id**](#function-object_id-37) (std::string const & str) <br> |
|   | [**object\_id**](#function-object_id-47) (char const \* str) <br> |
|   | [**object\_id**](#function-object_id-57) (T obj) <br> |
|   | [**object\_id**](#function-object_id-67) (std::shared\_ptr&lt; T &gt; const & ptr) <br> |
|   | [**object\_id**](#function-object_id-77) (T const & ref) <br> |
|  bool | [**operator!=**](#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator==**](#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |


## Public Functions inherited from zmbt::entity_id

See [zmbt::entity\_id](classzmbt_1_1entity__id.md)

| Type | Name |
| ---: | :--- |
|  boost::json::string\_view | [**annotation**](classzmbt_1_1entity__id.md#function-annotation) () const<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-17) () = default<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-27) (boost::json::string\_view key, std::type\_index type\_index) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-37) (boost::json::string\_view key, std::size\_t type\_index\_hash) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-47) (boost::json::array const & str) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-57) (boost::json::value const & val) <br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-67) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](classzmbt_1_1entity__id.md#function-entity_id-77) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  boost::json::string\_view | [**key**](classzmbt_1_1entity__id.md#function-key) () const<br> |
|  bool | [**operator!=**](classzmbt_1_1entity__id.md#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_2) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_3) (boost::json::value const & v) <br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](classzmbt_1_1entity__id.md#function-operator_4) (boost::json::value && v) <br> |
|  bool | [**operator==**](classzmbt_1_1entity__id.md#function-operator_5) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  boost::json::string\_view | [**str**](classzmbt_1_1entity__id.md#function-str) () const<br> |
| virtual  | [**~entity\_id**](classzmbt_1_1entity__id.md#function-entity_id) () = default<br> |






















































## Public Functions Documentation




### function entity\_id [1/7]

```C++
zmbt::object_id::entity_id () = default
```




<hr>



### function entity\_id [2/7]

```C++
zmbt::object_id::entity_id (
    boost::json::string_view key,
    std::type_index type_index
) 
```




<hr>



### function entity\_id [3/7]

```C++
zmbt::object_id::entity_id (
    boost::json::string_view key,
    std::size_t type_index_hash
) 
```




<hr>



### function entity\_id [4/7]

```C++
explicit zmbt::object_id::entity_id (
    boost::json::array const & str
) 
```




<hr>



### function entity\_id [5/7]

```C++
explicit zmbt::object_id::entity_id (
    boost::json::value const & val
) 
```




<hr>



### function entity\_id [6/7]

```C++
zmbt::object_id::entity_id (
    entity_id const &
) = default
```




<hr>



### function entity\_id [7/7]

```C++
zmbt::object_id::entity_id (
    entity_id &&
) = default
```




<hr>



### function object\_id [1/7]

```C++
inline zmbt::object_id::object_id (
    boost::json::string_view str
) 
```




<hr>



### function object\_id [2/7]

```C++
inline zmbt::object_id::object_id (
    boost::json::string const & str
) 
```




<hr>



### function object\_id [3/7]

```C++
inline zmbt::object_id::object_id (
    std::string const & str
) 
```




<hr>



### function object\_id [4/7]

```C++
inline zmbt::object_id::object_id (
    char const * str
) 
```




<hr>



### function object\_id [5/7]

```C++
template<class T, is_ptr< T >>
inline zmbt::object_id::object_id (
    T obj
) 
```




<hr>



### function object\_id [6/7]

```C++
template<class T>
inline zmbt::object_id::object_id (
    std::shared_ptr< T > const & ptr
) 
```




<hr>



### function object\_id [7/7]

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



### function operator== 

```C++
inline bool zmbt::object_id::operator== (
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

