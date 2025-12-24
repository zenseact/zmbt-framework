

# File preprocessor.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**preprocessor.hpp**](preprocessor_8hpp.md)

[Go to the source code of this file](preprocessor_8hpp_source.md)

_type traits to check serialization compatibility_ [More...](#detailed-description)

* `#include <boost/preprocessor.hpp>`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_CUR\_LOC**](preprocessor_8hpp.md#define-zmbt_cur_loc)  `\_\_FILE\_\_ "#" ZMBT\_STRINGIFY(\_\_LINE\_\_)`<br> |
| define  | [**ZMBT\_PP\_INJECT\_CODE\_INTO\_NS**](preprocessor_8hpp.md#define-zmbt_pp_inject_code_into_ns) (code, ...) `/* multi line expression */`<br> |
| define  | [**ZMBT\_PP\_NS\_CLOSE**](preprocessor_8hpp.md#define-zmbt_pp_ns_close) (s, state, ns) `}`<br> |
| define  | [**ZMBT\_PP\_NS\_CLOSE\_ALL**](preprocessor_8hpp.md#define-zmbt_pp_ns_close_all) (...) `BOOST\_PP\_SEQ\_FOR\_EACH(ZMBT\_PP\_NS\_CLOSE, , BOOST\_PP\_VARIADIC\_TO\_SEQ(\_\_VA\_ARGS\_\_))`<br> |
| define  | [**ZMBT\_PP\_NS\_OPEN**](preprocessor_8hpp.md#define-zmbt_pp_ns_open) (s, state, ns) `namespace ns {`<br> |
| define  | [**ZMBT\_PP\_NS\_OPEN\_ALL**](preprocessor_8hpp.md#define-zmbt_pp_ns_open_all) (...) `BOOST\_PP\_SEQ\_FOR\_EACH(ZMBT\_PP\_NS\_OPEN, , BOOST\_PP\_VARIADIC\_TO\_SEQ(\_\_VA\_ARGS\_\_))`<br> |
| define  | [**ZMBT\_STRINGIFY**](preprocessor_8hpp.md#define-zmbt_stringify) (x) `ZMBT\_STRINGIFY\_IMPL(x)`<br> |
| define  | [**ZMBT\_STRINGIFY\_IMPL**](preprocessor_8hpp.md#define-zmbt_stringify_impl) (x) `#x`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2022-2023 Volvo Car Corporation 




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_CUR\_LOC 

```C++
#define ZMBT_CUR_LOC `__FILE__ "#" ZMBT_STRINGIFY(__LINE__)`
```




<hr>



### define ZMBT\_PP\_INJECT\_CODE\_INTO\_NS 

```C++
#define ZMBT_PP_INJECT_CODE_INTO_NS (
    code,
    ...
) `/* multi line expression */`
```




<hr>



### define ZMBT\_PP\_NS\_CLOSE 

```C++
#define ZMBT_PP_NS_CLOSE (
    s,
    state,
    ns
) `}`
```




<hr>



### define ZMBT\_PP\_NS\_CLOSE\_ALL 

```C++
#define ZMBT_PP_NS_CLOSE_ALL (
    ...
) `BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_CLOSE, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))`
```




<hr>



### define ZMBT\_PP\_NS\_OPEN 

```C++
#define ZMBT_PP_NS_OPEN (
    s,
    state,
    ns
) `namespace ns {`
```




<hr>



### define ZMBT\_PP\_NS\_OPEN\_ALL 

```C++
#define ZMBT_PP_NS_OPEN_ALL (
    ...
) `BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_OPEN, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))`
```




<hr>



### define ZMBT\_STRINGIFY 

```C++
#define ZMBT_STRINGIFY (
    x
) `ZMBT_STRINGIFY_IMPL(x)`
```




<hr>



### define ZMBT\_STRINGIFY\_IMPL 

```C++
#define ZMBT_STRINGIFY_IMPL (
    x
) `#x`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/preprocessor.hpp`

