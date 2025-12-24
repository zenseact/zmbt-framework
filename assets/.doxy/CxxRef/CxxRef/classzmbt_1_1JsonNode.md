

# Class zmbt::JsonNode



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**JsonNode**](classzmbt_1_1JsonNode.md)



_Mutator/accessor wrapper over shared JSON value._ [More...](#detailed-description)

* `#include <json_node.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**JsonNode**](#function-jsonnode-29) (std::shared\_ptr&lt; boost::json::value &gt; root) <br> |
|   | [**JsonNode**](#function-jsonnode-39) () <br> |
|   | [**JsonNode**](#function-jsonnode-49) (boost::json::storage\_ptr sp) <br> |
|   | [**JsonNode**](#function-jsonnode-59) ([**JsonNode**](classzmbt_1_1JsonNode.md) const & o) <br> |
|   | [**JsonNode**](#function-jsonnode-69) ([**JsonNode**](classzmbt_1_1JsonNode.md) & o) <br> |
|   | [**JsonNode**](#function-jsonnode-79) ([**JsonNode**](classzmbt_1_1JsonNode.md) && o) <br> |
|   | [**JsonNode**](#function-jsonnode-89) (boost::json::value const & v) <br> |
|   | [**JsonNode**](#function-jsonnode-99) (boost::json::value && v) <br> |
|  boost::json::array const & | [**as\_array**](#function-as_array-12) () const<br> |
|  boost::json::array & | [**as\_array**](#function-as_array-22) () <br> |
|  boost::json::object const & | [**as\_object**](#function-as_object-12) () const<br> |
|  boost::json::object & | [**as\_object**](#function-as_object-22) () <br> |
|  boost::json::value const & | [**at**](#function-at-12) (boost::json::string\_view json\_ptr) const<br> |
|  boost::json::value const & | [**at**](#function-at-22) (boost::json::string\_view fmtstr, T && first, A &&... rest) const<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**branch**](#function-branch-14) (boost::json::kind kind, boost::json::string\_view subnode\_ptr) <br>_create_ [_**JsonNode**_](classzmbt_1_1JsonNode.md) _proxy over subnode_ |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**branch**](#function-branch-24) (boost::json::string\_view subnode\_ptr) <br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**branch**](#function-branch-34) (boost::json::kind kind, boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**branch**](#function-branch-44) (boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  bool | [**contains**](#function-contains-12) (boost::json::string\_view json\_ptr) const<br> |
|  bool | [**contains**](#function-contains-22) (boost::json::string\_view fmtstr, T && first, A &&... rest) const<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**copy**](#function-copy) () const<br> |
|  boost::json::value const \* | [**find\_pointer**](#function-find_pointer-13) (boost::json::string\_view json\_ptr) const<br> |
|  boost::json::value const \* | [**find\_pointer**](#function-find_pointer-23) (boost::json::string\_view fmtstr, T && first, A &&... rest) const<br> |
|  boost::json::value \* | [**find\_pointer**](#function-find_pointer-33) (boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  boost::json::array & | [**get\_or\_create\_array**](#function-get_or_create_array-12) (boost::json::string\_view json\_ptr="") <br> |
|  boost::json::array & | [**get\_or\_create\_array**](#function-get_or_create_array-22) (boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  boost::json::object & | [**get\_or\_create\_object**](#function-get_or_create_object-12) (boost::json::string\_view json\_ptr="") <br> |
|  boost::json::object & | [**get\_or\_create\_object**](#function-get_or_create_object-22) (boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  boost::json::value | [**get\_or\_default**](#function-get_or_default) (boost::json::string\_view json\_ptr, boost::json::value const & dflt) const<br> |
|  bool | [**is\_null**](#function-is_null) () const<br> |
|  boost::json::value const \* | [**node**](#function-node-14) (boost::system::error\_code & ec) const<br> |
|  boost::json::value \* | [**node**](#function-node-24) (boost::system::error\_code & ec) <br> |
|  boost::json::value const & | [**node**](#function-node-34) () const<br> |
|  boost::json::value & | [**node**](#function-node-44) () <br> |
|  boost::json::string\_view | [**node\_ptr**](#function-node_ptr) () const<br> |
|  boost::json::value & | [**operator()**](#function-operator) (boost::json::string\_view json\_ptr="") <br> |
|  boost::json::value & | [**operator()**](#function-operator_1) (boost::json::string\_view fmtstr, T && first, A &&... rest) <br> |
|  boost::json::value const & | [**operator()**](#function-operator_2) (boost::json::string\_view json\_ptr="") const<br> |
|  boost::json::value const & | [**operator()**](#function-operator_3) (boost::json::string\_view fmtstr, T && first, A &&... rest) const<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) & | [**operator=**](#function-operator_4) ([**JsonNode**](classzmbt_1_1JsonNode.md) const &) = default<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) & | [**operator=**](#function-operator_5) ([**JsonNode**](classzmbt_1_1JsonNode.md) &&) = default<br> |
|  std::shared\_ptr&lt; boost::json::value &gt; const & | [**root**](#function-root) () const<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**root\_node**](#function-root_node) () const<br> |
|  std::size\_t | [**size**](#function-size) () const<br> |
|   | [**~JsonNode**](#function-jsonnode) () noexcept<br> |




























## Detailed Description


**Provides** read/write access to the referenced value internal structure via JSON Pointer. Each instance owns a shared pointer to the root value, so subnode proxies may prolong original object lifetime.


**Additions** to the standard JSON Pointer syntax: At-the-end token: '@'. Points to the last element of an array. Queries with at-the-end token always results in creating a new node on empty array. Past-the-end token: '+'. Points to the new element of an array. Queries with past-the-end token always results in creating a new node.


**The** rules for the additional tokens are:


**For** each @ token
* if the node is null, the token substituted with 0; otherwise
* if the node is an empty array, the token substituted with 0; otherwise
* if the node is a non-empty array, the token substituted with node size - 1; otherwise
* an error is produced.




**For** each + token
* if the node is null, the token substituted with 0; otherwise
* if the node is an array, the token substituted with node size; otherwise
* an error is produced. 




    
## Public Functions Documentation




### function JsonNode [2/9]

```C++
inline explicit zmbt::JsonNode::JsonNode (
    std::shared_ptr< boost::json::value > root
) 
```




<hr>



### function JsonNode [3/9]

```C++
inline zmbt::JsonNode::JsonNode () 
```




<hr>



### function JsonNode [4/9]

```C++
inline explicit zmbt::JsonNode::JsonNode (
    boost::json::storage_ptr sp
) 
```




<hr>



### function JsonNode [5/9]

```C++
inline zmbt::JsonNode::JsonNode (
    JsonNode const & o
) 
```




<hr>



### function JsonNode [6/9]

```C++
inline zmbt::JsonNode::JsonNode (
    JsonNode & o
) 
```




<hr>



### function JsonNode [7/9]

```C++
inline zmbt::JsonNode::JsonNode (
    JsonNode && o
) 
```




<hr>



### function JsonNode [8/9]

```C++
inline zmbt::JsonNode::JsonNode (
    boost::json::value const & v
) 
```




<hr>



### function JsonNode [9/9]

```C++
inline zmbt::JsonNode::JsonNode (
    boost::json::value && v
) 
```




<hr>



### function as\_array [1/2]

```C++
inline boost::json::array const & zmbt::JsonNode::as_array () const
```




<hr>



### function as\_array [2/2]

```C++
inline boost::json::array & zmbt::JsonNode::as_array () 
```




<hr>



### function as\_object [1/2]

```C++
inline boost::json::object const & zmbt::JsonNode::as_object () const
```




<hr>



### function as\_object [2/2]

```C++
inline boost::json::object & zmbt::JsonNode::as_object () 
```




<hr>



### function at [1/2]

```C++
boost::json::value const & zmbt::JsonNode::at (
    boost::json::string_view json_ptr
) const
```




<hr>



### function at [2/2]

```C++
template<class T, class... A>
inline boost::json::value const & zmbt::JsonNode::at (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) const
```




<hr>



### function branch [1/4]

_create_ [_**JsonNode**_](classzmbt_1_1JsonNode.md) _proxy over subnode_
```C++
JsonNode zmbt::JsonNode::branch (
    boost::json::kind kind,
    boost::json::string_view subnode_ptr
) 
```




<hr>



### function branch [2/4]

```C++
inline JsonNode zmbt::JsonNode::branch (
    boost::json::string_view subnode_ptr
) 
```




<hr>



### function branch [3/4]

```C++
template<class T, class... A>
inline JsonNode zmbt::JsonNode::branch (
    boost::json::kind kind,
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function branch [4/4]

```C++
template<class T, class... A>
inline JsonNode zmbt::JsonNode::branch (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function contains [1/2]

```C++
inline bool zmbt::JsonNode::contains (
    boost::json::string_view json_ptr
) const
```




<hr>



### function contains [2/2]

```C++
template<class T, class... A>
inline bool zmbt::JsonNode::contains (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) const
```




<hr>



### function copy 

```C++
inline JsonNode zmbt::JsonNode::copy () const
```




<hr>



### function find\_pointer [1/3]

```C++
inline boost::json::value const * zmbt::JsonNode::find_pointer (
    boost::json::string_view json_ptr
) const
```




<hr>



### function find\_pointer [2/3]

```C++
template<class T, class... A>
inline boost::json::value const * zmbt::JsonNode::find_pointer (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) const
```




<hr>



### function find\_pointer [3/3]

```C++
template<class T, class... A>
inline boost::json::value * zmbt::JsonNode::find_pointer (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function get\_or\_create\_array [1/2]

```C++
inline boost::json::array & zmbt::JsonNode::get_or_create_array (
    boost::json::string_view json_ptr=""
) 
```




<hr>



### function get\_or\_create\_array [2/2]

```C++
template<class T, class... A>
inline boost::json::array & zmbt::JsonNode::get_or_create_array (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function get\_or\_create\_object [1/2]

```C++
inline boost::json::object & zmbt::JsonNode::get_or_create_object (
    boost::json::string_view json_ptr=""
) 
```




<hr>



### function get\_or\_create\_object [2/2]

```C++
template<class T, class... A>
inline boost::json::object & zmbt::JsonNode::get_or_create_object (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function get\_or\_default 

```C++
inline boost::json::value zmbt::JsonNode::get_or_default (
    boost::json::string_view json_ptr,
    boost::json::value const & dflt
) const
```




<hr>



### function is\_null 

```C++
inline bool zmbt::JsonNode::is_null () const
```




<hr>



### function node [1/4]

```C++
inline boost::json::value const * zmbt::JsonNode::node (
    boost::system::error_code & ec
) const
```




<hr>



### function node [2/4]

```C++
inline boost::json::value * zmbt::JsonNode::node (
    boost::system::error_code & ec
) 
```




<hr>



### function node [3/4]

```C++
inline boost::json::value const & zmbt::JsonNode::node () const
```




<hr>



### function node [4/4]

```C++
inline boost::json::value & zmbt::JsonNode::node () 
```




<hr>



### function node\_ptr 

```C++
inline boost::json::string_view zmbt::JsonNode::node_ptr () const
```




<hr>



### function operator() 

```C++
inline boost::json::value & zmbt::JsonNode::operator() (
    boost::json::string_view json_ptr=""
) 
```




<hr>



### function operator() 

```C++
template<class T, class... A>
inline boost::json::value & zmbt::JsonNode::operator() (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) 
```




<hr>



### function operator() 

```C++
inline boost::json::value const & zmbt::JsonNode::operator() (
    boost::json::string_view json_ptr=""
) const
```




<hr>



### function operator() 

```C++
template<class T, class... A>
inline boost::json::value const & zmbt::JsonNode::operator() (
    boost::json::string_view fmtstr,
    T && first,
    A &&... rest
) const
```




<hr>



### function operator= 

```C++
JsonNode & zmbt::JsonNode::operator= (
    JsonNode const &
) = default
```




<hr>



### function operator= 

```C++
JsonNode & zmbt::JsonNode::operator= (
    JsonNode &&
) = default
```




<hr>



### function root 

```C++
inline std::shared_ptr< boost::json::value > const & zmbt::JsonNode::root () const
```




<hr>



### function root\_node 

```C++
inline JsonNode zmbt::JsonNode::root_node () const
```




<hr>



### function size 

```C++
inline std::size_t zmbt::JsonNode::size () const
```




<hr>



### function ~JsonNode 

```C++
zmbt::JsonNode::~JsonNode () noexcept
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```C++
inline std::ostream & zmbt::JsonNode::operator<< (
    std::ostream & os,
    JsonNode const & view
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/json_node.hpp`

