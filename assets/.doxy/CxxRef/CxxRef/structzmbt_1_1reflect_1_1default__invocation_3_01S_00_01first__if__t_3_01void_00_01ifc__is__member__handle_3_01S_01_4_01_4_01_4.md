

# Struct zmbt::reflect::default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_member\_handle&lt; S &gt; &gt; &gt;

**template &lt;class S&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md) **>** [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_member\_handle&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__member__handle_3_01S_01_4_01_4_01_4.md)






















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
|  return\_t | [**apply**](#function-apply) (H && object, type ifc, args\_t args) <br> |


























## Public Types Documentation




### typedef args\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_member_handle< S > > >::args_t =  ifc_args_t<type>;
```




<hr>



### typedef host\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_member_handle< S > > >::host_t =  ifc_host_t<type>;
```




<hr>



### typedef return\_t 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_member_handle< S > > >::return_t =  ifc_return_t<type>;
```




<hr>



### typedef type 

```C++
using zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_member_handle< S > > >::type =  S;
```




<hr>
## Public Static Functions Documentation




### function apply 

```C++
template<class H>
static inline return_t zmbt::reflect::default_invocation< S, first_if_t< void, ifc_is_member_handle< S > > >::apply (
    H && object,
    type ifc,
    args_t args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/invocation.hpp`

