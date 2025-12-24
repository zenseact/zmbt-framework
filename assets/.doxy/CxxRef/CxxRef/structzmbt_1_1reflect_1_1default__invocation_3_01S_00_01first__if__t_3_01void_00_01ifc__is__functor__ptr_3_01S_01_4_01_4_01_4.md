

# Struct zmbt::reflect::default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_functor\_ptr&lt; S &gt; &gt; &gt;

**template &lt;class S&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md) **>** [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_functor\_ptr&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__functor__ptr_3_01S_01_4_01_4_01_4.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef ifc\_args\_t&lt; type &gt; | [**args\_t**](#typedef-args_t)  <br> |
| typedef ifc\_host\_t&lt; type &gt; | [**host\_t**](#typedef-host_t)  <br> |
| typedef ifc\_return\_t&lt; type &gt; | [**return\_t**](#typedef-return_t)  <br> |
| typedef S | [**type**](#typedef-type)  <br> |






















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  return\_t | [**apply**](#function-apply) (host\_t, type ifc, args\_t args) <br> |


























## Public Types Documentation




### typedef args\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_functor_ptr< S > > >::args_t =  ifc_args_t<type>;
```




<hr>



### typedef host\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_functor_ptr< S > > >::host_t =  ifc_host_t<type>;
```




<hr>



### typedef return\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_functor_ptr< S > > >::return_t =  ifc_return_t<type>;
```




<hr>



### typedef type 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_functor_ptr< S > > >::type =  S;
```




<hr>
## Public Static Functions Documentation




### function apply 

```C++
static inline return_t zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_functor_ptr< S > > >::apply (
    host_t,
    type ifc,
    args_t args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/invocation.hpp`

