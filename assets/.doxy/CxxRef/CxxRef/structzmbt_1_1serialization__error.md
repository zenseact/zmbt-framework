

# Struct zmbt::serialization\_error



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**serialization\_error**](structzmbt_1_1serialization__error.md)



_Failed serialization._ 

* `#include <exceptions.hpp>`



Inherits the following classes: [zmbt::base\_error](structzmbt_1_1base__error.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**base\_error**](#function-base_error) (boost::json::string\_view fmtstr, A &&... args) <br> |


## Public Functions inherited from zmbt::base_error

See [zmbt::base\_error](structzmbt_1_1base__error.md)

| Type | Name |
| ---: | :--- |
|   | [**base\_error**](structzmbt_1_1base__error.md#function-base_error) (boost::json::string\_view fmtstr, A &&... args) <br> |






















































## Public Functions Documentation




### function base\_error 

```C++
template<class... A>
inline zmbt::serialization_error::base_error (
    boost::json::string_view fmtstr,
    A &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/exceptions.hpp`

