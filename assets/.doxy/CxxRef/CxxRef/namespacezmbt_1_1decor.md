

# Namespace zmbt::decor



[**Namespace List**](namespaces.md) **>** [**zmbt**](namespacezmbt.md) **>** [**decor**](namespacezmbt_1_1decor.md)



_Signal type decorators API._ 
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**precise**](structzmbt_1_1decor_1_1precise.md) &lt;class VT&gt;<br>_Type decorator for floating point numbers. Throws on precision loss in runtime._  |
| struct | [**precision\_loss\_error**](structzmbt_1_1decor_1_1precision__loss__error.md) <br>_Exception thrown by_ [_**decor::precise**_](structzmbt_1_1decor_1_1precise.md) _._ |
| struct | [**underlying**](structzmbt_1_1decor_1_1underlying.md) &lt;class E&gt;<br> |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr [**zmbt::type\_tag**](structzmbt_1_1type__tag.md)&lt; [**precise**](structzmbt_1_1decor_1_1precise.md)&lt; T &gt; &gt; | [**Precise**](#variable-precise)  <br>_precise&lt;T&gt; decorator_  |
|  constexpr [**zmbt::type\_tag**](structzmbt_1_1type__tag.md)&lt; [**underlying**](structzmbt_1_1decor_1_1underlying.md)&lt; T &gt; &gt; | [**Underlying**](#variable-underlying)  <br>_underlying&lt;T&gt; decorator_  |












































## Public Attributes Documentation




### variable Precise 

_precise&lt;T&gt; decorator_ 
```C++
constexpr zmbt::type_tag<precise<T> > zmbt::decor::Precise;
```





**Template parameters:**


* `T` 




        

<hr>



### variable Underlying 

_underlying&lt;T&gt; decorator_ 
```C++
constexpr zmbt::type_tag<underlying<T> > zmbt::decor::Underlying;
```





**Template parameters:**


* `T` 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/decor/precise_real.hpp`

