

# Struct zmbt::ifc\_host

**template &lt;class Interface, class&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**ifc\_host**](structzmbt_1_1ifc__host.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef typename mp\_if&lt; ifc\_is\_member\_handle&lt; Interface &gt;, ct::qualified\_class\_of&lt; Interface &gt;, mp\_identity&lt; nullptr\_t &gt; &gt;::type | [**type**](#typedef-type)  <br> |
















































## Public Types Documentation




### typedef type 

```C++
using zmbt::ifc_host< Interface, class >::type =  typename mp_if<ifc_is_member_handle<Interface>, ct::qualified_class_of<Interface>, mp_identity<nullptr_t> >::type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/interface_traits.hpp`

