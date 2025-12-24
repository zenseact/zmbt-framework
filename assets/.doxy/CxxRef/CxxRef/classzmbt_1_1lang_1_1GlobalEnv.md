

# Class zmbt::lang::GlobalEnv



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md)



_Global Storage env for_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _side effects._

* `#include <global_env.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GlobalEnv**](#function-globalenv-13) () <br> |
|   | [**GlobalEnv**](#function-globalenv-23) ([**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) const &) = default<br> |
|   | [**GlobalEnv**](#function-globalenv-33) ([**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) &&) = default<br> |
|  boost::json::value | [**Load**](#function-load) (boost::json::string\_view const json\_pointer) noexcept const<br> |
|  void | [**Reset**](#function-reset) () noexcept<br> |
|  boost::json::value | [**Store**](#function-store) (boost::json::string\_view const json\_pointer, boost::json::value const & value) noexcept<br> |
|  [**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) & | [**operator=**](#function-operator) ([**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) const &) = default<br> |
|  [**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) & | [**operator=**](#function-operator_1) ([**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) &&) = default<br> |
|   | [**~GlobalEnv**](#function-globalenv) () <br> |




























## Public Functions Documentation




### function GlobalEnv [1/3]

```C++
zmbt::lang::GlobalEnv::GlobalEnv () 
```




<hr>



### function GlobalEnv [2/3]

```C++
zmbt::lang::GlobalEnv::GlobalEnv (
    GlobalEnv const &
) = default
```




<hr>



### function GlobalEnv [3/3]

```C++
zmbt::lang::GlobalEnv::GlobalEnv (
    GlobalEnv &&
) = default
```




<hr>



### function Load 

```C++
boost::json::value zmbt::lang::GlobalEnv::Load (
    boost::json::string_view const json_pointer
) noexcept const
```




<hr>



### function Reset 

```C++
void zmbt::lang::GlobalEnv::Reset () noexcept
```




<hr>



### function Store 

```C++
boost::json::value zmbt::lang::GlobalEnv::Store (
    boost::json::string_view const json_pointer,
    boost::json::value const & value
) noexcept
```




<hr>



### function operator= 

```C++
GlobalEnv & zmbt::lang::GlobalEnv::operator= (
    GlobalEnv const &
) = default
```




<hr>



### function operator= 

```C++
GlobalEnv & zmbt::lang::GlobalEnv::operator= (
    GlobalEnv &&
) = default
```




<hr>



### function ~GlobalEnv 

```C++
zmbt::lang::GlobalEnv::~GlobalEnv () 
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```C++
std::ostream & zmbt::lang::GlobalEnv::operator<< (
    std::ostream & os,
    GlobalEnv const & env
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/global_env.hpp`

