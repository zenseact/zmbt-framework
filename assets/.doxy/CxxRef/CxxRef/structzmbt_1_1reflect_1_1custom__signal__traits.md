

# Struct zmbt::reflect::custom\_signal\_traits

**template &lt;class T, class E&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**reflect**](namespacezmbt_1_1reflect.md) **>** [**custom\_signal\_traits**](structzmbt_1_1reflect_1_1custom__signal__traits.md)



_Customize signal traits metafunction._ [More...](#detailed-description)






Inherited by the following classes: [zmbt::reflect::signal\_traits&lt; T, detail::enable\_custom\_signal\_traits&lt; T &gt; &gt;](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__custom__signal__traits_3_01T_01_4_01_4.md)




























































## Detailed Description


Provide static 'init' method for signal initialization with signature: 
```C++
static constexpr T init();
```
 

**Template parameters:**


* `T` signal type 
* `E` SFINAE enabler 




    

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/reflect/signal_traits.hpp`

