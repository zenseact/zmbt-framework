

# Namespace zmbt::reflect



[**Namespace List**](namespaces.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md)



_Reflection metafunctions._ 
















## Classes

| Type | Name |
| ---: | :--- |
| class | [**Prototypes**](classzmbt_1_1reflect_1_1Prototypes.md) <br>_Interface prototypes in JSON as {"args": [...], "return": ... }._  |
| struct | [**custom\_invocation**](structzmbt_1_1reflect_1_1custom__invocation.md) &lt;class T, class E&gt;<br>_Customize the interface reflection metafunction._  |
| struct | [**custom\_serialization**](structzmbt_1_1reflect_1_1custom__serialization.md) &lt;class T, class&gt;<br>_User-defined serialization._  |
| struct | [**custom\_serialization&lt; ChannelKind &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01ChannelKind_01_4.md) &lt;&gt;<br> |
| struct | [**custom\_serialization&lt; T, mp\_if&lt; is\_base\_of&lt; lang::Expression, T &gt;, void &gt; &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01T_00_01mp__if_3_01is__base__of_3_01lang_1_1Ex2d769b9dfa1b415c78e21b0b59756cc2.md) &lt;class T&gt;<br> |
| struct | [**custom\_serialization&lt; lang::Encoding &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01lang_1_1Encoding_01_4.md) &lt;&gt;<br> |
| struct | [**custom\_serialization&lt; lang::ExpressionView &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01lang_1_1ExpressionView_01_4.md) &lt;&gt;<br> |
| struct | [**custom\_signal\_traits**](structzmbt_1_1reflect_1_1custom__signal__traits.md) &lt;class T, class E&gt;<br>_Customize signal traits metafunction._  |
| struct | [**default\_invocation**](structzmbt_1_1reflect_1_1default__invocation.md) &lt;class S, class E&gt;<br>_Interface reflection metafunction._  |
| struct | [**default\_invocation&lt; S, first\_if\_any\_t&lt; void, ifc\_is\_fn\_handle&lt; S &gt;, ifc\_is\_functor\_ref&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__any__t_3_01void_00_01ifc__is_03ee2f30da4b8a63c386d237830e07c9.md) &lt;class S&gt;<br> |
| struct | [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_functor\_ptr&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__functor__ptr_3_01S_01_4_01_4_01_4.md) &lt;class S&gt;<br> |
| struct | [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_member\_handle&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__member__handle_3_01S_01_4_01_4_01_4.md) &lt;class S&gt;<br> |
| struct | [**has\_invocation\_for**](structzmbt_1_1reflect_1_1has__invocation__for.md) &lt;class T, class E&gt;<br>_Trait check for reflect::invocation&lt;T&gt;_  |
| struct | [**invocation**](structzmbt_1_1reflect_1_1invocation.md) &lt;class T, class E&gt;<br>_Interface reflection metafunction._  |
| struct | [**invocation&lt; S, detail::enable\_custom\_invocation&lt; S &gt; &gt;**](structzmbt_1_1reflect_1_1invocation_3_01S_00_01detail_1_1enable__custom__invocation_3_01S_01_4_01_4.md) &lt;class S&gt;<br> |
| struct | [**invocation&lt; S, detail::enable\_default\_invocation&lt; S &gt; &gt;**](structzmbt_1_1reflect_1_1invocation_3_01S_00_01detail_1_1enable__default__invocation_3_01S_01_4_01_4.md) &lt;class S&gt;<br> |
| struct | [**serialization**](structzmbt_1_1reflect_1_1serialization.md) &lt;class T, class&gt;<br>_Serialization metafunction._  |
| struct | [**serialization&lt; T, detail::enable\_for\_custom\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__custom__serialization_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**serialization&lt; T, detail::enable\_for\_default\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__default__serialization_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**signal\_traits**](structzmbt_1_1reflect_1_1signal__traits.md) &lt;class T, class E&gt;<br>_Signal traits metafunction, providing default values._  |
| struct | [**signal\_traits&lt; T, detail::enable\_custom\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__custom__signal__traits_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**signal\_traits&lt; T, detail::enable\_default\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__default__signal__traits_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**signal\_traits&lt; T, detail::enable\_initialization\_trap&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__initialization__trap_3_01T_01_4_01_4.md) &lt;class T&gt;<br>_Unhandled types catcher._  |
| struct | [**signal\_traits&lt; void &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01void_01_4.md) &lt;&gt;<br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**custom\_signal\_traits**](structzmbt_1_1reflect_1_1custom__signal__traits.md)&lt; T, E &gt; | [**custom\_initialization**](#typedef-custom_initialization)  <br> |
| typedef [**signal\_traits**](structzmbt_1_1reflect_1_1signal__traits.md)&lt; T, E &gt; | [**initialization**](#typedef-initialization)  <br> |
| typedef typename [**reflect::invocation**](structzmbt_1_1reflect_1_1invocation.md)&lt; Interface &gt;[**::type**](namespacezmbt.md#variable-type) | [**interface\_t**](#typedef-interface_t)  <br> |
| typedef typename [**invocation**](structzmbt_1_1reflect_1_1invocation.md)&lt; Interface &gt;::args\_t | [**invocation\_args\_t**](#typedef-invocation_args_t)  <br>_Interface arguments tuple type._  |
| typedef tuple\_unqf\_t&lt; [**invocation\_args\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_args_t)&lt; Interface &gt; &gt; | [**invocation\_args\_unqf\_t**](#typedef-invocation_args_unqf_t)  <br>_Interface cv-unqualified arguments tuple type._  |
| typedef std::integral\_constant&lt; bool, 0==boost::mp11::mp\_size&lt; [**invocation\_args\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_args_t)&lt; Interface &gt; &gt;::value &gt; | [**invocation\_has\_void\_args**](#typedef-invocation_has_void_args)  <br>_Interface has no args._  |
| typedef std::is\_void&lt; [**invocation\_ret\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_ret_t)&lt; Interface &gt; &gt; | [**invocation\_has\_void\_ret**](#typedef-invocation_has_void_ret)  <br>_Interface has no return._  |
| typedef typename [**invocation**](structzmbt_1_1reflect_1_1invocation.md)&lt; T &gt;::host\_t | [**invocation\_host\_t**](#typedef-invocation_host_t)  <br>_Interface host object type._  |
| typedef typename [**invocation**](structzmbt_1_1reflect_1_1invocation.md)&lt; Interface &gt;::return\_t | [**invocation\_ret\_t**](#typedef-invocation_ret_t)  <br>_Interface return type._  |
| typedef mp\_if&lt; [**invocation\_has\_void\_ret**](namespacezmbt_1_1reflect.md#typedef-invocation_has_void_ret)&lt; Interface &gt;, nullptr\_t, [**invocation\_ret\_unqf\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_ret_unqf_t)&lt; Interface &gt; &gt; | [**invocation\_ret\_unqf\_or\_nullptr\_t**](#typedef-invocation_ret_unqf_or_nullptr_t)  <br> |
| typedef remove\_cvref\_t&lt; [**invocation\_ret\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_ret_t)&lt; Interface &gt; &gt; | [**invocation\_ret\_unqf\_t**](#typedef-invocation_ret_unqf_t)  <br>_Interface cv-unqualified return type._  |
| typedef remove\_cvref\_t&lt; [**invocation\_host\_t**](namespacezmbt_1_1reflect.md#typedef-invocation_host_t)&lt; T &gt; &gt; | [**invocation\_unqf\_host\_t**](#typedef-invocation_unqf_host_t)  <br>_Interface cv-unqualified host object type._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr T | [**init\_tuple**](#function-init_tuple) () <br>_Initialize tuple with default values provided by_ [_**signal\_traits**_](structzmbt_1_1reflect_1_1signal__traits.md) _._ |
|  constexpr T | [**init\_value**](#function-init_value) () <br>_Initialize value of type T with default value provided by_ [_**signal\_traits**_](structzmbt_1_1reflect_1_1signal__traits.md) _._ |
|  detail::enable\_hermetic\_serialization&lt; T, std::ostream & &gt; | [**operator&lt;&lt;**](#function-operator) (std::ostream & os, T const & value) <br>_ADL injection wrapper enabling operator&lt;&lt;._  |
|  detail::enable\_hermetic\_serialization&lt; T, void &gt; | [**tag\_invoke**](#function-tag_invoke) (boost::json::value\_from\_tag const &, boost::json::value & v, T const & t) <br>_ADL injection wrapper enabling boost::json::value\_from&lt;T&gt; conversion._  |
|  detail::enable\_hermetic\_serialization&lt; T, T &gt; | [**tag\_invoke**](#function-tag_invoke) (boost::json::value\_to\_tag&lt; T &gt; const &, boost::json::value const & v) <br>_ADL injection wrapper enabling boost::json::value\_from&lt;T&gt; conversion._  |




























## Public Types Documentation




### typedef custom\_initialization 

```C++
using zmbt::reflect::custom_initialization = typedef custom_signal_traits<T, E>;
```




<hr>



### typedef initialization 

```C++
using zmbt::reflect::initialization = typedef signal_traits<T, E>;
```




<hr>



### typedef interface\_t 

```C++
using zmbt::reflect::interface_t = typedef typename reflect::invocation<Interface>::type;
```




<hr>



### typedef invocation\_args\_t 

_Interface arguments tuple type._ 
```C++
using zmbt::reflect::invocation_args_t = typedef typename invocation<Interface>::args_t;
```





**Template parameters:**


* `Interface` Interface pointer type 




        

<hr>



### typedef invocation\_args\_unqf\_t 

_Interface cv-unqualified arguments tuple type._ 
```C++
using zmbt::reflect::invocation_args_unqf_t = typedef tuple_unqf_t<invocation_args_t<Interface> >;
```





**Template parameters:**


* `Interface` Interface pointer type 




        

<hr>



### typedef invocation\_has\_void\_args 

_Interface has no args._ 
```C++
using zmbt::reflect::invocation_has_void_args = typedef std::integral_constant<bool, 0 == boost::mp11::mp_size<invocation_args_t<Interface> >::value >;
```





**Template parameters:**


* `Interface` 




        

<hr>



### typedef invocation\_has\_void\_ret 

_Interface has no return._ 
```C++
using zmbt::reflect::invocation_has_void_ret = typedef std::is_void<invocation_ret_t<Interface> >;
```





**Template parameters:**


* `Interface` 




        

<hr>



### typedef invocation\_host\_t 

_Interface host object type._ 
```C++
using zmbt::reflect::invocation_host_t = typedef typename invocation<T>::host_t;
```





**Template parameters:**


* `T` Interface pointer type 




        

<hr>



### typedef invocation\_ret\_t 

_Interface return type._ 
```C++
using zmbt::reflect::invocation_ret_t = typedef typename invocation<Interface>::return_t;
```





**Template parameters:**


* `Interface` Interface pointer type 




        

<hr>



### typedef invocation\_ret\_unqf\_or\_nullptr\_t 

```C++
using zmbt::reflect::invocation_ret_unqf_or_nullptr_t = typedef mp_if<invocation_has_void_ret<Interface>, nullptr_t, invocation_ret_unqf_t<Interface> >;
```




<hr>



### typedef invocation\_ret\_unqf\_t 

_Interface cv-unqualified return type._ 
```C++
using zmbt::reflect::invocation_ret_unqf_t = typedef remove_cvref_t<invocation_ret_t<Interface> >;
```





**Template parameters:**


* `Interface` Interface pointer type 




        

<hr>



### typedef invocation\_unqf\_host\_t 

_Interface cv-unqualified host object type._ 
```C++
using zmbt::reflect::invocation_unqf_host_t = typedef remove_cvref_t<invocation_host_t<T> >;
```





**Template parameters:**


* `T` Interface pointer type 




        

<hr>
## Public Functions Documentation




### function init\_tuple 

_Initialize tuple with default values provided by_ [_**signal\_traits**_](structzmbt_1_1reflect_1_1signal__traits.md) _._
```C++
template<class T>
constexpr T zmbt::reflect::init_tuple () 
```




<hr>



### function init\_value 

_Initialize value of type T with default value provided by_ [_**signal\_traits**_](structzmbt_1_1reflect_1_1signal__traits.md) _._
```C++
template<class T>
constexpr T zmbt::reflect::init_value () 
```




<hr>



### function operator&lt;&lt; 

_ADL injection wrapper enabling operator&lt;&lt;._ 
```C++
template<class T>
detail::enable_hermetic_serialization< T, std::ostream & > zmbt::reflect::operator<< (
    std::ostream & os,
    T const & value
) 
```




<hr>



### function tag\_invoke 

_ADL injection wrapper enabling boost::json::value\_from&lt;T&gt; conversion._ 
```C++
template<class T>
detail::enable_hermetic_serialization< T, void > zmbt::reflect::tag_invoke (
    boost::json::value_from_tag const &,
    boost::json::value & v,
    T const & t
) 
```




<hr>



### function tag\_invoke 

_ADL injection wrapper enabling boost::json::value\_from&lt;T&gt; conversion._ 
```C++
template<class T>
detail::enable_hermetic_serialization< T, T > zmbt::reflect::tag_invoke (
    boost::json::value_to_tag< T > const &,
    boost::json::value const & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/decor/precise_real.hpp`

