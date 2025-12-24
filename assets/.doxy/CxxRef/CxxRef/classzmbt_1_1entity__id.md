

# Class zmbt::entity\_id



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**entity\_id**](classzmbt_1_1entity__id.md)



_Base class for annotated key objects._ 

* `#include <entity_id.hpp>`





Inherited by the following classes: [zmbt::interface\_id](classzmbt_1_1interface__id.md),  [zmbt::object\_id](classzmbt_1_1object__id.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|  boost::json::string\_view | [**annotation**](#function-annotation) () const<br> |
|   | [**entity\_id**](#function-entity_id-17) () = default<br> |
|   | [**entity\_id**](#function-entity_id-27) (boost::json::string\_view key, std::type\_index type\_index) <br> |
|   | [**entity\_id**](#function-entity_id-37) (boost::json::string\_view key, std::size\_t type\_index\_hash) <br> |
|   | [**entity\_id**](#function-entity_id-47) (boost::json::array const & str) <br> |
|   | [**entity\_id**](#function-entity_id-57) (boost::json::value const & val) <br> |
|   | [**entity\_id**](#function-entity_id-67) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](#function-entity_id-77) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  boost::json::string\_view | [**key**](#function-key) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_2) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_3) (boost::json::value const & v) <br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_4) (boost::json::value && v) <br> |
|  bool | [**operator==**](#function-operator_5) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  boost::json::string\_view | [**str**](#function-str) () const<br> |
| virtual  | [**~entity\_id**](#function-entity_id) () = default<br> |




























## Public Functions Documentation




### function annotation 

```C++
boost::json::string_view zmbt::entity_id::annotation () const
```




<hr>



### function entity\_id [1/7]

```C++
zmbt::entity_id::entity_id () = default
```




<hr>



### function entity\_id [2/7]

```C++
zmbt::entity_id::entity_id (
    boost::json::string_view key,
    std::type_index type_index
) 
```




<hr>



### function entity\_id [3/7]

```C++
zmbt::entity_id::entity_id (
    boost::json::string_view key,
    std::size_t type_index_hash
) 
```




<hr>



### function entity\_id [4/7]

```C++
explicit zmbt::entity_id::entity_id (
    boost::json::array const & str
) 
```




<hr>



### function entity\_id [5/7]

```C++
explicit zmbt::entity_id::entity_id (
    boost::json::value const & val
) 
```




<hr>



### function entity\_id [6/7]

```C++
zmbt::entity_id::entity_id (
    entity_id const &
) = default
```




<hr>



### function entity\_id [7/7]

```C++
zmbt::entity_id::entity_id (
    entity_id &&
) = default
```




<hr>



### function key 

```C++
inline boost::json::string_view zmbt::entity_id::key () const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::entity_id::operator!= (
    entity_id const & other
) const
```




<hr>



### function operator= 

```C++
entity_id & zmbt::entity_id::operator= (
    entity_id const &
) = default
```




<hr>



### function operator= 

```C++
entity_id & zmbt::entity_id::operator= (
    entity_id &&
) = default
```




<hr>



### function operator= 

```C++
inline entity_id & zmbt::entity_id::operator= (
    boost::json::value const & v
) 
```




<hr>



### function operator= 

```C++
inline entity_id & zmbt::entity_id::operator= (
    boost::json::value && v
) 
```




<hr>



### function operator== 

```C++
inline bool zmbt::entity_id::operator== (
    entity_id const & other
) const
```




<hr>



### function str 

```C++
boost::json::string_view zmbt::entity_id::str () const
```




<hr>



### function ~entity\_id 

```C++
virtual zmbt::entity_id::~entity_id () = default
```




<hr>## Friends Documentation





### friend hash\_value 

_Boost.Hash customization point._ 
```C++
inline std::size_t zmbt::entity_id::hash_value (
    entity_id const & v
) 
```




<hr>



### friend operator&lt;&lt; 

```C++
inline std::ostream & zmbt::entity_id::operator<< (
    std::ostream & os,
    entity_id const & id
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/entity_id.hpp`

