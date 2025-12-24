

# Struct zmbt::mapping::ModelDefinition::T\_At

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_At**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__At.md)



_T\_At transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_Channel](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Channel.md),  [zmbt::mapping::ModelDefinition::N\_MaybeChannel](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__MaybeChannel.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  require\_not\_str&lt; Interface, Target &gt; | [**At**](#function-at-14) (Interface && cal, [**object\_id**](classzmbt_1_1object__id.md) const & obj) <br>_Create input channel with an interface literal._  |
|  require\_not\_str&lt; Interface, Target &gt; | [**At**](#function-at-24) (Interface && cal, [**Param**](classzmbt_1_1Param.md) const & obj) <br> |
|  require\_not\_str&lt; Interface, Target &gt; | [**At**](#function-at-34) (Interface && cal) <br>_Create input channel._  |
|  Target | [**At**](#function-at-44) (boost::json::string\_view key, T &&... fmtargs) <br> |


## Public Functions inherited from zmbt::mapping::ModelDefinition::BaseTransition

See [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)

| Type | Name |
| ---: | :--- |
| virtual  | [**~BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-basetransition) () <br> |
















































## Protected Functions inherited from zmbt::mapping::ModelDefinition::BaseTransition

See [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)

| Type | Name |
| ---: | :--- |
|   | [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-basetransition-14) (detail::DefinitionHelper && m) <br> |
|   | [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-basetransition-24) () <br> |
|   | [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-basetransition-34) ([**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md) const &) = delete<br> |
|   | [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-basetransition-44) ([**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md) &&) = default<br> |
|  detail::DefinitionHelper & | [**state**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-state) () <br> |
|  Target | [**transit\_to**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md#function-transit_to) () <br> |






## Public Functions Documentation




### function At [1/4]

_Create input channel with an interface literal._ 
```C++
template<class Interface>
inline require_not_str< Interface, Target > T_At::At (
    Interface && cal,
    object_id const & obj
) 
```




<hr>



### function At [2/4]

```C++
template<class Interface>
inline require_not_str< Interface, Target > T_At::At (
    Interface && cal,
    Param const & obj
) 
```




<hr>



### function At [3/4]

_Create input channel._ 
```C++
template<class Interface>
inline require_not_str< Interface, Target > T_At::At (
    Interface && cal
) 
```




<hr>



### function At [4/4]

```C++
template<class... T>
inline Target T_At::At (
    boost::json::string_view key,
    T &&... fmtargs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

