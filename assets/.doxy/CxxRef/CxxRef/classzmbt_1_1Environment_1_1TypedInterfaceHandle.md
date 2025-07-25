

# Class zmbt::Environment::TypedInterfaceHandle

**template &lt;class Interface&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Environment**](classzmbt_1_1Environment.md) **>** [**TypedInterfaceHandle**](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md)



[_**Environment**_](classzmbt_1_1Environment.md) _API handler for specific interface._

* `#include <environment_interface_record.hpp>`



Inherits the following classes: [zmbt::Environment::InterfaceHandle](classzmbt_1_1Environment_1_1InterfaceHandle.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|  return\_t | [**Hook**](#function-hook-12) (hookout\_args\_t args) <br>_Hook the producing interface call to the environment._  |
|  return\_t | [**Hook**](#function-hook-22) (A &&... arg) <br>_Hook the producing interface call to the environment._  |
|   | [**TypedInterfaceHandle**](#function-typedinterfacehandle-13) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, H const & refobj) <br> |
|   | [**TypedInterfaceHandle**](#function-typedinterfacehandle-23) ([**TypedInterfaceHandle**](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md) const &) = default<br> |
|   | [**TypedInterfaceHandle**](#function-typedinterfacehandle-33) ([**TypedInterfaceHandle**](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md) &&) = default<br> |
|   | [**~TypedInterfaceHandle**](#function-typedinterfacehandle) () = default<br> |


## Public Functions inherited from zmbt::Environment::InterfaceHandle

See [zmbt::Environment::InterfaceHandle](classzmbt_1_1Environment_1_1InterfaceHandle.md)

| Type | Name |
| ---: | :--- |
|  boost::json::array | [**CaptureSlice**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-captureslice) (boost::json::string\_view signal\_path, int start=0, int stop=-1, int const step=1) const<br> |
|  boost::json::array const & | [**Captures**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-captures) () const<br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**Env**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-env) () const<br>[_**Environment**_](classzmbt_1_1Environment.md) _of the interface handle._ |
|  void | [**Inject**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-inject) (std::shared\_ptr&lt; [**Generator**](classzmbt_1_1Generator.md) &gt; gen, [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & tf, ChannelKind const kind, boost::json::string\_view jp="") <br>_Set generating function for injection._  |
|  void | [**InjectArgs**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-injectargs) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e, boost::json::string\_view jp="") <br>_Set generating function for injection args._  |
|  void | [**InjectReturn**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-injectreturn) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e, boost::json::string\_view jp="") <br>_Set generating function for injection return._  |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-18) ([**Environment**](classzmbt_1_1Environment.md) const & e, [**interface\_id**](classzmbt_1_1interface__id.md) const & interface, [**object\_id**](classzmbt_1_1object__id.md) refobj) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-28) ([**Environment**](classzmbt_1_1Environment.md) const & e, boost::json::string\_view ref) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-38) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, [**object\_id**](classzmbt_1_1object__id.md) refobj) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-48) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, nullptr\_t) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-58) (boost::json::string\_view ref) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-68) ([**interface\_id**](classzmbt_1_1interface__id.md) const & interface, H const & obj) <br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-78) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) const &) = default<br> |
|   | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle-88) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) && o) = default<br> |
|  boost::json::array | [**ObservedArgs**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-observedargs) (int const nofcall=-1) <br>_Input arguments observed at nofcall._  |
|  std::size\_t | [**ObservedCalls**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-observedcalls) () const<br>_Number of interface calls._  |
|  boost::json::value | [**ObservedReturn**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-observedreturn) (int const nofcall=-1) const<br>_Observed return value at nofcall._  |
|  boost::json::value const & | [**PrototypeArgs**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-prototypeargs) () const<br> |
|  boost::json::value const & | [**PrototypeReturn**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-prototypereturn) () const<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**RunAsAction**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-runasaction) () <br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**RunAsTrigger**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-runastrigger) (std::size\_t const nofcall=0) <br> |
|  boost::json::value | [**YieldInjection**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-yieldinjection) (ChannelKind const kind) <br>_Yield input generator._  |
|  boost::json::value | [**YieldInjectionArgs**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-yieldinjectionargs) () <br>_Injection at nofcall._  |
|  boost::json::value | [**YieldInjectionReturn**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-yieldinjectionreturn) () <br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**interface**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interface) () const<br> |
|  boost::json::string const & | [**key**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-key) () const<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**operator=**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-operator) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) const &) = default<br> |
|  [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) & | [**operator=**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-operator_1) ([**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) &&) = default<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**refobj**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-refobj) () const<br> |
|   | [**~InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md#function-interfacehandle) () = default<br> |
















## Protected Attributes inherited from zmbt::Environment::InterfaceHandle

See [zmbt::Environment::InterfaceHandle](classzmbt_1_1Environment_1_1InterfaceHandle.md)

| Type | Name |
| ---: | :--- |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) | [**captures**](classzmbt_1_1Environment_1_1InterfaceHandle.md#variable-captures)  <br> |
|  [**zmbt::Environment**](classzmbt_1_1Environment.md) | [**env**](classzmbt_1_1Environment_1_1InterfaceHandle.md#variable-env)  <br> |






































## Public Functions Documentation




### function Hook [1/2]

_Hook the producing interface call to the environment._ 
```C++
inline return_t zmbt::Environment::TypedInterfaceHandle::Hook (
    hookout_args_t args
) 
```



Register input arguments, update the output reference arguments, increment call count and return corresponding env value




**Parameters:**


* `args` 



**Returns:**

return\_t 





        

<hr>



### function Hook [2/2]

_Hook the producing interface call to the environment._ 
```C++
template<class... A>
inline return_t zmbt::Environment::TypedInterfaceHandle::Hook (
    A &&... arg
) 
```



Register input arguments, update the output reference arguments, increment call count and return corresponding env value




**Template parameters:**


* `A...` compatible arg types 



**Parameters:**


* `arg` 



**Returns:**

return\_t 





        

<hr>



### function TypedInterfaceHandle [1/3]

```C++
template<class H>
inline zmbt::Environment::TypedInterfaceHandle::TypedInterfaceHandle (
    interface_id const & interface,
    H const & refobj
) 
```




<hr>



### function TypedInterfaceHandle [2/3]

```C++
zmbt::Environment::TypedInterfaceHandle::TypedInterfaceHandle (
    TypedInterfaceHandle const &
) = default
```




<hr>



### function TypedInterfaceHandle [3/3]

```C++
zmbt::Environment::TypedInterfaceHandle::TypedInterfaceHandle (
    TypedInterfaceHandle &&
) = default
```




<hr>



### function ~TypedInterfaceHandle 

```C++
zmbt::Environment::TypedInterfaceHandle::~TypedInterfaceHandle () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/environment_interface_record.hpp`

