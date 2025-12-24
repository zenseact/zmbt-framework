

# Struct zmbt::OutputRecorder::Registry



[**ClassList**](annotated.md) **>** [**Registry**](structzmbt_1_1OutputRecorder_1_1Registry.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**FrameBuffs**](structzmbt_1_1OutputRecorder_1_1Registry_1_1FrameBuffs.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef boost::concurrent\_flat\_map&lt; std::thread::id, [**FrameBuffs**](structzmbt_1_1OutputRecorder_1_1Registry_1_1FrameBuffs.md), std::hash&lt; std::thread::id &gt; &gt; | [**FramesBuffMap**](#typedef-framesbuffmap)  <br> |
| typedef std::function&lt; void(Registry &)&gt; | [**consume\_fn\_t**](#typedef-consume_fn_t)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::atomic&lt; std::size\_t &gt; | [**count**](#variable-count)   = `{0}`<br> |
|  std::type\_index | [**data\_typeid**](#variable-data_typeid)  <br> |
|  std::bitset&lt; static\_cast&lt; unsigned &gt;(ChannelKind::Undefined)&gt; | [**enable\_categories\_**](#variable-enable_categories_)   = `{}`<br> |
|  consume\_fn\_t | [**extract\_fn**](#variable-extract_fn)  <br> |
|  std::shared\_ptr&lt; FramesBuffMap &gt; | [**frame\_buff\_map**](#variable-frame_buff_map)  <br> |
|  boost::json::string | [**interface\_name**](#variable-interface_name)  <br> |
|  std::atomic&lt; std::size\_t &gt; | [**lost\_count**](#variable-lost_count)   = `{0}`<br> |
|  boost::json::array | [**serialized\_frames**](#variable-serialized_frames)   = `{}`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Registry**](#function-registry) (std::type\_index typid, boost::json::string name, consume\_fn\_t efn, std::shared\_ptr&lt; FramesBuffMap &gt; fbm) <br> |
|   | [**~Registry**](#function-registry) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; Registry &gt; | [**Make**](#function-make) () <br> |


























## Public Types Documentation




### typedef FramesBuffMap 

```C++
using zmbt::OutputRecorder::Registry::FramesBuffMap =  boost::concurrent_flat_map<std::thread::id, FrameBuffs, std::hash<std::thread::id> >;
```




<hr>



### typedef consume\_fn\_t 

```C++
using zmbt::OutputRecorder::Registry::consume_fn_t =  std::function<void(Registry&)>;
```




<hr>
## Public Attributes Documentation




### variable count 

```C++
std::atomic<std::size_t> zmbt::OutputRecorder::Registry::count;
```




<hr>



### variable data\_typeid 

```C++
std::type_index zmbt::OutputRecorder::Registry::data_typeid;
```




<hr>



### variable enable\_categories\_ 

```C++
std::bitset<static_cast<unsigned>(ChannelKind::Undefined)> zmbt::OutputRecorder::Registry::enable_categories_;
```




<hr>



### variable extract\_fn 

```C++
consume_fn_t zmbt::OutputRecorder::Registry::extract_fn;
```




<hr>



### variable frame\_buff\_map 

```C++
std::shared_ptr<FramesBuffMap> zmbt::OutputRecorder::Registry::frame_buff_map;
```




<hr>



### variable interface\_name 

```C++
boost::json::string zmbt::OutputRecorder::Registry::interface_name;
```




<hr>



### variable lost\_count 

```C++
std::atomic<std::size_t> zmbt::OutputRecorder::Registry::lost_count;
```




<hr>



### variable serialized\_frames 

```C++
boost::json::array zmbt::OutputRecorder::Registry::serialized_frames;
```




<hr>
## Public Functions Documentation




### function Registry 

```C++
inline Registry::Registry (
    std::type_index typid,
    boost::json::string name,
    consume_fn_t efn,
    std::shared_ptr< FramesBuffMap > fbm
) 
```




<hr>



### function ~Registry 

```C++
inline Registry::~Registry () 
```




<hr>
## Public Static Functions Documentation




### function Make 

```C++
template<class Interface>
static inline std::shared_ptr< Registry > Registry::Make () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/output_recorder.hpp`

