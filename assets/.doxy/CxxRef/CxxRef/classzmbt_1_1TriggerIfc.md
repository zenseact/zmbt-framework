

# Class zmbt::TriggerIfc



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**TriggerIfc**](classzmbt_1_1TriggerIfc.md)



[More...](#detailed-description)

* `#include <trigger.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TriggerIfc**](#function-triggerifc-14) (Interface && interface) <br> |
|   | [**TriggerIfc**](#function-triggerifc-24) ([**TriggerIfc**](classzmbt_1_1TriggerIfc.md) const &) = default<br> |
|   | [**TriggerIfc**](#function-triggerifc-34) ([**TriggerIfc**](classzmbt_1_1TriggerIfc.md) &) = default<br> |
|   | [**TriggerIfc**](#function-triggerifc-44) ([**TriggerIfc**](classzmbt_1_1TriggerIfc.md) &&) = default<br> |
|  void | [**execute**](#function-execute) (std::shared\_ptr&lt; void &gt; obj, boost::json::value const & args\_in, [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) & recorder) const<br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**id**](#function-id) () const<br> |
|  [**TriggerIfc**](classzmbt_1_1TriggerIfc.md) & | [**operator=**](#function-operator) ([**TriggerIfc**](classzmbt_1_1TriggerIfc.md) const &) = default<br> |
|  [**TriggerIfc**](classzmbt_1_1TriggerIfc.md) & | [**operator=**](#function-operator_1) ([**TriggerIfc**](classzmbt_1_1TriggerIfc.md) &&) = default<br> |
| virtual  | [**~TriggerIfc**](#function-triggerifc) () = default<br> |




























## Detailed Description


Function wrapper. Transforms `(T...) -> R` or `(O*)(T...) -> R` to `(shared_ptr<void>, JSON) -> void`, capturing R with [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) 


    
## Public Functions Documentation




### function TriggerIfc [1/4]

```C++
template<class Interface>
inline zmbt::TriggerIfc::TriggerIfc (
    Interface && interface
) 
```




<hr>



### function TriggerIfc [2/4]

```C++
zmbt::TriggerIfc::TriggerIfc (
    TriggerIfc const &
) = default
```




<hr>



### function TriggerIfc [3/4]

```C++
zmbt::TriggerIfc::TriggerIfc (
    TriggerIfc &
) = default
```




<hr>



### function TriggerIfc [4/4]

```C++
zmbt::TriggerIfc::TriggerIfc (
    TriggerIfc &&
) = default
```




<hr>



### function execute 

```C++
inline void zmbt::TriggerIfc::execute (
    std::shared_ptr< void > obj,
    boost::json::value const & args_in,
    OutputRecorder & recorder
) const
```




<hr>



### function id 

```C++
inline interface_id zmbt::TriggerIfc::id () const
```




<hr>



### function operator= 

```C++
TriggerIfc & zmbt::TriggerIfc::operator= (
    TriggerIfc const &
) = default
```




<hr>



### function operator= 

```C++
TriggerIfc & zmbt::TriggerIfc::operator= (
    TriggerIfc &&
) = default
```




<hr>



### function ~TriggerIfc 

```C++
virtual zmbt::TriggerIfc::~TriggerIfc () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/trigger.hpp`

