

# Class zmbt::Environment::InterfaceHandle



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Environment**](classzmbt_1_1Environment.md) **>** [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md)



[_**Environment**_](classzmbt_1_1Environment.md) _extension handling the interface data._[More...](#detailed-description)

* `#include <environment_interface_record.hpp>`





Inherited by the following classes: [zmbt::Environment::TypedInterfaceHandle](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|  boost::json::array | [**CaptureSlice**](#function-captureslice) (boost::json::string\_view signal\_path) const<br> |
|  boost::json::array const & | [**Captures**](#function-captures) () const<br> |
|  void | [**EnableOutputRecordFor**](#function-enableoutputrecordfor) (ChannelKind const kind) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**Env**](#function-env) () const<br>[_**Environment**_](classzmbt_1_1Environment.md) _of the interface handle._ |
|  void | [**Inject**](#function-inject) (std::shared\_ptr&lt; [**Generator**](classzmbt_1_1Generator.md) &gt; gen, [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & tf, ChannelKind const kind, boost::json::string\_view jp="") <br>_Set generating function for injection._  |
|  void | [**InjectArgs**](#function-injectargs) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e, boost::json::string\_view jp="") <br>_Set generating function for injection args._  |
|  void | [**InjectReturn**](#function-injectreturn) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e, boost::json::string\_view jp="") <br>_Set generating function for injection return._  |
|   | [**InterfaceHandle**](#function-interfacehandle-18) ([**Environment**](classzmbt_1_1Environment.md) const & e, [**interface\_id**](classzmbt_1_1interface__id.md) const & interface, [**object\_id**](classzmbt_1_1object__id.md) refobj) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-28) ([**Environment**](classzmbt_1_1Environment.md) const & e, boost::json::string\_view ref) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-38) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, [**object\_id**](classzmbt_1_1object__id.md) refobj) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-48) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, nullptr\_t) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-58) (boost::json::string\_view ref) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-68) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, H const & obj) <br> |
|   | [**InterfaceHandle**](#function-interfacehandle-78) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) const &) = default<br> |
|   | [**InterfaceHandle**](#function-interfacehandle-88) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) && o) = default<br> |
|  void | [**MaybeThrowException**](#function-maybethrowexception) () <br>_throw exception if set for current call_  |
|  std::size\_t | [**ObservedCalls**](#function-observedcalls) () const<br>_Number of interface calls._  |
|  boost::json::array | [**PrototypeArgs**](#function-prototypeargs) () const<br> |
|  boost::json::value | [**PrototypeReturn**](#function-prototypereturn) () const<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**RunAsAction**](#function-runasaction) () <br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**RunAsTrigger**](#function-runastrigger) (std::size\_t const repeats=1) <br> |
|  boost::json::value | [**YieldInjection**](#function-yieldinjection) (ChannelKind const kind) <br>_Yield input generator._  |
|  boost::json::value | [**YieldInjectionArgs**](#function-yieldinjectionargs) () <br>_Injection at nofcall._  |
|  boost::json::value | [**YieldInjectionReturn**](#function-yieldinjectionreturn) () <br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**interface**](#function-interface) () const<br> |
|  boost::json::string const & | [**key**](#function-key) () const<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**operator=**](#function-operator) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) const &) = default<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**operator=**](#function-operator_1) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) &&) = default<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**refobj**](#function-refobj) () const<br> |
|   | [**~InterfaceHandle**](#function-interfacehandle) () = default<br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**zmbt::Environment**](classzmbt_1_1Environment.md) | [**env**](#variable-env)  <br> |
|  std::shared\_ptr&lt; [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &gt; | [**output\_recorder\_**](#variable-output_recorder_)  <br> |




















## Detailed Description




**Template parameters:**


* `Interface` 




    
## Public Functions Documentation




### function CaptureSlice 

```C++
boost::json::array zmbt::Environment::InterfaceHandle::CaptureSlice (
    boost::json::string_view signal_path
) const
```




<hr>



### function Captures 

```C++
boost::json::array const & zmbt::Environment::InterfaceHandle::Captures () const
```




<hr>



### function EnableOutputRecordFor 

```C++
void zmbt::Environment::InterfaceHandle::EnableOutputRecordFor (
    ChannelKind const kind
) 
```




<hr>



### function Env 

[_**Environment**_](classzmbt_1_1Environment.md) _of the interface handle._
```C++
inline Environment & zmbt::Environment::InterfaceHandle::Env () const
```





**Returns:**

[**zmbt::Environment**](classzmbt_1_1Environment.md)& 





        

<hr>



### function Inject 

_Set generating function for injection._ 
```C++
void zmbt::Environment::InterfaceHandle::Inject (
    std::shared_ptr< Generator > gen,
    lang::Expression const & tf,
    ChannelKind const kind,
    boost::json::string_view jp=""
) 
```





**Parameters:**


* `e` input generator 
* `group` return \| args \| exception 
* `jp` JSON Pointer 




        

<hr>



### function InjectArgs 

_Set generating function for injection args._ 
```C++
inline void zmbt::Environment::InterfaceHandle::InjectArgs (
    lang::Expression const & e,
    boost::json::string_view jp=""
) 
```



The intput conditions are evaluated from the corresponding expressions with 0-based call index as an argument. 

**Parameters:**


* `e` 
* `jp` JSON Pointer 




        

<hr>



### function InjectReturn 

_Set generating function for injection return._ 
```C++
inline void zmbt::Environment::InterfaceHandle::InjectReturn (
    lang::Expression const & e,
    boost::json::string_view jp=""
) 
```



The intput conditions are evaluated from the corresponding expressions with 0-based call index as an argument. 

**Parameters:**


* `e` generating funcion expression 
* `jp` JSON Pointer 




        

<hr>



### function InterfaceHandle [1/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    Environment const & e,
    interface_id const & interface,
    object_id refobj
) 
```




<hr>



### function InterfaceHandle [2/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    Environment const & e,
    boost::json::string_view ref
) 
```




<hr>



### function InterfaceHandle [3/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    interface_id const & interface,
    object_id refobj
) 
```




<hr>



### function InterfaceHandle [4/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    interface_id const & interface,
    nullptr_t
) 
```




<hr>



### function InterfaceHandle [5/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    boost::json::string_view ref
) 
```




<hr>



### function InterfaceHandle [6/8]

```C++
template<class H, class E>
inline zmbt::Environment::InterfaceHandle::InterfaceHandle (
    interface_id const & interface,
    H const & obj
) 
```




<hr>



### function InterfaceHandle [7/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    InterfaceHandle const &
) = default
```




<hr>



### function InterfaceHandle [8/8]

```C++
zmbt::Environment::InterfaceHandle::InterfaceHandle (
    InterfaceHandle && o
) = default
```




<hr>



### function MaybeThrowException 

_throw exception if set for current call_ 
```C++
void zmbt::Environment::InterfaceHandle::MaybeThrowException () 
```




<hr>



### function ObservedCalls 

_Number of interface calls._ 
```C++
std::size_t zmbt::Environment::InterfaceHandle::ObservedCalls () const
```




<hr>



### function PrototypeArgs 

```C++
boost::json::array zmbt::Environment::InterfaceHandle::PrototypeArgs () const
```




<hr>



### function PrototypeReturn 

```C++
boost::json::value zmbt::Environment::InterfaceHandle::PrototypeReturn () const
```




<hr>



### function RunAsAction 

```C++
InterfaceHandle & zmbt::Environment::InterfaceHandle::RunAsAction () 
```




<hr>



### function RunAsTrigger 

```C++
InterfaceHandle & zmbt::Environment::InterfaceHandle::RunAsTrigger (
    std::size_t const repeats=1
) 
```




<hr>



### function YieldInjection 

_Yield input generator._ 
```C++
boost::json::value zmbt::Environment::InterfaceHandle::YieldInjection (
    ChannelKind const kind
) 
```




<hr>



### function YieldInjectionArgs 

_Injection at nofcall._ 
```C++
inline boost::json::value zmbt::Environment::InterfaceHandle::YieldInjectionArgs () 
```




<hr>



### function YieldInjectionReturn 

```C++
inline boost::json::value zmbt::Environment::InterfaceHandle::YieldInjectionReturn () 
```




<hr>



### function interface 

```C++
inline interface_id zmbt::Environment::InterfaceHandle::interface () const
```




<hr>



### function key 

```C++
boost::json::string const & zmbt::Environment::InterfaceHandle::key () const
```




<hr>



### function operator= 

```C++
InterfaceHandle & zmbt::Environment::InterfaceHandle::operator= (
    InterfaceHandle const &
) = default
```




<hr>



### function operator= 

```C++
InterfaceHandle & zmbt::Environment::InterfaceHandle::operator= (
    InterfaceHandle &&
) = default
```




<hr>



### function refobj 

```C++
inline object_id zmbt::Environment::InterfaceHandle::refobj () const
```




<hr>



### function ~InterfaceHandle 

```C++
zmbt::Environment::InterfaceHandle::~InterfaceHandle () = default
```




<hr>
## Protected Attributes Documentation




### variable env 

```C++
zmbt::Environment zmbt::Environment::InterfaceHandle::env;
```




<hr>



### variable output\_recorder\_ 

```C++
std::shared_ptr<OutputRecorder> zmbt::Environment::InterfaceHandle::output_recorder_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/environment_interface_record.hpp`

