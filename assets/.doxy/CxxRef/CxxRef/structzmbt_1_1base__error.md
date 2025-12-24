

# Struct zmbt::base\_error



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**base\_error**](structzmbt_1_1base__error.md)



_ZMBT Base exception._ 

* `#include <exceptions.hpp>`



Inherits the following classes: std::runtime_error


Inherited by the following classes: [zmbt::environment\_error](structzmbt_1_1environment__error.md),  [zmbt::lang::Operator::operator\_error](structzmbt_1_1lang_1_1Operator_1_1operator__error.md),  [zmbt::model\_error](structzmbt_1_1model__error.md),  [zmbt::output\_recorder\_error](structzmbt_1_1output__recorder__error.md),  [zmbt::serialization\_error](structzmbt_1_1serialization__error.md),  [zmbt::test\_assertion\_failure](structzmbt_1_1test__assertion__failure.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**base\_error**](#function-base_error) (boost::json::string\_view fmtstr, A &&... args) <br> |




























## Public Functions Documentation




### function base\_error 

```C++
template<class... A>
inline zmbt::base_error::base_error (
    boost::json::string_view fmtstr,
    A &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/exceptions.hpp`

