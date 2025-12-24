

# Struct zmbt::reflect::custom\_invocation

**template &lt;class T, class E&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md) **>** [**custom\_invocation**](structzmbt_1_1reflect_1_1custom__invocation.md)



_Customize the interface reflection metafunction._ [More...](#detailed-description)


































































## Detailed Description


Provide custom type definitions and 'apply' method for callables. Required fields:
* type: callable type (metafunction template parameter)
* host\_t: host object type
* return\_t: callable return type
* args\_t: callable arguments tuple type
* apply: callable invocation method with signature:
  * If host\_t is nullptr\_t: 
```C++
static return_t apply(nullptr_t, type ifc, args_t args);
```

  * If host\_t is class: 
```C++
template <class H>
static return_t apply(H&& object, type ifc, args_t args);
```







For class methods, the `apply` function must support references and pointers, including smart pointers. Therefore, a templated version of the `apply` is recommended but can be replaced with a set of overloads if necessary.




**Template parameters:**


* `T` callable type 
* `E` SFINAE enabler 




    

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/invocation.hpp`

