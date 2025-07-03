

# Class zmbt::Environment



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Environment**](classzmbt_1_1Environment.md)



_Controlled environment data storage._ [More...](#detailed-description)

* `#include <environment.hpp>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) <br>[_**Environment**_](classzmbt_1_1Environment.md) _extension handling the interface data._ |
| class | [**TypedInterfaceHandle**](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md) &lt;class Interface&gt;<br>[_**Environment**_](classzmbt_1_1Environment.md) _API handler for specific interface._ |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**PersistentConfig**](structzmbt_1_1Environment_1_1PersistentConfig.md) | [**Config**](#function-config) () const<br> |
|  bool | [**ContainsShared**](#function-containsshared) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr) const<br>_Check if shared variable exists._  |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**DefaultObjectId**](#function-defaultobjectid) ([**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id) const<br> |
|  lock\_t | [**DeferLock**](#function-deferlock) () const<br>_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock with std::defer\_lock tag._ |
|  void | [**DumpJsonData**](#function-dumpjsondata) (std::ostream & os) <br>_Dump current environment state in prettified JSON._  |
|   | [**Environment**](#function-environment-13) () <br> |
|   | [**Environment**](#function-environment-23) ([**Environment**](classzmbt_1_1Environment.md) &&) = default<br> |
|   | [**Environment**](#function-environment-33) ([**Environment**](classzmbt_1_1Environment.md) const &) = default<br> |
|  boost::json::string | [**GetOrRegisterInterface**](#function-getorregisterinterface) ([**object\_id**](classzmbt_1_1object__id.md) const & obj\_id, [**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id) <br> |
|  boost::json::string | [**GetOrRegisterParametricTrigger**](#function-getorregisterparametrictrigger) ([**object\_id**](classzmbt_1_1object__id.md) const & obj\_id, [**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id) <br> |
|  std::shared\_ptr&lt; T &gt; | [**GetShared**](#function-getshared) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr) const<br>_Get the shared data associated with object and string key._  |
|  T & | [**GetSharedRef**](#function-getsharedref) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, A &&... args) <br>_Get reference to shared var, creating it if necessary._  |
|  boost::json::value | [**GetVar**](#function-getvar-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr) <br>_Get the environment variable._  |
|  T | [**GetVar**](#function-getvar-22) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr) <br> |
|  boost::json::value | [**GetVarOrDefault**](#function-getvarordefault-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, boost::json::value default\_value={}) <br>_Get the environment variable or use default._  |
|  T | [**GetVarOrDefault**](#function-getvarordefault-22) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, T default\_value=[**reflect::signal\_traits**](structzmbt_1_1reflect_1_1signal__traits.md)&lt; T &gt;::init()) <br> |
|  boost::json::value | [**GetVarOrUpdate**](#function-getvarorupdate-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, boost::json::value update\_value) <br>_Get or update the environment variable._  |
|  T | [**GetVarOrUpdate**](#function-getvarorupdate-22) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, T update\_value) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**HandleTestFailure**](#function-handletestfailure) (boost::json::value const & diagnostics) <br> |
|  bool | [**HasAction**](#function-hasaction) (boost::json::string\_view key) const<br> |
|  bool | [**HasTrigger**](#function-hastrigger) (boost::json::string\_view key) const<br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**InterfaceId**](#function-interfaceid) (boost::json::string\_view interface\_key) const<br> |
|  lock\_t | [**Lock**](#function-lock) () const<br>_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock._ |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**ObjectId**](#function-objectid) (boost::json::string\_view interface\_key) const<br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterAction**](#function-registeraction) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, std::function&lt; void()&gt; action) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterAnonymousInterface**](#function-registeranonymousinterface-12) ([**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id) <br> |
|  enable\_if\_t&lt; is\_ifc\_handle&lt; I &gt;::value, [**Environment**](classzmbt_1_1Environment.md) & &gt; | [**RegisterAnonymousInterface**](#function-registeranonymousinterface-22) (I && interface, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id=[**object\_id**](classzmbt_1_1object__id.md){[**ifc\_host\_nullptr**](namespacezmbt.md#variable-ifc_host_nullptr)&lt; I &gt;}) <br> |
|  boost::json::string | [**RegisterAnonymousTrigger**](#function-registeranonymoustrigger) (I && interface, H && host) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterInterface**](#function-registerinterface-12) (boost::json::string\_view key, [**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id) <br>_Register interface to enable FFI in the test model runners._  |
|  enable\_if\_t&lt; is\_ifc\_handle&lt; I &gt;::value, [**Environment**](classzmbt_1_1Environment.md) & &gt; | [**RegisterInterface**](#function-registerinterface-22) (boost::json::string\_view key, I && interface, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id=[**object\_id**](classzmbt_1_1object__id.md){[**ifc\_host\_nullptr**](namespacezmbt.md#variable-ifc_host_nullptr)&lt; I &gt;}) <br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**RegisterParametricTriggerIfc**](#function-registerparametrictriggerifc) (I && interface) <br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**RegisterParametricTriggerObj**](#function-registerparametrictriggerobj) (T && obj) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterPrototypes**](#function-registerprototypes) (I && interface) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterTrigger**](#function-registertrigger-12) (boost::json::string\_view key, I && interface, H && host) <br>_Register test trigger to enable FFI in the test model runners._  |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RegisterTrigger**](#function-registertrigger-22) (boost::json::string\_view key, I && interface) <br> |
|  void | [**ResetAll**](#function-resetall) () <br>_Clear all data._  |
|  void | [**ResetAllFor**](#function-resetallfor) ([**object\_id**](classzmbt_1_1object__id.md) obj) <br>_Clear data associated with the object._  |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**ResetFailureHandler**](#function-resetfailurehandler) () <br>_Reset the test handler to default._  |
|  void | [**ResetInterfaceData**](#function-resetinterfacedata) () <br>_Clear all interface data (accessed via ArgsFor/ReturnFor)_  |
|  void | [**ResetInterfaceDataFor**](#function-resetinterfacedatafor) ([**object\_id**](classzmbt_1_1object__id.md) obj) <br>_Clear interface data associated with the object (accessed via ArgsFor/ReturnFor)_  |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**RunAction**](#function-runaction) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr) <br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**SetFailureHandler**](#function-setfailurehandler) (std::function&lt; void(boost::json::value const &)&gt; const & fn) <br>_Set custom test failure handler._  |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**SetPrettyPrint**](#function-setprettyprint) (bool const pretty\_print=true) <br>_Set pretty print JSON values._  |
|  void | [**SetShared**](#function-setshared) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, std::shared\_ptr&lt; T &gt; data) <br>_Set the shared data associated with key._  |
|  void | [**SetVar**](#function-setvar-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, boost::json::value var) <br>_Set the environment variable._  |
|  void | [**SetVar**](#function-setvar-22) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & key\_expr, T var) <br> |
|  lock\_t | [**TryLock**](#function-trylock) () const<br>_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock with std::try\_to\_lock tag._ |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) & | [**json\_data**](#function-json_data-12) () <br> |
|  [**JsonNode**](classzmbt_1_1JsonNode.md) const & | [**json\_data**](#function-json_data-22) () const<br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**operator=**](#function-operator) ([**Environment**](classzmbt_1_1Environment.md) &&) = default<br> |
|  [**Environment**](classzmbt_1_1Environment.md) & | [**operator=**](#function-operator_1) ([**Environment**](classzmbt_1_1Environment.md) const &) = default<br> |
| virtual  | [**~Environment**](#function-environment) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  boost::json::string | [**autokey**](#function-autokey) ([**object\_id**](classzmbt_1_1object__id.md) const & obj\_id, [**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id) <br> |


## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void(boost::json::value const  &)&gt; | [**FailureHandler**](#typedef-failurehandler)  <br> |
| typedef [**reflect::invocation\_args\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_args_t)&lt; ifc\_pointer\_t&lt; T &gt; &gt; | [**argsref\_t**](#typedef-argsref_t)  <br> |
| typedef mp\_transform&lt; rvref\_to\_val, argsref\_t&lt; I &gt; &gt; | [**hookout\_args\_t**](#typedef-hookout_args_t)  <br> |
| typedef typename EnvironmentData::lock\_t | [**lock\_t**](#typedef-lock_t)  <br> |
| typedef [**reflect::invocation\_ret\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_ret_t)&lt; ifc\_pointer\_t&lt; T &gt; &gt; | [**return\_t**](#typedef-return_t)  <br> |
| typedef mp\_if&lt; std::is\_rvalue\_reference&lt; T &gt;, std::remove\_reference\_t&lt; T &gt;, T &gt; | [**rvref\_to\_val**](#typedef-rvref_to_val)  <br> |
| typedef [**reflect::invocation\_args\_unqf\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_args_unqf_t)&lt; ifc\_pointer\_t&lt; T &gt; &gt; | [**unqf\_args\_t**](#typedef-unqf_args_t)  <br> |




## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**PersistentConfig**](structzmbt_1_1Environment_1_1PersistentConfig.md) &gt; | [**config\_**](#variable-config_)  <br> |
|  std::shared\_ptr&lt; [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) &gt; | [**data\_**](#variable-data_)  <br> |




















## Detailed Description


Handles signal values and calls counters to/from environment, using interface pointers as keys 

**See also:** [Environment API](/user-guide/environment/) 



    
## Public Functions Documentation




### function Config 

```C++
PersistentConfig zmbt::Environment::Config () const
```




<hr>



### function ContainsShared 

_Check if shared variable exists._ 
```C++
bool zmbt::Environment::ContainsShared (
    lang::Expression const & key_expr
) const
```




<hr>



### function DefaultObjectId 

```C++
object_id zmbt::Environment::DefaultObjectId (
    interface_id const & ifc_id
) const
```




<hr>



### function DeferLock 

_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock with std::defer\_lock tag._
```C++
lock_t zmbt::Environment::DeferLock () const
```




<hr>



### function DumpJsonData 

_Dump current environment state in prettified JSON._ 
```C++
void zmbt::Environment::DumpJsonData (
    std::ostream & os
) 
```





**Parameters:**


* `os` output stream 




        

<hr>



### function Environment [1/3]

```C++
zmbt::Environment::Environment () 
```




<hr>



### function Environment [2/3]

```C++
zmbt::Environment::Environment (
    Environment &&
) = default
```




<hr>



### function Environment [3/3]

```C++
zmbt::Environment::Environment (
    Environment const &
) = default
```




<hr>



### function GetOrRegisterInterface 

```C++
boost::json::string zmbt::Environment::GetOrRegisterInterface (
    object_id const & obj_id,
    interface_id const & ifc_id
) 
```




<hr>



### function GetOrRegisterParametricTrigger 

```C++
boost::json::string zmbt::Environment::GetOrRegisterParametricTrigger (
    object_id const & obj_id,
    interface_id const & ifc_id
) 
```




<hr>



### function GetShared 

_Get the shared data associated with object and string key._ 
```C++
template<class T>
inline std::shared_ptr< T > zmbt::Environment::GetShared (
    lang::Expression const & key_expr
) const
```



Dynamic polymorphism is not supported, as the type safety is ensured by comparing typeid(T). If the type T is not exactly the same as was used on corresponding SetShared call, the method will throw [**environment\_error**](structzmbt_1_1environment__error.md).


If corresponding data was not set, the metod return nullptr.




**Template parameters:**


* `T` 
* `A` 



**Parameters:**


* `fmtstr` key format string 
* `arg` key format arguments 



**Returns:**

std::shared\_ptr&lt;T&gt; 





        

<hr>



### function GetSharedRef 

_Get reference to shared var, creating it if necessary._ 
```C++
template<class T, class... A>
inline T & zmbt::Environment::GetSharedRef (
    lang::Expression const & key_expr,
    A &&... args
) 
```





**Template parameters:**


* `T` 
* `...A` Constructor args for initial value 



**Parameters:**


* `key_expr` 



**Returns:**







        

<hr>



### function GetVar [1/2]

_Get the environment variable._ 
```C++
boost::json::value zmbt::Environment::GetVar (
    lang::Expression const & key_expr
) 
```





**Template parameters:**


* `T` 



**Parameters:**


* `obj` 
* `key` 



**Returns:**

T 





        

<hr>



### function GetVar [2/2]

```C++
template<class T>
inline T zmbt::Environment::GetVar (
    lang::Expression const & key_expr
) 
```




<hr>



### function GetVarOrDefault [1/2]

_Get the environment variable or use default._ 
```C++
boost::json::value zmbt::Environment::GetVarOrDefault (
    lang::Expression const & key_expr,
    boost::json::value default_value={}
) 
```





**Template parameters:**


* `T` 



**Parameters:**


* `obj` 
* `key` 
* `default_value` 



**Returns:**

T 





        

<hr>



### function GetVarOrDefault [2/2]

```C++
template<class T>
inline T zmbt::Environment::GetVarOrDefault (
    lang::Expression const & key_expr,
    T default_value=reflect::signal_traits < T >::init()
) 
```




<hr>



### function GetVarOrUpdate [1/2]

_Get or update the environment variable._ 
```C++
boost::json::value zmbt::Environment::GetVarOrUpdate (
    lang::Expression const & key_expr,
    boost::json::value update_value
) 
```





**Template parameters:**


* `T` 



**Parameters:**


* `obj` 
* `key` 
* `update_value` 



**Returns:**

T 





        

<hr>



### function GetVarOrUpdate [2/2]

```C++
template<class T>
inline T zmbt::Environment::GetVarOrUpdate (
    lang::Expression const & key_expr,
    T update_value
) 
```




<hr>



### function HandleTestFailure 

```C++
Environment & zmbt::Environment::HandleTestFailure (
    boost::json::value const & diagnostics
) 
```




<hr>



### function HasAction 

```C++
bool zmbt::Environment::HasAction (
    boost::json::string_view key
) const
```




<hr>



### function HasTrigger 

```C++
bool zmbt::Environment::HasTrigger (
    boost::json::string_view key
) const
```




<hr>



### function InterfaceId 

```C++
interface_id zmbt::Environment::InterfaceId (
    boost::json::string_view interface_key
) const
```




<hr>



### function Lock 

_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock._
```C++
lock_t zmbt::Environment::Lock () const
```




<hr>



### function ObjectId 

```C++
object_id zmbt::Environment::ObjectId (
    boost::json::string_view interface_key
) const
```




<hr>



### function RegisterAction 

```C++
Environment & zmbt::Environment::RegisterAction (
    lang::Expression const & key_expr,
    std::function< void()> action
) 
```




<hr>



### function RegisterAnonymousInterface [1/2]

```C++
inline Environment & zmbt::Environment::RegisterAnonymousInterface (
    interface_id const & ifc_id,
    object_id const & obj_id
) 
```




<hr>



### function RegisterAnonymousInterface [2/2]

```C++
template<class I>
inline enable_if_t< is_ifc_handle< I >::value, Environment & > zmbt::Environment::RegisterAnonymousInterface (
    I && interface,
    object_id const & obj_id=object_id { ifc_host_nullptr < I >}
) 
```




<hr>



### function RegisterAnonymousTrigger 

```C++
template<class H, class I>
inline boost::json::string zmbt::Environment::RegisterAnonymousTrigger (
    I && interface,
    H && host
) 
```




<hr>



### function RegisterInterface [1/2]

_Register interface to enable FFI in the test model runners._ 
```C++
Environment & zmbt::Environment::RegisterInterface (
    boost::json::string_view key,
    interface_id const & ifc_id,
    object_id const & obj_id
) 
```





**Parameters:**


* `obj_id` object associated with callable 
* `ifc_id` callable handle 
* `key` string key, unique per environment 



**Returns:**







        

<hr>



### function RegisterInterface [2/2]

```C++
template<class I>
inline enable_if_t< is_ifc_handle< I >::value, Environment & > zmbt::Environment::RegisterInterface (
    boost::json::string_view key,
    I && interface,
    object_id const & obj_id=object_id { ifc_host_nullptr < I >}
) 
```




<hr>



### function RegisterParametricTriggerIfc 

```C++
template<class I>
inline interface_id zmbt::Environment::RegisterParametricTriggerIfc (
    I && interface
) 
```




<hr>



### function RegisterParametricTriggerObj 

```C++
template<class T>
inline object_id zmbt::Environment::RegisterParametricTriggerObj (
    T && obj
) 
```




<hr>



### function RegisterPrototypes 

```C++
template<class I>
inline Environment & zmbt::Environment::RegisterPrototypes (
    I && interface
) 
```




<hr>



### function RegisterTrigger [1/2]

_Register test trigger to enable FFI in the test model runners._ 
```C++
template<class I, class H>
inline Environment & zmbt::Environment::RegisterTrigger (
    boost::json::string_view key,
    I && interface,
    H && host
) 
```





**Template parameters:**


* `H` 
* `I` 



**Parameters:**


* `host` callable host object 
* `interface` callable interface handle 
* `key` string key, unique per environment 



**Returns:**







        

<hr>



### function RegisterTrigger [2/2]

```C++
template<class I>
inline Environment & zmbt::Environment::RegisterTrigger (
    boost::json::string_view key,
    I && interface
) 
```




<hr>



### function ResetAll 

_Clear all data._ 
```C++
void zmbt::Environment::ResetAll () 
```




<hr>



### function ResetAllFor 

_Clear data associated with the object._ 
```C++
void zmbt::Environment::ResetAllFor (
    object_id obj
) 
```





**Parameters:**


* `obj` 




        

<hr>



### function ResetFailureHandler 

_Reset the test handler to default._ 
```C++
Environment & zmbt::Environment::ResetFailureHandler () 
```




<hr>



### function ResetInterfaceData 

_Clear all interface data (accessed via ArgsFor/ReturnFor)_ 
```C++
void zmbt::Environment::ResetInterfaceData () 
```





**Warning:**

this will invalidate interface records 





        

<hr>



### function ResetInterfaceDataFor 

_Clear interface data associated with the object (accessed via ArgsFor/ReturnFor)_ 
```C++
void zmbt::Environment::ResetInterfaceDataFor (
    object_id obj
) 
```





**Warning:**

this will invalidate interface records 




**Parameters:**


* `obj` 




        

<hr>



### function RunAction 

```C++
Environment & zmbt::Environment::RunAction (
    lang::Expression const & key_expr
) 
```




<hr>



### function SetFailureHandler 

_Set custom test failure handler._ 
```C++
Environment & zmbt::Environment::SetFailureHandler (
    std::function< void(boost::json::value const &)> const & fn
) 
```




<hr>



### function SetPrettyPrint 

_Set pretty print JSON values._ 
```C++
Environment & zmbt::Environment::SetPrettyPrint (
    bool const pretty_print=true
) 
```




<hr>



### function SetShared 

_Set the shared data associated with key._ 
```C++
template<class T>
inline void zmbt::Environment::SetShared (
    lang::Expression const & key_expr,
    std::shared_ptr< T > data
) 
```





**Template parameters:**


* `T` 
* `A` 



**Parameters:**


* `key_expr` 
* `data` 




        

<hr>



### function SetVar [1/2]

_Set the environment variable._ 
```C++
void zmbt::Environment::SetVar (
    lang::Expression const & key_expr,
    boost::json::value var
) 
```





**Template parameters:**


* `T` 



**Parameters:**


* `key_expr` key constant expression 
* `var` 




        

<hr>



### function SetVar [2/2]

```C++
template<class T>
inline void zmbt::Environment::SetVar (
    lang::Expression const & key_expr,
    T var
) 
```




<hr>



### function TryLock 

_Get the_ [_**Environment**_](classzmbt_1_1Environment.md) _thread lock with std::try\_to\_lock tag._
```C++
lock_t zmbt::Environment::TryLock () const
```




<hr>



### function json\_data [1/2]

```C++
inline JsonNode & zmbt::Environment::json_data () 
```




<hr>



### function json\_data [2/2]

```C++
inline JsonNode const & zmbt::Environment::json_data () const
```




<hr>



### function operator= 

```C++
Environment & zmbt::Environment::operator= (
    Environment &&
) = default
```




<hr>



### function operator= 

```C++
Environment & zmbt::Environment::operator= (
    Environment const &
) = default
```




<hr>



### function ~Environment 

```C++
inline virtual zmbt::Environment::~Environment () 
```




<hr>
## Public Static Functions Documentation




### function autokey 

```C++
static inline boost::json::string zmbt::Environment::autokey (
    object_id const & obj_id,
    interface_id const & ifc_id
) 
```




<hr>
## Protected Types Documentation




### typedef FailureHandler 

```C++
using zmbt::Environment::FailureHandler =  std::function<void(boost::json::value const&)>;
```




<hr>



### typedef argsref\_t 

```C++
using zmbt::Environment::argsref_t =  reflect::invocation_args_t<ifc_pointer_t<T> >;
```




<hr>



### typedef hookout\_args\_t 

```C++
using zmbt::Environment::hookout_args_t =  mp_transform<rvref_to_val, argsref_t<I> >;
```




<hr>



### typedef lock\_t 

```C++
using zmbt::Environment::lock_t =  typename EnvironmentData::lock_t;
```




<hr>



### typedef return\_t 

```C++
using zmbt::Environment::return_t =  reflect::invocation_ret_t<ifc_pointer_t<T> >;
```




<hr>



### typedef rvref\_to\_val 

```C++
using zmbt::Environment::rvref_to_val =  mp_if<std::is_rvalue_reference<T>, std::remove_reference_t<T>, T>;
```




<hr>



### typedef unqf\_args\_t 

```C++
using zmbt::Environment::unqf_args_t =  reflect::invocation_args_unqf_t<ifc_pointer_t<T> >;
```




<hr>
## Protected Attributes Documentation




### variable config\_ 

```C++
std::shared_ptr<PersistentConfig> zmbt::Environment::config_;
```




<hr>



### variable data\_ 

```C++
std::shared_ptr<EnvironmentData> zmbt::Environment::data_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/environment.hpp`

