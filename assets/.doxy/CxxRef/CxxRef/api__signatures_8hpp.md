

# File api\_signatures.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**api\_signatures.hpp**](api__signatures_8hpp.md)

[Go to the source code of this file](api__signatures_8hpp_source.md)

[More...](#detailed-description)

* `#include <cstddef>`
* `#include <initializer_list>`
* `#include <boost/json.hpp>`
* `#include "zmbt/reflect/serialization.hpp"`
* `#include "keyword.hpp"`
* `#include "expression.hpp"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |
| namespace | [**lang**](namespacezmbt_1_1lang.md) <br>[_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _Language implementation._ |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md) &lt;K&gt;<br> |
| struct | [**SignatureBinary**](structzmbt_1_1lang_1_1SignatureBinary.md) &lt;K&gt;<br>_Binary expression._  |
| struct | [**SignatureConst**](structzmbt_1_1lang_1_1SignatureConst.md) &lt;K&gt;<br>_Const expression._  |
| struct | [**SignatureDebug**](structzmbt_1_1lang_1_1SignatureDebug.md) <br> |
| struct | [**SignatureDecorate**](structzmbt_1_1lang_1_1SignatureDecorate.md) <br> |
| struct | [**SignatureError**](structzmbt_1_1lang_1_1SignatureError.md) <br> |
| struct | [**SignatureOverload**](structzmbt_1_1lang_1_1SignatureOverload.md) <br> |
| struct | [**SignatureTernary**](structzmbt_1_1lang_1_1SignatureTernary.md) &lt;K&gt;<br>_Ternary expression._  |
| struct | [**SignatureUnary**](structzmbt_1_1lang_1_1SignatureUnary.md) &lt;K&gt;<br>_Unary expression._  |
| struct | [**SignatureUndecorate**](structzmbt_1_1lang_1_1SignatureUndecorate.md) <br> |
| struct | [**SignatureVariadic**](structzmbt_1_1lang_1_1SignatureVariadic.md) &lt;K&gt;<br>_Variadic expression._  |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_DEBUG\_EXPR**](api__signatures_8hpp.md#define-zmbt_debug_expr) (f) `[**::zmbt::expr::Debug**](namespacezmbt_1_1expr.md#variable-debug)(f, ZMBT\_CUR\_LOC)`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_DEBUG\_EXPR 

```C++
#define ZMBT_DEBUG_EXPR (
    f
) `::zmbt::expr::Debug (f, ZMBT_CUR_LOC)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/api_signatures.hpp`

