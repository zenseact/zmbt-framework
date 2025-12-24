

# File operator.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**operator.hpp**](operator_8hpp.md)

[Go to the source code of this file](operator_8hpp_source.md)

[More...](#detailed-description)

* `#include <limits>`
* `#include "zmbt/core.hpp"`
* `#include "zmbt/reflect.hpp"`
* `#include "keyword.hpp"`
* `#include "lazy_param.hpp"`
* `#include <boost/current_function.hpp>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |
| namespace | [**lang**](namespacezmbt_1_1lang.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _Language implementation._ |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Operator**](classzmbt_1_1lang_1_1Operator.md) <br>_Signal transformation and comparison handler. Enables type erasure._  |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1A.md) <br> |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1B.md) <br> |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1C.md) <br> |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1D.md) <br> |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1L.md) <br> |
| struct | [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1S.md) <br> |
| struct | [**operator\_error**](structzmbt_1_1lang_1_1Operator_1_1operator__error.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_SOH\_HANDLE\_BIN\_TRANSFORM**](operator_8hpp.md#define-zmbt_soh_handle_bin_transform) (OP, TRAIT) `/* multi line expression */`<br> |
| define  | [**ZMBT\_SOH\_HANDLE\_UNARY\_TRANSFORM**](operator_8hpp.md#define-zmbt_soh_handle_unary_transform) (OP, TRAIT) `/* multi line expression */`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_SOH\_HANDLE\_BIN\_TRANSFORM 

```C++
#define ZMBT_SOH_HANDLE_BIN_TRANSFORM (
    OP,
    TRAIT
) `/* multi line expression */`
```




<hr>



### define ZMBT\_SOH\_HANDLE\_UNARY\_TRANSFORM 

```C++
#define ZMBT_SOH_HANDLE_UNARY_TRANSFORM (
    OP,
    TRAIT
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/operator.hpp`

