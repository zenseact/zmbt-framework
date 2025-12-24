

# Class zmbt::mapping::PipeHandle



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**mapping**](namespacezmbt_1_1mapping.md) **>** [**PipeHandle**](classzmbt_1_1mapping_1_1PipeHandle.md)



_Pipe clause handle for_ [_**SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _model._

* `#include <pipe_handle.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PipeHandle**](#function-pipehandle) ([**JsonNode**](classzmbt_1_1JsonNode.md) & model, std::size\_t const pipe\_idx) <br> |
|  std::list&lt; [**ChannelHandle**](classzmbt_1_1mapping_1_1ChannelHandle.md) &gt; const & | [**channels**](#function-channels) () const<br> |
|  int | [**column**](#function-column) () const<br> |
|  [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) | [**expression**](#function-expression) () const<br> |
|  bool | [**has\_expression**](#function-has_expression) () const<br> |
|  boost::json::value | [**id**](#function-id) () const<br> |
|  boost::json::value | [**index**](#function-index) () const<br> |
|  void | [**inject**](#function-inject) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) expr) const<br> |
|  bool | [**is\_input**](#function-is_input) () const<br> |
|  bool | [**is\_output**](#function-is_output) () const<br> |
|  boost::json::value | [**observe**](#function-observe) () const<br> |
|  bool | [**overload**](#function-overload) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) & e) const<br> |
|  boost::json::value | [**type**](#function-type) () const<br> |
|   | [**~PipeHandle**](#function-pipehandle) () = default<br> |




























## Public Functions Documentation




### function PipeHandle 

```C++
zmbt::mapping::PipeHandle::PipeHandle (
    JsonNode & model,
    std::size_t const pipe_idx
) 
```




<hr>



### function channels 

```C++
inline std::list< ChannelHandle > const & zmbt::mapping::PipeHandle::channels () const
```




<hr>



### function column 

```C++
int zmbt::mapping::PipeHandle::column () const
```




<hr>



### function expression 

```C++
lang::Expression zmbt::mapping::PipeHandle::expression () const
```




<hr>



### function has\_expression 

```C++
bool zmbt::mapping::PipeHandle::has_expression () const
```




<hr>



### function id 

```C++
boost::json::value zmbt::mapping::PipeHandle::id () const
```




<hr>



### function index 

```C++
boost::json::value zmbt::mapping::PipeHandle::index () const
```




<hr>



### function inject 

```C++
void zmbt::mapping::PipeHandle::inject (
    lang::Expression expr
) const
```




<hr>



### function is\_input 

```C++
bool zmbt::mapping::PipeHandle::is_input () const
```




<hr>



### function is\_output 

```C++
bool zmbt::mapping::PipeHandle::is_output () const
```




<hr>



### function observe 

```C++
boost::json::value zmbt::mapping::PipeHandle::observe () const
```




<hr>



### function overload 

```C++
bool zmbt::mapping::PipeHandle::overload (
    lang::Expression & e
) const
```




<hr>



### function type 

```C++
boost::json::value zmbt::mapping::PipeHandle::type () const
```




<hr>



### function ~PipeHandle 

```C++
zmbt::mapping::PipeHandle::~PipeHandle () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/pipe_handle.hpp`

