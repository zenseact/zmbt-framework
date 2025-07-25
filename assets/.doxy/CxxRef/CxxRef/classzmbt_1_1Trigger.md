

# Class zmbt::Trigger



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Trigger**](classzmbt_1_1Trigger.md)



_Controlled environment interface executor._ 

* `#include <trigger.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Trigger**](#function-trigger-25) ([**Trigger**](classzmbt_1_1Trigger.md) const &) = default<br> |
|   | [**Trigger**](#function-trigger-35) ([**Trigger**](classzmbt_1_1Trigger.md) &&) = default<br> |
|   | [**Trigger**](#function-trigger-45) (T && obj, I && interface) <br> |
|   | [**Trigger**](#function-trigger-55) ([**type\_tag**](structzmbt_1_1type__tag.md)&lt; T &gt;, I && interface, A &&... args) <br> |
|  boost::json::value | [**execute**](#function-execute) (boost::json::value const & args\_in=boost::json::array{}) const<br> |
|  [**interface\_id**](classzmbt_1_1interface__id.md) | [**ifc\_id**](#function-ifc_id) () const<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**obj\_id**](#function-obj_id) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**Trigger**](classzmbt_1_1Trigger.md) const & o) const<br> |
|  boost::json::value | [**operator()**](#function-operator()) (boost::json::value const & args\_in=boost::json::array{}) const<br> |
|  [**Trigger**](classzmbt_1_1Trigger.md) & | [**operator=**](#function-operator_1) ([**Trigger**](classzmbt_1_1Trigger.md) const &) = default<br> |
|  [**Trigger**](classzmbt_1_1Trigger.md) & | [**operator=**](#function-operator_2) ([**Trigger**](classzmbt_1_1Trigger.md) &&) = default<br> |
|  bool | [**operator==**](#function-operator_3) ([**Trigger**](classzmbt_1_1Trigger.md) const & o) const<br> |
|   | [**~Trigger**](#function-trigger) () = default<br> |




























## Public Functions Documentation




### function Trigger [2/5]

```C++
zmbt::Trigger::Trigger (
    Trigger const &
) = default
```




<hr>



### function Trigger [3/5]

```C++
zmbt::Trigger::Trigger (
    Trigger &&
) = default
```




<hr>



### function Trigger [4/5]

```C++
template<class T, class I>
inline zmbt::Trigger::Trigger (
    T && obj,
    I && interface
) 
```




<hr>



### function Trigger [5/5]

```C++
template<class T, class I, class... A>
inline zmbt::Trigger::Trigger (
    type_tag < T >,
    I && interface,
    A &&... args
) 
```




<hr>



### function execute 

```C++
inline boost::json::value zmbt::Trigger::execute (
    boost::json::value const & args_in=boost::json::array{}
) const
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
inline boost::json::value zmbt::Trigger::operator() (
    boost::json::value const & args_in=boost::json::array{}
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

