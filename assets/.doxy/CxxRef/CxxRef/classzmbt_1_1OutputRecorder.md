

# Class zmbt::OutputRecorder



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**OutputRecorder**](classzmbt_1_1OutputRecorder.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Frame**](structzmbt_1_1OutputRecorder_1_1Frame.md) &lt;class T&gt;<br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**OutputRecorder**](#function-outputrecorder-13) ([**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id) <br> |
|   | [**OutputRecorder**](#function-outputrecorder-23) ([**OutputRecorder**](classzmbt_1_1OutputRecorder.md) const &) = default<br> |
|   | [**OutputRecorder**](#function-outputrecorder-33) ([**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &&) = default<br> |
|  void | [**clear**](#function-clear) () <br> |
|  std::size\_t | [**count**](#function-count) () const<br>_Push calls count._  |
|  boost::json::array const & | [**data\_frames**](#function-data_frames) () const<br> |
|  void | [**enable\_category**](#function-enable_category) (ChannelKind const ck) <br> |
|  void | [**flush**](#function-flush) () <br>_Flush interal bufferregistry\_s and complete serialization._  |
|  boost::json::string\_view | [**interface\_name**](#function-interface_name) () const<br> |
|  bool | [**is\_registered**](#function-is_registered) () const<br>[_**OutputRecorder**_](classzmbt_1_1OutputRecorder.md) _._ |
|  std::size\_t | [**lost\_count**](#function-lost_count) () const<br>_Push calls count after the capture is closed._  |
|  [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) & | [**operator=**](#function-operator) ([**OutputRecorder**](classzmbt_1_1OutputRecorder.md) const &) = default<br> |
|  [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) & | [**operator=**](#function-operator_1) ([**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &&) = default<br> |
|  void | [**push**](#function-push) (ArgsTuple const & args, [**ErrorOr**](classzmbt_1_1ErrorOr.md)&lt; Return &gt; const & return\_or\_error) <br>_Multi-producer push._  |
|  void | [**setup\_handlers**](#function-setup_handlers) () <br> |
|   | [**~OutputRecorder**](#function-outputrecorder) () <br> |




























## Public Functions Documentation




### function OutputRecorder [1/3]

```C++
zmbt::OutputRecorder::OutputRecorder (
    interface_id const & ifc_id,
    object_id const & obj_id
) 
```




<hr>



### function OutputRecorder [2/3]

```C++
zmbt::OutputRecorder::OutputRecorder (
    OutputRecorder const &
) = default
```




<hr>



### function OutputRecorder [3/3]

```C++
zmbt::OutputRecorder::OutputRecorder (
    OutputRecorder &&
) = default
```




<hr>



### function clear 

```C++
void zmbt::OutputRecorder::clear () 
```




<hr>



### function count 

_Push calls count._ 
```C++
inline std::size_t zmbt::OutputRecorder::count () const
```




<hr>



### function data\_frames 

```C++
boost::json::array const & zmbt::OutputRecorder::data_frames () const
```




<hr>



### function enable\_category 

```C++
void zmbt::OutputRecorder::enable_category (
    ChannelKind const ck
) 
```




<hr>



### function flush 

_Flush interal bufferregistry\_s and complete serialization._ 
```C++
void zmbt::OutputRecorder::flush () 
```




<hr>



### function interface\_name 

```C++
inline boost::json::string_view zmbt::OutputRecorder::interface_name () const
```




<hr>



### function is\_registered 

[_**OutputRecorder**_](classzmbt_1_1OutputRecorder.md) _._
```C++
inline bool zmbt::OutputRecorder::is_registered () const
```




<hr>



### function lost\_count 

_Push calls count after the capture is closed._ 
```C++
inline std::size_t zmbt::OutputRecorder::lost_count () const
```




<hr>



### function operator= 

```C++
OutputRecorder & zmbt::OutputRecorder::operator= (
    OutputRecorder const &
) = default
```




<hr>



### function operator= 

```C++
OutputRecorder & zmbt::OutputRecorder::operator= (
    OutputRecorder &&
) = default
```




<hr>



### function push 

_Multi-producer push._ 
```C++
template<class ArgsTuple, class Return>
inline void zmbt::OutputRecorder::push (
    ArgsTuple const & args,
    ErrorOr < Return > const & return_or_error
) 
```




<hr>



### function setup\_handlers 

```C++
template<class Interface, class InterfacePointer>
inline void zmbt::OutputRecorder::setup_handlers () 
```




<hr>



### function ~OutputRecorder 

```C++
zmbt::OutputRecorder::~OutputRecorder () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/output_recorder.hpp`

