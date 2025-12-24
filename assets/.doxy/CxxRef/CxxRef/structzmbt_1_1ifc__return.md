

# Struct zmbt::ifc\_return

**template &lt;class Interface&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**ifc\_return**](structzmbt_1_1ifc__return.md)








Inherits the following classes: boost::callable_traits::return_type< Interface >














## Public Types

| Type | Name |
| ---: | :--- |
| typedef typename mp\_eval\_or&lt; mp\_identity&lt; void &gt;, ct::return\_type, ifc\_handle\_t&lt; Interface &gt; &gt;::type | [**type**](#typedef-type)  <br> |
















































## Public Types Documentation




### typedef type 

```C++
using zmbt::ifc_return< Interface >::type =  typename mp_eval_or<mp_identity<void>, ct::return_type, ifc_handle_t<Interface> >::type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/interface_traits.hpp`

