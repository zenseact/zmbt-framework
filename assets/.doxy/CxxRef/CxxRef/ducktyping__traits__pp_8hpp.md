

# File ducktyping\_traits\_pp.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**ducktyping\_traits\_pp.hpp**](ducktyping__traits__pp_8hpp.md)

[Go to the source code of this file](ducktyping__traits__pp_8hpp_source.md)

[More...](#detailed-description)

































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**CAT**](ducktyping__traits__pp_8hpp.md#define-cat) (a, b) `a##b`<br> |
| define  | [**ZMBT\_HAS\_MEMBER**](ducktyping__traits__pp_8hpp.md#define-zmbt_has_member) (name) `ZMBT\_TRAIT\_HAS\_MEMBER(CAT(has\_member\_, name), name)`<br> |
| define  | [**ZMBT\_HAS\_TYPE**](ducktyping__traits__pp_8hpp.md#define-zmbt_has_type) (name) `ZMBT\_TRAIT\_HAS\_TYPE(CAT(has\_type\_, name), name)`<br> |
| define  | [**ZMBT\_TRAIT\_HAS\_MEMBER**](ducktyping__traits__pp_8hpp.md#define-zmbt_trait_has_member) (trait, name) `/* multi line expression */`<br> |
| define  | [**ZMBT\_TRAIT\_HAS\_TYPE**](ducktyping__traits__pp_8hpp.md#define-zmbt_trait_has_type) (trait, name) `/* multi line expression */`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2022-2023 Volvo Car Corporation 




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define CAT 

```C++
#define CAT (
    a,
    b
) `a##b`
```




<hr>



### define ZMBT\_HAS\_MEMBER 

```C++
#define ZMBT_HAS_MEMBER (
    name
) `ZMBT_TRAIT_HAS_MEMBER(CAT(has_member_, name), name)`
```




<hr>



### define ZMBT\_HAS\_TYPE 

```C++
#define ZMBT_HAS_TYPE (
    name
) `ZMBT_TRAIT_HAS_TYPE(CAT(has_type_, name), name)`
```




<hr>



### define ZMBT\_TRAIT\_HAS\_MEMBER 

```C++
#define ZMBT_TRAIT_HAS_MEMBER (
    trait,
    name
) `/* multi line expression */`
```




<hr>



### define ZMBT\_TRAIT\_HAS\_TYPE 

```C++
#define ZMBT_TRAIT_HAS_TYPE (
    trait,
    name
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/ducktyping_traits_pp.hpp`

