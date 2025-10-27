

# Class zmbt::Trigger



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Trigger**](classzmbt_1_1Trigger.md)



_Controlled environment interface executor._ 

* `#include <trigger.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Trigger**](#function-trigger-24) ([**Trigger**](classzmbt_1_1Trigger.md) const &) = default<br> |
|   | [**Trigger**](#function-trigger-34) ([**Trigger**](classzmbt_1_1Trigger.md) &&) = default<br> |
|   | [**Trigger**](#function-trigger-44) (T && obj, Interface && interface, std::shared\_ptr&lt; [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) &gt; recorder) <br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**ifc\_id**](#function-ifc_id) () const<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**obj\_id**](#function-obj_id) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**Trigger**](classzmbt_1_1Trigger.md) const & o) const<br> |
|  void | [**operator()**](#function-operator_1) (boost::json::value args\_in) const<br> |
|  [**Trigger**](classzmbt_1_1Trigger.md) & | [**operator=**](#function-operator_2) ([**Trigger**](classzmbt_1_1Trigger.md) const &) = default<br> |
|  [**Trigger**](classzmbt_1_1Trigger.md) & | [**operator=**](#function-operator_3) ([**Trigger**](classzmbt_1_1Trigger.md) &&) = default<br> |
|  bool | [**operator==**](#function-operator_4) ([**Trigger**](classzmbt_1_1Trigger.md) const & o) const<br> |
|   | [**~Trigger**](#function-trigger) () = default<br> |




























## Public Functions Documentation




### function Trigger [2/4]

```C++
zmbt::Trigger::Trigger (
    Trigger const &
) = default
```




<hr>



### function Trigger [3/4]

```C++
zmbt::Trigger::Trigger (
    Trigger &&
) = default
```




<hr>



### function Trigger [4/4]

```C++
template<class T, class Interface>
inline zmbt::Trigger::Trigger (
    T && obj,
    Interface && interface,
    std::shared_ptr< OutputRecorder > recorder
) 
```




<hr>



### function ifc\_id 

```C++
inline interface_id zmbt::Trigger::ifc_id () const
```




<hr>



### function obj\_id 

```C++
inline object_id zmbt::Trigger::obj_id () const
```




<hr>



### function operator!= 

```C++
inline bool zmbt::Trigger::operator!= (
    Trigger const & o
) const
```




<hr>



### function operator() 

```C++
inline void zmbt::Trigger::operator() (
    boost::json::value args_in
) const
```




<hr>



### function operator= 

```C++
Trigger & zmbt::Trigger::operator= (
    Trigger const &
) = default
```




<hr>



### function operator= 

```C++
Trigger & zmbt::Trigger::operator= (
    Trigger &&
) = default
```




<hr>



### function operator== 

```C++
inline bool zmbt::Trigger::operator== (
    Trigger const & o
) const
```




<hr>



### function ~Trigger 

```C++
zmbt::Trigger::~Trigger () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/trigger.hpp`

