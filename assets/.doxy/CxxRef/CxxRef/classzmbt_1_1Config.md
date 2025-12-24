

# Class zmbt::Config



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Config**](classzmbt_1_1Config.md)



_Global app config._ 

* `#include <config.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void(boost::json::value const  &)&gt; | [**FailureHandler**](#typedef-failurehandler)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Config**](#function-config-13) () <br> |
|   | [**Config**](#function-config-23) ([**Config**](classzmbt_1_1Config.md) const &) = default<br> |
|   | [**Config**](#function-config-33) ([**Config**](classzmbt_1_1Config.md) &&) = default<br> |
|  [**Config**](classzmbt_1_1Config.md) & | [**HandleTestFailure**](#function-handletestfailure) (boost::json::value const & diagnostics) <br> |
|  [**Config**](classzmbt_1_1Config.md) & | [**ResetFailureHandler**](#function-resetfailurehandler) () <br>_Reset the test handler to default._  |
|  [**Config**](classzmbt_1_1Config.md) & | [**ResetRng**](#function-resetrng) () <br>_Reset random number generator to initial seed/state._  |
|  std::mt19937 & | [**Rng**](#function-rng) () <br>_Thread-local random number generator instance._  |
|  std::uint64\_t | [**RngSeed**](#function-rngseed) () const<br> |
|  [**Config**](classzmbt_1_1Config.md) & | [**SetFailureHandler**](#function-setfailurehandler) (FailureHandler const & fn) <br>_Set custom test failure handler._  |
|  [**Config**](classzmbt_1_1Config.md) & | [**SetRngSeed**](#function-setrngseed) (std::uint64\_t const seed) <br>_Set random number generator seed._  |
|  [**Config**](classzmbt_1_1Config.md) & | [**operator=**](#function-operator) ([**Config**](classzmbt_1_1Config.md) const &) = default<br> |
|  [**Config**](classzmbt_1_1Config.md) & | [**operator=**](#function-operator_1) ([**Config**](classzmbt_1_1Config.md) &&) = default<br> |
|   | [**~Config**](#function-config) () = default<br> |




























## Public Types Documentation




### typedef FailureHandler 

```C++
using zmbt::Config::FailureHandler =  std::function<void(boost::json::value const&)>;
```




<hr>
## Public Functions Documentation




### function Config [1/3]

```C++
zmbt::Config::Config () 
```




<hr>



### function Config [2/3]

```C++
zmbt::Config::Config (
    Config const &
) = default
```




<hr>



### function Config [3/3]

```C++
zmbt::Config::Config (
    Config &&
) = default
```




<hr>



### function HandleTestFailure 

```C++
Config & zmbt::Config::HandleTestFailure (
    boost::json::value const & diagnostics
) 
```




<hr>



### function ResetFailureHandler 

_Reset the test handler to default._ 
```C++
Config & zmbt::Config::ResetFailureHandler () 
```




<hr>



### function ResetRng 

_Reset random number generator to initial seed/state._ 
```C++
Config & zmbt::Config::ResetRng () 
```




<hr>



### function Rng 

_Thread-local random number generator instance._ 
```C++
std::mt19937 & zmbt::Config::Rng () 
```




<hr>



### function RngSeed 

```C++
std::uint64_t zmbt::Config::RngSeed () const
```




<hr>



### function SetFailureHandler 

_Set custom test failure handler._ 
```C++
Config & zmbt::Config::SetFailureHandler (
    FailureHandler const & fn
) 
```




<hr>



### function SetRngSeed 

_Set random number generator seed._ 
```C++
Config & zmbt::Config::SetRngSeed (
    std::uint64_t const seed
) 
```




<hr>



### function operator= 

```C++
Config & zmbt::Config::operator= (
    Config const &
) = default
```




<hr>



### function operator= 

```C++
Config & zmbt::Config::operator= (
    Config &&
) = default
```




<hr>



### function ~Config 

```C++
zmbt::Config::~Config () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/application/config.hpp`

