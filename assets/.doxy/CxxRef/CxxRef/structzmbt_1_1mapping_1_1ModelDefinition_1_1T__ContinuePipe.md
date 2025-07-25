

# Struct zmbt::mapping::ModelDefinition::T\_ContinuePipe

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_ContinuePipe**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ContinuePipe.md)



_T\_ContinuePipe transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_ChannelEnd](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__ChannelEnd.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**Blend**](#function-blend) () <br> |
|  Target | [**Group**](#function-group) () <br> |


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




### function Blend 

```C++
inline Target T_ContinuePipe::Blend () 
```



Combine channel outputs in time series


Combined channels produce a list of pairs [channel alias, requested signal], sorted by timestamp. Use it in combination with Saturate expr for testing strict or partial order on mock calls. 


        

<hr>



### function Group 

```C++
inline Target T_ContinuePipe::Group () 
```



Pack channel outputs into an array similarly to Fork keyword X with Y with Z -&gt; [X, Y, Z] 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

