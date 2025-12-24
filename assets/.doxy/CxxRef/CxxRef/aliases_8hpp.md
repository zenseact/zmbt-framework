

# File aliases.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**aliases.hpp**](aliases_8hpp.md)

[Go to the source code of this file](aliases_8hpp_source.md)

[More...](#detailed-description)

* `#include <type_traits>`
* `#include <tuple>`
* `#include <boost/callable_traits.hpp>`
* `#include <boost/mp11.hpp>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**USING\_STD\_TYPETRAIT**](aliases_8hpp.md#define-using_std_typetrait) (trait) `template &lt;class T&gt; using trait = std::trait&lt;T&gt;;`<br> |
| define  | [**USING\_STD\_TYPETRAIT\_BINARY**](aliases_8hpp.md#define-using_std_typetrait_binary) (trait) `/* multi line expression */`<br> |
| define  | [**USING\_STD\_TYPETRAIT\_PROPERTY**](aliases_8hpp.md#define-using_std_typetrait_property) (trait) `USING\_STD\_TYPETRAIT(trait) USING\_TYPETRAIT\_V(trait)`<br> |
| define  | [**USING\_STD\_TYPETRAIT\_TRANSFORM**](aliases_8hpp.md#define-using_std_typetrait_transform) (trait) `USING\_STD\_TYPETRAIT(trait) USING\_TYPETRAIT\_T(trait)`<br> |
| define  | [**USING\_TYPETRAIT\_T**](aliases_8hpp.md#define-using_typetrait_t) (trait) `template&lt;class T&gt; using trait##\_t = typename trait&lt;T&gt;::type;`<br> |
| define  | [**USING\_TYPETRAIT\_V**](aliases_8hpp.md#define-using_typetrait_v) (trait) `template&lt;class T&gt; static constexpr bool trait##\_v = trait&lt;T&gt;::value;`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2022-2023 Volvo Car Corporation 




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define USING\_STD\_TYPETRAIT 

```C++
#define USING_STD_TYPETRAIT (
    trait
) `template <class T> using trait = std::trait<T>;`
```




<hr>



### define USING\_STD\_TYPETRAIT\_BINARY 

```C++
#define USING_STD_TYPETRAIT_BINARY (
    trait
) `/* multi line expression */`
```




<hr>



### define USING\_STD\_TYPETRAIT\_PROPERTY 

```C++
#define USING_STD_TYPETRAIT_PROPERTY (
    trait
) `USING_STD_TYPETRAIT(trait) USING_TYPETRAIT_V(trait)`
```




<hr>



### define USING\_STD\_TYPETRAIT\_TRANSFORM 

```C++
#define USING_STD_TYPETRAIT_TRANSFORM (
    trait
) `USING_STD_TYPETRAIT(trait) USING_TYPETRAIT_T(trait)`
```




<hr>



### define USING\_TYPETRAIT\_T 

```C++
#define USING_TYPETRAIT_T (
    trait
) `template<class T> using trait##_t = typename trait<T>::type;`
```




<hr>



### define USING\_TYPETRAIT\_V 

```C++
#define USING_TYPETRAIT_V (
    trait
) `template<class T> static constexpr bool trait##_v = trait<T>::value;`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/aliases.hpp`

