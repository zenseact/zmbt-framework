

# Class zmbt::interface\_id



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**interface\_id**](classzmbt_1_1interface__id.md)



_Pointer-based interface id with type annotation._ 

* `#include <interface_id.hpp>`



Inherits the following classes: [zmbt::entity\_id](classzmbt_1_1entity__id.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**entity\_id**](#function-entity_id-15) () = default<br> |
|   | [**entity\_id**](#function-entity_id-25) (boost::json::string\_view key, boost::json::string\_view type) <br> |
|   | [**entity\_id**](#function-entity_id-35) (boost::json::value const & val) <br> |
|   | [**entity\_id**](#function-entity_id-45) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](#function-entity_id-55) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|   | [**interface\_id**](#function-interface_id) (T const & ifc) <br> |
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
zmbt::interface_id::entity_id () = default
```




<hr>



### function entity\_id [2/5]

```C++
zmbt::interface_id::entity_id (
    boost::json::string_view key,
    boost::json::string_view type
) 
```




<hr>



### function entity\_id [3/5]

```C++
explicit zmbt::interface_id::entity_id (
    boost::json::value const & val
) 
```




<hr>



### function entity\_id [4/5]

```C++
zmbt::interface_id::entity_id (
    entity_id const &
) = default
```




<hr>



### function entity\_id [5/5]

```C++
zmbt::interface_id::entity_id (
    entity_id &&
) = default
```




<hr>



### function interface\_id 

```C++
template<class T, class>
inline zmbt::interface_id::interface_id (
    T const & ifc
) 
```




<hr>



### function operator!= 

```C++
inline bool zmbt::interface_id::operator!= (
    entity_id const & other
) const
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::interface_id::operator< (
    entity_id const & other
) const
```




<hr>



### function operator&lt;= 

```C++
inline bool zmbt::interface_id::operator<= (
    entity_id const & other
) const
```




<hr>



### function operator== 

```C++
inline bool zmbt::interface_id::operator== (
    entity_id const & other
) const
```




<hr>



### function operator&gt; 

```C++
inline bool zmbt::interface_id::operator> (
    entity_id const & other
) const
```




<hr>



### function operator&gt;= 

```C++
inline bool zmbt::interface_id::operator>= (
    entity_id const & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/interface_id.hpp`

