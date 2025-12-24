

# Struct zmbt::mapping::TestDiagnostics



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**mapping**](namespacezmbt_1_1mapping.md) **>** [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md)



_Test diagnostics for test case execution._ 

* `#include <test_diagnostics.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Result**](#enum-result)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  boost::json::value | [**channel\_id**](#variable-channel_id)   = `{}`<br> |
|  boost::json::string | [**comment**](#variable-comment)   = `{}`<br> |
|  boost::json::string | [**description**](#variable-description)   = `{}`<br> |
|  boost::json::value | [**error**](#variable-error)   = `{}`<br> |
|  boost::json::array | [**eval\_stack**](#variable-eval_stack)   = `{}`<br> |
|  boost::json::value | [**expected**](#variable-expected)   = `{}`<br> |
|  boost::json::string | [**message**](#variable-message)   = `{}`<br> |
|  boost::json::string | [**model\_name**](#variable-model_name)  <br> |
|  boost::json::value | [**observed**](#variable-observed)   = `{}`<br> |
|  boost::json::value | [**pipe\_id**](#variable-pipe_id)   = `{}`<br> |
|  Result | [**result**](#variable-result)   = `{}`<br> |
|  bool | [**tabular\_condition\_failure\_**](#variable-tabular_condition_failure_)   = `{}`<br> |
|  size\_t | [**tc**](#variable-tc)   = `{}`<br> |
|  size\_t | [**tr**](#variable-tr)   = `{}`<br> |
|  boost::json::array | [**traces**](#variable-traces)   = `{}`<br> |
|  boost::json::value | [**vector**](#variable-vector)   = `{}`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**ChannelId**](#function-channelid) (boost::json::value const & id) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Comment**](#function-comment) (boost::json::string\_view msg) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Description**](#function-description) (boost::json::string\_view msg) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Error**](#function-error) (boost::json::string\_view origin, boost::json::value const & err) <br>_report test setup or execution error_  |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**EvalStack**](#function-evalstack) ([**lang::EvalLog**](structzmbt_1_1lang_1_1EvalLog.md) const & log) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Fail**](#function-fail) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & expected, boost::json::value const & observed) <br>_report test expectation failure_  |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**PipeId**](#function-pipeid) (boost::json::value const & id) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**TabularConditionFailure**](#function-tabularconditionfailure) (std::size\_t const col) <br> |
|   | [**TestDiagnostics**](#function-testdiagnostics-13) (boost::json::string\_view model\_name) <br> |
|   | [**TestDiagnostics**](#function-testdiagnostics-23) ([**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) const &) = default<br> |
|   | [**TestDiagnostics**](#function-testdiagnostics-33) ([**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) &&) = default<br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**TestRow**](#function-testrow) (std::size\_t const r) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Traces**](#function-traces) (std::shared\_ptr&lt; boost::json::array &gt; const traces\_ptr) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**Vector**](#function-vector) (boost::json::value val) <br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**operator=**](#function-operator) ([**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) const &) = default<br> |
|  [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) & | [**operator=**](#function-operator_1) ([**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) &&) = default<br> |
|  boost::json::value | [**to\_json**](#function-to_json) () const<br> |
|   | [**~TestDiagnostics**](#function-testdiagnostics) () = default<br> |




























## Public Types Documentation




### enum Result 

```C++
enum zmbt::mapping::TestDiagnostics::Result {
    Success,
    Fail,
    Error
};
```




<hr>
## Public Attributes Documentation




### variable channel\_id 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::channel_id;
```




<hr>



### variable comment 

```C++
boost::json::string zmbt::mapping::TestDiagnostics::comment;
```




<hr>



### variable description 

```C++
boost::json::string zmbt::mapping::TestDiagnostics::description;
```




<hr>



### variable error 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::error;
```




<hr>



### variable eval\_stack 

```C++
boost::json::array zmbt::mapping::TestDiagnostics::eval_stack;
```




<hr>



### variable expected 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::expected;
```




<hr>



### variable message 

```C++
boost::json::string zmbt::mapping::TestDiagnostics::message;
```




<hr>



### variable model\_name 

```C++
boost::json::string zmbt::mapping::TestDiagnostics::model_name;
```




<hr>



### variable observed 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::observed;
```




<hr>



### variable pipe\_id 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::pipe_id;
```




<hr>



### variable result 

```C++
Result zmbt::mapping::TestDiagnostics::result;
```




<hr>



### variable tabular\_condition\_failure\_ 

```C++
bool zmbt::mapping::TestDiagnostics::tabular_condition_failure_;
```




<hr>



### variable tc 

```C++
size_t zmbt::mapping::TestDiagnostics::tc;
```




<hr>



### variable tr 

```C++
size_t zmbt::mapping::TestDiagnostics::tr;
```




<hr>



### variable traces 

```C++
boost::json::array zmbt::mapping::TestDiagnostics::traces;
```




<hr>



### variable vector 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::vector;
```




<hr>
## Public Functions Documentation




### function ChannelId 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::ChannelId (
    boost::json::value const & id
) 
```




<hr>



### function Comment 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::Comment (
    boost::json::string_view msg
) 
```




<hr>



### function Description 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::Description (
    boost::json::string_view msg
) 
```




<hr>



### function Error 

_report test setup or execution error_ 
```C++
TestDiagnostics & zmbt::mapping::TestDiagnostics::Error (
    boost::json::string_view origin,
    boost::json::value const & err
) 
```




<hr>



### function EvalStack 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::EvalStack (
    lang::EvalLog const & log
) 
```




<hr>



### function Fail 

_report test expectation failure_ 
```C++
TestDiagnostics & zmbt::mapping::TestDiagnostics::Fail (
    lang::Expression const & expected,
    boost::json::value const & observed
) 
```




<hr>



### function PipeId 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::PipeId (
    boost::json::value const & id
) 
```




<hr>



### function TabularConditionFailure 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::TabularConditionFailure (
    std::size_t const col
) 
```




<hr>



### function TestDiagnostics [1/3]

```C++
inline zmbt::mapping::TestDiagnostics::TestDiagnostics (
    boost::json::string_view model_name
) 
```




<hr>



### function TestDiagnostics [2/3]

```C++
zmbt::mapping::TestDiagnostics::TestDiagnostics (
    TestDiagnostics const &
) = default
```




<hr>



### function TestDiagnostics [3/3]

```C++
zmbt::mapping::TestDiagnostics::TestDiagnostics (
    TestDiagnostics &&
) = default
```




<hr>



### function TestRow 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::TestRow (
    std::size_t const r
) 
```




<hr>



### function Traces 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::Traces (
    std::shared_ptr< boost::json::array > const traces_ptr
) 
```




<hr>



### function Vector 

```C++
inline TestDiagnostics & zmbt::mapping::TestDiagnostics::Vector (
    boost::json::value val
) 
```




<hr>



### function operator= 

```C++
TestDiagnostics & zmbt::mapping::TestDiagnostics::operator= (
    TestDiagnostics const &
) = default
```




<hr>



### function operator= 

```C++
TestDiagnostics & zmbt::mapping::TestDiagnostics::operator= (
    TestDiagnostics &&
) = default
```




<hr>



### function to\_json 

```C++
boost::json::value zmbt::mapping::TestDiagnostics::to_json () const
```




<hr>



### function ~TestDiagnostics 

```C++
zmbt::mapping::TestDiagnostics::~TestDiagnostics () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/test_diagnostics.hpp`

