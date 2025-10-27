

# Struct zmbt::EnvironmentData



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**EnvironmentData**](structzmbt_1_1EnvironmentData.md)



_Data container for the_ [_**Environment**_](classzmbt_1_1Environment.md) _._

* `#include <environment_data.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void(boost::json::value const  &)&gt; | [**FailureHandler**](#typedef-failurehandler)  <br> |
| typedef std::unique\_lock&lt; mutex\_t &gt; | [**lock\_t**](#typedef-lock_t)  <br> |
| typedef std::recursive\_mutex | [**mutex\_t**](#typedef-mutex_t)  <br> |
| typedef std::pair&lt; std::type\_index, std::shared\_ptr&lt; void &gt; &gt; | [**shared\_data\_record**](#typedef-shared_data_record)  <br> |
| typedef std::map&lt; boost::json::string, shared\_data\_record &gt; | [**shared\_data\_table**](#typedef-shared_data_table)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::map&lt; boost::json::string, std::function&lt; void()&gt; &gt; | [**callbacks**](#variable-callbacks)  <br> |
|  std::atomic\_bool | [**has\_test\_error**](#variable-has_test_error)   = `{false}`<br> |
|  boost::concurrent\_flat\_map&lt; std::pair&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**object\_id**](classzmbt_1_1object__id.md) &gt;, [**shared\_resource**](classzmbt_1_1shared__resource.md)&lt; [**InjectionTable**](classzmbt_1_1InjectionTable.md) &gt; &gt; | [**injection\_tables**](#variable-injection_tables)   = `{}`<br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**json\_data**](#variable-json_data)   = `{init\_json\_data()}`<br> |
|  mutex\_t | [**mutex**](#variable-mutex)  <br> |
|  boost::concurrent\_flat\_map&lt; std::pair&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**object\_id**](classzmbt_1_1object__id.md) &gt;, [**shared\_resource**](classzmbt_1_1shared__resource.md)&lt; [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &gt; &gt; | [**output\_recorders**](#variable-output_recorders)   = `{}`<br> |
|  shared\_data\_table | [**shared**](#variable-shared)  <br> |
|  std::unordered\_map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**TriggerIfc**](classzmbt_1_1TriggerIfc.md) &gt; | [**trigger\_ifcs**](#variable-trigger_ifcs)  <br> |
|  std::unordered\_map&lt; [**object\_id**](classzmbt_1_1object__id.md), [**TriggerObj**](classzmbt_1_1TriggerObj.md) &gt; | [**trigger\_objs**](#variable-trigger_objs)  <br> |
|  std::map&lt; boost::json::string, [**Trigger**](classzmbt_1_1Trigger.md) &gt; | [**triggers**](#variable-triggers)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EnvironmentData**](#function-environmentdata-13) () <br> |
|   | [**EnvironmentData**](#function-environmentdata-23) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) && o) = delete<br> |
|   | [**EnvironmentData**](#function-environmentdata-33) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) const & o) = delete<br> |
|  [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) & | [**operator=**](#function-operator) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) && o) = delete<br> |
|  [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) & | [**operator=**](#function-operator_1) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) const & o) = delete<br> |
|   | [**~EnvironmentData**](#function-environmentdata) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  boost::json::value | [**init\_json\_data**](#function-init_json_data) () <br> |


























## Public Types Documentation




### typedef FailureHandler 

```C++
using zmbt::EnvironmentData::FailureHandler =  std::function<void(boost::json::value const&)>;
```




<hr>



### typedef lock\_t 

```C++
using zmbt::EnvironmentData::lock_t =  std::unique_lock<mutex_t>;
```




<hr>



### typedef mutex\_t 

```C++
using zmbt::EnvironmentData::mutex_t =  std::recursive_mutex;
```




<hr>



### typedef shared\_data\_record 

```C++
using zmbt::EnvironmentData::shared_data_record =  std::pair<std::type_index, std::shared_ptr<void> >;
```




<hr>



### typedef shared\_data\_table 

```C++
using zmbt::EnvironmentData::shared_data_table =  std::map<boost::json::string, shared_data_record>;
```




<hr>
## Public Attributes Documentation




### variable callbacks 

```C++
std::map<boost::json::string, std::function<void()> > zmbt::EnvironmentData::callbacks;
```




<hr>



### variable has\_test\_error 

```C++
std::atomic_bool zmbt::EnvironmentData::has_test_error;
```




<hr>



### variable injection\_tables 

```C++
boost::concurrent_flat_map< std::pair<interface_id, object_id>, shared_resource<InjectionTable> > zmbt::EnvironmentData::injection_tables;
```




<hr>



### variable json\_data 

```C++
JsonNode zmbt::EnvironmentData::json_data;
```




<hr>



### variable mutex 

```C++
mutex_t zmbt::EnvironmentData::mutex;
```




<hr>



### variable output\_recorders 

```C++
boost::concurrent_flat_map< std::pair<interface_id, object_id>, shared_resource<OutputRecorder> > zmbt::EnvironmentData::output_recorders;
```




<hr>



### variable shared 

```C++
shared_data_table zmbt::EnvironmentData::shared;
```




<hr>



### variable trigger\_ifcs 

```C++
std::unordered_map<interface_id, TriggerIfc> zmbt::EnvironmentData::trigger_ifcs;
```




<hr>



### variable trigger\_objs 

```C++
std::unordered_map<object_id, TriggerObj> zmbt::EnvironmentData::trigger_objs;
```




<hr>



### variable triggers 

```C++
std::map<boost::json::string, Trigger> zmbt::EnvironmentData::triggers;
```




<hr>
## Public Functions Documentation




### function EnvironmentData [1/3]

```C++
zmbt::EnvironmentData::EnvironmentData () 
```




<hr>



### function EnvironmentData [2/3]

```C++
zmbt::EnvironmentData::EnvironmentData (
    EnvironmentData && o
) = delete
```




<hr>



### function EnvironmentData [3/3]

```C++
zmbt::EnvironmentData::EnvironmentData (
    EnvironmentData const & o
) = delete
```




<hr>



### function operator= 

```C++
EnvironmentData & zmbt::EnvironmentData::operator= (
    EnvironmentData && o
) = delete
```




<hr>



### function operator= 

```C++
EnvironmentData & zmbt::EnvironmentData::operator= (
    EnvironmentData const & o
) = delete
```




<hr>



### function ~EnvironmentData 

```C++
zmbt::EnvironmentData::~EnvironmentData () 
```




<hr>
## Public Static Functions Documentation




### function init\_json\_data 

```C++
static boost::json::value zmbt::EnvironmentData::init_json_data () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/environment_data.hpp`

