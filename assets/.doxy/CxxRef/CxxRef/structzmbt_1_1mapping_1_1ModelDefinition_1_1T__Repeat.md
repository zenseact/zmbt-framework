

# Struct zmbt::mapping::ModelDefinition::T\_Repeat

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_Repeat**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Repeat.md)



_T\_Repeat transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_Repeat](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Repeat.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**Repeat**](#function-repeat-12) (std::size\_t N) <br>_Run trigger N times._  |
|  Target | [**Repeat**](#function-repeat-22) ([**Param**](classzmbt_1_1Param.md) const & N) <br>_Run trigger N times._  |


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




### function Repeat [1/2]

_Run trigger N times._ 
```C++
inline Target T_Repeat::Repeat (
    std::size_t N
) 
```




<hr>



### function Repeat [2/2]

_Run trigger N times._ 
```C++
inline Target T_Repeat::Repeat (
    Param const & N
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

