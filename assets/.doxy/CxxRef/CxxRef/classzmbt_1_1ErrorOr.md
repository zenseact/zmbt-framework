

# Class zmbt::ErrorOr

**template &lt;class Return&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**ErrorOr**](classzmbt_1_1ErrorOr.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ErrorOr**](#function-erroror) () = default<br> |
|  [**ErrorInfo**](structzmbt_1_1ErrorInfo.md) const & | [**as\_error**](#function-as_error) () const<br> |
|  Return const & | [**as\_return**](#function-as_return) () const<br> |
|  void | [**dump\_to**](#function-dump_to) (boost::json::object & obj) const<br> |
|  bool | [**is\_error**](#function-is_error) () const<br> |
|  bool | [**is\_null**](#function-is_null) () const<br> |
|  bool | [**is\_return**](#function-is_return) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**ErrorOr**](classzmbt_1_1ErrorOr.md)&lt; Return &gt; | [**MakeError**](#function-makeerror) ([**ErrorInfo**](structzmbt_1_1ErrorInfo.md) err) <br> |
|  [**ErrorOr**](classzmbt_1_1ErrorOr.md)&lt; Return &gt; | [**MakeValue**](#function-makevalue) (Return ret) <br> |


























## Public Functions Documentation




### function ErrorOr 

```C++
zmbt::ErrorOr::ErrorOr () = default
```




<hr>



### function as\_error 

```C++
inline ErrorInfo const & zmbt::ErrorOr::as_error () const
```




<hr>



### function as\_return 

```C++
inline Return const & zmbt::ErrorOr::as_return () const
```




<hr>



### function dump\_to 

```C++
inline void zmbt::ErrorOr::dump_to (
    boost::json::object & obj
) const
```




<hr>



### function is\_error 

```C++
inline bool zmbt::ErrorOr::is_error () const
```




<hr>



### function is\_null 

```C++
inline bool zmbt::ErrorOr::is_null () const
```




<hr>



### function is\_return 

```C++
inline bool zmbt::ErrorOr::is_return () const
```




<hr>
## Public Static Functions Documentation




### function MakeError 

```C++
static inline ErrorOr < Return > zmbt::ErrorOr::MakeError (
    ErrorInfo err
) 
```




<hr>



### function MakeValue 

```C++
static inline ErrorOr < Return > zmbt::ErrorOr::MakeValue (
    Return ret
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/error_or_return.hpp`

