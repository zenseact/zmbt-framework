

# File signal\_traits.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**signal\_traits.hpp**](signal__traits_8hpp.md)

[Go to the source code of this file](signal__traits_8hpp_source.md)

[More...](#detailed-description)

* `#include <functional>`
* `#include <type_traits>`
* `#include "zmbt/core.hpp"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |
| namespace | [**reflect**](namespacezmbt_1_1reflect.md) <br>_Reflection metafunctions._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**signal\_traits&lt; T, detail::enable\_custom\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__custom__signal__traits_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**signal\_traits&lt; T, detail::enable\_default\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__default__signal__traits_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**signal\_traits&lt; T, detail::enable\_initialization\_trap&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__initialization__trap_3_01T_01_4_01_4.md) &lt;class T&gt;<br>_Unhandled types catcher._  |
| struct | [**signal\_traits&lt; void &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01void_01_4.md) &lt;&gt;<br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_DEFINE\_CUSTOM\_INIT**](signal__traits_8hpp.md#define-zmbt_define_custom_init) (Type, Args) `/* multi line expression */`<br> |
| define  | [**ZMBT\_PP\_UNPACK**](signal__traits_8hpp.md#define-zmbt_pp_unpack) (...) `\_\_VA\_ARGS\_\_`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2022-2023 Volvo Car Corporation 




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_DEFINE\_CUSTOM\_INIT 

```C++
#define ZMBT_DEFINE_CUSTOM_INIT (
    Type,
    Args
) `/* multi line expression */`
```




<hr>



### define ZMBT\_PP\_UNPACK 

```C++
#define ZMBT_PP_UNPACK (
    ...
) `__VA_ARGS__`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/signal_traits.hpp`

