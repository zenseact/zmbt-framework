

# File eval\_impl\_pp.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_impl\_pp.hpp**](eval__impl__pp_8hpp.md)

[Go to the source code of this file](eval__impl__pp_8hpp_source.md)

[More...](#detailed-description)

































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ASSERT**](eval__impl__pp_8hpp.md#define-assert) (cond, msg) `if (!(cond)) { return ::zmbt::lang::detail::make\_error\_expr(msg, keyword\_to\_str());}`<br> |
| define  | [**ZMBT\_DEFINE\_EVALUATE\_IMPL**](eval__impl__pp_8hpp.md#define-zmbt_define_evaluate_impl) (K) `/* multi line expression */`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ASSERT 

```C++
#define ASSERT (
    cond,
    msg
) `if (!(cond)) { return ::zmbt::lang::detail::make_error_expr(msg, keyword_to_str());}`
```




<hr>



### define ZMBT\_DEFINE\_EVALUATE\_IMPL 

```C++
#define ZMBT_DEFINE_EVALUATE_IMPL (
    K
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/eval_impl_pp.hpp`

