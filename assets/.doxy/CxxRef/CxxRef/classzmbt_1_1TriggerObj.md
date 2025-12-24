

# Class zmbt::TriggerObj



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**TriggerObj**](classzmbt_1_1TriggerObj.md)



_Object handler to be used with_ [_**TriggerIfc**_](classzmbt_1_1TriggerIfc.md) _._

* `#include <trigger.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TriggerObj**](#function-triggerobj-18) (std::shared\_ptr&lt; T &gt; obj) <br> |
|   | [**TriggerObj**](#function-triggerobj-28) (T \* obj) <br> |
|   | [**TriggerObj**](#function-triggerobj-38) (T const \* obj) <br> |
|   | [**TriggerObj**](#function-triggerobj-48) (T & obj) <br> |
|   | [**TriggerObj**](#function-triggerobj-58) (T const & obj) <br> |
|   | [**TriggerObj**](#function-triggerobj-68) (nullptr\_t) <br> |
|   | [**TriggerObj**](#function-triggerobj-78) ([**TriggerObj**](classzmbt_1_1TriggerObj.md) const &) = default<br> |
|   | [**TriggerObj**](#function-triggerobj-88) ([**TriggerObj**](classzmbt_1_1TriggerObj.md) &&) = default<br> |
|  [**object\_id**](classzmbt_1_1object__id.md) | [**id**](#function-id) () const<br> |
|  [**TriggerObj**](classzmbt_1_1TriggerObj.md) & | [**operator=**](#function-operator) ([**TriggerObj**](classzmbt_1_1TriggerObj.md) const &) = default<br> |
|  [**TriggerObj**](classzmbt_1_1TriggerObj.md) & | [**operator=**](#function-operator_1) ([**TriggerObj**](classzmbt_1_1TriggerObj.md) &&) = default<br> |
|  std::shared\_ptr&lt; void &gt; | [**ptr**](#function-ptr) () const<br> |
|  bool | [**unsafe**](#function-unsafe) () const<br> |
| virtual  | [**~TriggerObj**](#function-triggerobj) () = default<br> |




























## Public Functions Documentation




### function TriggerObj [1/8]

```C++
template<class T>
inline zmbt::TriggerObj::TriggerObj (
    std::shared_ptr< T > obj
) 
```




<hr>



### function TriggerObj [2/8]

```C++
template<class T>
inline zmbt::TriggerObj::TriggerObj (
    T * obj
) 
```




<hr>



### function TriggerObj [3/8]

```C++
template<class T>
inline zmbt::TriggerObj::TriggerObj (
    T const * obj
) 
```




<hr>



### function TriggerObj [4/8]

```C++
template<class T>
inline zmbt::TriggerObj::TriggerObj (
    T & obj
) 
```




<hr>



### function TriggerObj [5/8]

```C++
template<class T>
inline zmbt::TriggerObj::TriggerObj (
    T const & obj
) 
```




<hr>



### function TriggerObj [6/8]

```C++
inline zmbt::TriggerObj::TriggerObj (
    nullptr_t
) 
```




<hr>



### function TriggerObj [7/8]

```C++
zmbt::TriggerObj::TriggerObj (
    TriggerObj const &
) = default
```




<hr>



### function TriggerObj [8/8]

```C++
zmbt::TriggerObj::TriggerObj (
    TriggerObj &&
) = default
```




<hr>



### function id 

```C++
inline object_id zmbt::TriggerObj::id () const
```




<hr>



### function operator= 

```C++
TriggerObj & zmbt::TriggerObj::operator= (
    TriggerObj const &
) = default
```




<hr>



### function operator= 

```C++
TriggerObj & zmbt::TriggerObj::operator= (
    TriggerObj &&
) = default
```




<hr>



### function ptr 

```C++
inline std::shared_ptr< void > zmbt::TriggerObj::ptr () const
```




<hr>



### function unsafe 

```C++
inline bool zmbt::TriggerObj::unsafe () const
```




<hr>



### function ~TriggerObj 

```C++
virtual zmbt::TriggerObj::~TriggerObj () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/trigger.hpp`

