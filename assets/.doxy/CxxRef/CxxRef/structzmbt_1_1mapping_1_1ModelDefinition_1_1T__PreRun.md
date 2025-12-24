

# Struct zmbt::mapping::ModelDefinition::T\_PreRun

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_PreRun**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PreRun.md)



_T\_PreRun transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_Pre](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Pre.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**PreRun**](#function-prerun) (T &&... tasks) <br> |


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




### function PreRun 

```C++
template<class... T>
inline Target T_PreRun::PreRun (
    T &&... tasks
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

