

# Class zmbt::mapping::ChannelHandle



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**mapping**](namespacezmbt_1_1mapping.md) **>** [**ChannelHandle**](classzmbt_1_1mapping_1_1ChannelHandle.md)



_Channel clause handle for_ [_**SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _model._

* `#include <channel_handle.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::list&lt; [**ChannelHandle**](classzmbt_1_1mapping_1_1ChannelHandle.md) &gt; | [**ConditionPipe**](#typedef-conditionpipe)  <br> |
| typedef ChannelKind | [**Kind**](#typedef-kind)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ChannelHandle**](#function-channelhandle) ([**JsonNode**](classzmbt_1_1JsonNode.md) & model, boost::json::string\_view cnl\_ptr) <br> |
|  boost::json::array | [**captures**](#function-captures) () const<br> |
|  boost::json::string | [**full\_path**](#function-full_path) () const<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**host**](#function-host) () const<br> |
|  std::size\_t | [**index**](#function-index) () const<br> |
|  [**Environment::InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) | [**inerface\_handle**](#function-inerface_handle) () const<br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**interface**](#function-interface) () const<br> |
|  boost::json::string | [**key**](#function-key) () const<br> |
|  Kind | [**kind**](#function-kind) () const<br> |
|  bool | [**operator==**](#function-operator) (boost::json::value const & v) const<br> |
|  boost::json::string | [**signal\_path**](#function-signal_path) () const<br> |
|  boost::json::value | [**tag**](#function-tag) () const<br>_Channel alias or index if not set._  |
|  [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) | [**transform**](#function-transform) () const<br> |
|   | [**~ChannelHandle**](#function-channelhandle) () = default<br> |




























## Public Types Documentation




### typedef ConditionPipe 

```C++
using zmbt::mapping::ChannelHandle::ConditionPipe =  std::list<ChannelHandle>;
```




<hr>



### typedef Kind 

```C++
using zmbt::mapping::ChannelHandle::Kind =  ChannelKind;
```




<hr>
## Public Functions Documentation




### function ChannelHandle 

```C++
zmbt::mapping::ChannelHandle::ChannelHandle (
    JsonNode & model,
    boost::json::string_view cnl_ptr
) 
```




<hr>



### function captures 

```C++
boost::json::array zmbt::mapping::ChannelHandle::captures () const
```




<hr>



### function full\_path 

```C++
boost::json::string zmbt::mapping::ChannelHandle::full_path () const
```




<hr>



### function host 

```C++
object_id zmbt::mapping::ChannelHandle::host () const
```




<hr>



### function index 

```C++
std::size_t zmbt::mapping::ChannelHandle::index () const
```




<hr>



### function inerface\_handle 

```C++
Environment::InterfaceHandle zmbt::mapping::ChannelHandle::inerface_handle () const
```




<hr>



### function interface 

```C++
interface_id zmbt::mapping::ChannelHandle::interface () const
```




<hr>



### function key 

```C++
boost::json::string zmbt::mapping::ChannelHandle::key () const
```




<hr>



### function kind 

```C++
Kind zmbt::mapping::ChannelHandle::kind () const
```




<hr>



### function operator== 

```C++
bool zmbt::mapping::ChannelHandle::operator== (
    boost::json::value const & v
) const
```




<hr>



### function signal\_path 

```C++
boost::json::string zmbt::mapping::ChannelHandle::signal_path () const
```




<hr>



### function tag 

_Channel alias or index if not set._ 
```C++
boost::json::value zmbt::mapping::ChannelHandle::tag () const
```




<hr>



### function transform 

```C++
lang::Expression zmbt::mapping::ChannelHandle::transform () const
```




<hr>



### function ~ChannelHandle 

```C++
zmbt::mapping::ChannelHandle::~ChannelHandle () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/channel_handle.hpp`

