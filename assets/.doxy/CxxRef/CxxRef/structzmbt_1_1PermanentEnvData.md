

# Struct zmbt::PermanentEnvData



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**PermanentEnvData**](structzmbt_1_1PermanentEnvData.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void([**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &)&gt; | [**SetupOutputRecorder**](#typedef-setupoutputrecorder)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  boost::concurrent\_flat\_map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**object\_id**](classzmbt_1_1object__id.md) &gt; | [**default\_objects**](#variable-default_objects)   = `{}`<br> |
|  boost::concurrent\_flat\_map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), SetupOutputRecorder &gt; | [**output\_recorder\_factories**](#variable-output_recorder_factories)   = `{}`<br> |
|  boost::concurrent\_flat\_map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**reflect::Prototypes**](classzmbt_1_1reflect_1_1Prototypes.md) &gt; | [**prototypes**](#variable-prototypes)   = `{}`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  boost::optional&lt; [**object\_id**](classzmbt_1_1object__id.md) &gt; | [**get\_default\_object**](#function-get_default_object) ([**interface\_id**](classzmbt_1_1interface__id.md) const &) const<br> |
|  boost::optional&lt; SetupOutputRecorder &gt; | [**get\_output\_recorder\_factory**](#function-get_output_recorder_factory) ([**interface\_id**](classzmbt_1_1interface__id.md) const &) const<br> |
|  boost::optional&lt; [**reflect::Prototypes**](classzmbt_1_1reflect_1_1Prototypes.md) &gt; | [**get\_prototypes**](#function-get_prototypes) ([**interface\_id**](classzmbt_1_1interface__id.md) const &) const<br> |




























## Public Types Documentation




### typedef SetupOutputRecorder 

```C++
using zmbt::PermanentEnvData::SetupOutputRecorder =  std::function<void(OutputRecorder&)>;
```




<hr>
## Public Attributes Documentation




### variable default\_objects 

```C++
boost::concurrent_flat_map<interface_id, object_id> zmbt::PermanentEnvData::default_objects;
```




<hr>



### variable output\_recorder\_factories 

```C++
boost::concurrent_flat_map<interface_id, SetupOutputRecorder> zmbt::PermanentEnvData::output_recorder_factories;
```




<hr>



### variable prototypes 

```C++
boost::concurrent_flat_map<interface_id, reflect::Prototypes> zmbt::PermanentEnvData::prototypes;
```




<hr>
## Public Functions Documentation




### function get\_default\_object 

```C++
boost::optional< object_id > zmbt::PermanentEnvData::get_default_object (
    interface_id const &
) const
```




<hr>



### function get\_output\_recorder\_factory 

```C++
boost::optional< SetupOutputRecorder > zmbt::PermanentEnvData::get_output_recorder_factory (
    interface_id const &
) const
```




<hr>



### function get\_prototypes 

```C++
boost::optional< reflect::Prototypes > zmbt::PermanentEnvData::get_prototypes (
    interface_id const &
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/permanent_data.hpp`

