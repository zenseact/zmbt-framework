

# Class zmbt::entity\_id



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**entity\_id**](classzmbt_1_1entity__id.md)



_Base class for annotated key objects._ 

* `#include <entity_id.hpp>`





Inherited by the following classes: [zmbt::interface\_id](classzmbt_1_1interface__id.md),  [zmbt::object\_id](classzmbt_1_1object__id.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|  boost::json::string | [**annotation**](#function-annotation) () const<br> |
|   | [**entity\_id**](#function-entity_id-15) () = default<br> |
|   | [**entity\_id**](#function-entity_id-25) (boost::json::string\_view key, boost::json::string\_view type) <br> |
|   | [**entity\_id**](#function-entity_id-35) (boost::json::value const & val) <br> |
|   | [**entity\_id**](#function-entity_id-45) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|   | [**entity\_id**](#function-entity_id-55) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  boost::json::string | [**key**](#function-key) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;**](#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&lt;=**](#function-operator_2) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_3) ([**entity\_id**](classzmbt_1_1entity__id.md) const &) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_4) ([**entity\_id**](classzmbt_1_1entity__id.md) &&) = default<br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_5) (boost::json::value const & v) <br> |
|  [**entity\_id**](classzmbt_1_1entity__id.md) & | [**operator=**](#function-operator_6) (boost::json::value && v) <br> |
|  bool | [**operator==**](#function-operator_7) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;**](#function-operator_8) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  bool | [**operator&gt;=**](#function-operator_9) ([**entity\_id**](classzmbt_1_1entity__id.md) const & other) const<br> |
|  boost::json::string | [**str**](#function-str) () const<br> |
| virtual  | [**~entity\_id**](#function-entity_id) () = default<br> |




























## Public Functions Documentation




### function annotation 

```C++
inline boost::json::string zmbt::entity_id::annotation () const
```




<hr>



### function entity\_id [1/5]

```C++
zmbt::entity_id::entity_id () = default
```




<hr>



### function entity\_id [2/5]

```C++
zmbt::entity_id::entity_id (
    boost::json::string_view key,
    boost::json::string_view type
) 
```




<hr>



### function entity\_id [3/5]

```C++
explicit zmbt::entity_id::entity_id (
    boost::json::value const & val
) 
```




<hr>



### function entity\_id [4/5]

```C++
zmbt::entity_id::entity_id (
    entity_id const &
) = default
```




<hr>



### function entity\_id [5/5]

```C++
zmbt::entity_id::entity_id (
    entity_id &&
) = default
```




<hr>



### function key 

```C++
inline boost::json::string zmbt::entity_id::key () const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::entity_id::operator!= (
    entity_id const & other
) const
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::entity_id::operator< (
    entity_id const & other
) const
```




<hr>



### function operator&lt;= 

```C++
inline bool zmbt::entity_id::operator<= (
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



### function operator&gt; 

```C++
inline bool zmbt::entity_id::operator> (
    entity_id const & other
) const
```




<hr>



### function operator&gt;= 

```C++
inline bool zmbt::entity_id::operator>= (
    entity_id const & other
) const
```




<hr>



### function str 

```C++
inline boost::json::string zmbt::entity_id::str () const
```




<hr>



### function ~entity\_id 

```C++
virtual zmbt::entity_id::~entity_id () = default
```




<hr>## Friends Documentation





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

