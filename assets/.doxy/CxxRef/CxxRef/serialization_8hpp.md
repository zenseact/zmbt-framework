

# File serialization.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**serialization.hpp**](serialization_8hpp.md)

[Go to the source code of this file](serialization_8hpp_source.md)

[More...](#detailed-description)

* `#include <type_traits>`
* `#include <boost/json.hpp>`
* `#include "zmbt/core/preprocessor.hpp"`
* `#include "zmbt/core.hpp"`
* `#include "ducktyping_traits_pp.hpp"`
* `#include "signal_traits.hpp"`
* `#include "serialization_defaults.hpp"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |
| namespace | [**reflect**](namespacezmbt_1_1reflect.md) <br>_Reflection metafunctions._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**has\_serialization**](structzmbt_1_1has__serialization.md) &lt;class T, class E&gt;<br> |
| struct | [**serialization&lt; T, detail::enable\_for\_custom\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__custom__serialization_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**serialization&lt; T, detail::enable\_for\_default\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__default__serialization_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_INJECT\_JSON\_TAG\_INVOKE**](serialization_8hpp.md#define-zmbt_inject_json_tag_invoke)  `using [**zmbt::reflect::tag\_invoke**](namespacezmbt_1_1reflect.md#function-tag_invoke);`<br> |
| define  | [**ZMBT\_INJECT\_JSON\_TAG\_INVOKE\_INTO**](serialization_8hpp.md#define-zmbt_inject_json_tag_invoke_into) (...) `ZMBT\_PP\_INJECT\_CODE\_INTO\_NS(ZMBT\_INJECT\_JSON\_TAG\_INVOKE, \_\_VA\_ARGS\_\_)`<br> |
| define  | [**ZMBT\_INJECT\_OSTREAM\_OPERATOR**](serialization_8hpp.md#define-zmbt_inject_ostream_operator)  `using zmbt::reflect::operator&lt;&lt;;`<br> |
| define  | [**ZMBT\_INJECT\_OSTREAM\_OPERATOR\_INTO**](serialization_8hpp.md#define-zmbt_inject_ostream_operator_into) (...) `ZMBT\_PP\_INJECT\_CODE\_INTO\_NS(ZMBT\_INJECT\_OSTREAM\_OPERATOR, \_\_VA\_ARGS\_\_)`<br> |
| define  | [**ZMBT\_INJECT\_SERIALIZATION**](serialization_8hpp.md#define-zmbt_inject_serialization)  `ZMBT\_INJECT\_JSON\_TAG\_INVOKE ZMBT\_INJECT\_OSTREAM\_OPERATOR`<br> |
| define  | [**ZMBT\_INJECT\_SERIALIZATION\_INTO**](serialization_8hpp.md#define-zmbt_inject_serialization_into) (...) `ZMBT\_PP\_INJECT\_CODE\_INTO\_NS(ZMBT\_INJECT\_SERIALIZATION, \_\_VA\_ARGS\_\_)`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2022-2023 Volvo Car Corporation 




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_INJECT\_JSON\_TAG\_INVOKE 

```C++
#define ZMBT_INJECT_JSON_TAG_INVOKE `using zmbt::reflect::tag_invoke ;`
```




<hr>



### define ZMBT\_INJECT\_JSON\_TAG\_INVOKE\_INTO 

```C++
#define ZMBT_INJECT_JSON_TAG_INVOKE_INTO (
    ...
) `ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_JSON_TAG_INVOKE, __VA_ARGS__)`
```




<hr>



### define ZMBT\_INJECT\_OSTREAM\_OPERATOR 

```C++
#define ZMBT_INJECT_OSTREAM_OPERATOR `using zmbt::reflect::operator<<;`
```




<hr>



### define ZMBT\_INJECT\_OSTREAM\_OPERATOR\_INTO 

```C++
#define ZMBT_INJECT_OSTREAM_OPERATOR_INTO (
    ...
) `ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_OSTREAM_OPERATOR, __VA_ARGS__)`
```




<hr>



### define ZMBT\_INJECT\_SERIALIZATION 

```C++
#define ZMBT_INJECT_SERIALIZATION `ZMBT_INJECT_JSON_TAG_INVOKE ZMBT_INJECT_OSTREAM_OPERATOR`
```




<hr>



### define ZMBT\_INJECT\_SERIALIZATION\_INTO 

```C++
#define ZMBT_INJECT_SERIALIZATION_INTO (
    ...
) `ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_SERIALIZATION, __VA_ARGS__)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/serialization.hpp`

