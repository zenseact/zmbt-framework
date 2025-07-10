

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
|  FailureHandler | [**failure\_handler**](#variable-failure_handler)   = `{[**default\_test\_failure**](namespacezmbt.md#function-default_test_failure)}`<br> |
|  std::map&lt; [**object\_id**](classzmbt_1_1object__id.md), std::map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), std::map&lt; ChannelKind, std::map&lt; boost::json::string, std::pair&lt; Generator::Shared, [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) &gt; &gt; &gt; &gt; &gt; | [**input\_generators**](#variable-input_generators)  <br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**json\_data**](#variable-json_data)   = `{init\_json\_data()}`<br> |
|  mutex\_t | [**mutex**](#variable-mutex)  <br> |
|  shared\_data\_table | [**shared**](#variable-shared)  <br> |
|  std::map&lt; [**interface\_id**](classzmbt_1_1interface__id.md), [**TriggerIfc**](classzmbt_1_1TriggerIfc.md) &gt; | [**trigger\_ifcs**](#variable-trigger_ifcs)  <br> |
|  std::map&lt; [**object\_id**](classzmbt_1_1object__id.md), [**TriggerObj**](classzmbt_1_1TriggerObj.md) &gt; | [**trigger\_objs**](#variable-trigger_objs)  <br> |
|  std::map&lt; boost::json::string, [**Trigger**](classzmbt_1_1Trigger.md) &gt; | [**triggers**](#variable-triggers)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EnvironmentData**](#function-environmentdata-13) () <br> |
|   | [**EnvironmentData**](#function-environmentdata-23) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) && o) <br> |
|   | [**EnvironmentData**](#function-environmentdata-33) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) const & o) <br> |
|  [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) & | [**operator=**](#function-operator) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) && o) <br> |
|  [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) & | [**operator=**](#function-operator_1) ([**EnvironmentData**](structzmbt_1_1EnvironmentData.md) const & o) <br> |
| virtual  | [**~EnvironmentData**](#function-environmentdata) () <br> |


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



### variable failure\_handler 

```C++
FailureHandler zmbt::EnvironmentData::failure_handler;
```




<hr>



### variable input\_generators 

```C++
std::map<object_id, std::map<interface_id, std::map<ChannelKind, std::map<boost::json::string, std::pair<Generator::Shared, lang::Expression> > > > > zmbt::EnvironmentData::input_generators;
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



### variable shared 

```C++
shared_data_table zmbt::EnvironmentData::shared;
```




<hr>



### variable trigger\_ifcs 

```C++
std::map<interface_id, TriggerIfc> zmbt::EnvironmentData::trigger_ifcs;
```




<hr>



### variable trigger\_objs 

```C++
std::map<object_id, TriggerObj> zmbt::EnvironmentData::trigger_objs;
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
) 
```




<hr>



### function EnvironmentData [3/3]

```C++
zmbt::EnvironmentData::EnvironmentData (
    EnvironmentData const & o
) 
```




<hr>



### function operator= 

```C++
EnvironmentData & zmbt::EnvironmentData::operator= (
    EnvironmentData && o
) 
```




<hr>



### function operator= 

```C++
EnvironmentData & zmbt::EnvironmentData::operator= (
    EnvironmentData const & o
) 
```




<hr>



### function ~EnvironmentData 

```C++
virtual zmbt::EnvironmentData::~EnvironmentData () 
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

