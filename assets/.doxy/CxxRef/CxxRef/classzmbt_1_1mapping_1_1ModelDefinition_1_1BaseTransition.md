

# Class zmbt::mapping::ModelDefinition::BaseTransition



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**mapping**](namespacezmbt_1_1mapping.md) **>** [**ModelDefinition**](classzmbt_1_1mapping_1_1ModelDefinition.md) **>** [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)










Inherited by the following classes: [zmbt::mapping::ModelDefinition::T\_As](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__As.md),  [zmbt::mapping::ModelDefinition::T\_At](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__At.md),  [zmbt::mapping::ModelDefinition::T\_ContinuePipe](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ContinuePipe.md),  [zmbt::mapping::ModelDefinition::T\_CustomParam](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__CustomParam.md),  [zmbt::mapping::ModelDefinition::T\_Description](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Description.md),  [zmbt::mapping::ModelDefinition::T\_Filter](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Filter.md),  [zmbt::mapping::ModelDefinition::T\_OnTrigger](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__OnTrigger.md),  [zmbt::mapping::ModelDefinition::T\_Param](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Param.md),  [zmbt::mapping::ModelDefinition::T\_ParamRow](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ParamRow.md),  [zmbt::mapping::ModelDefinition::T\_PipeId](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PipeId.md),  [zmbt::mapping::ModelDefinition::T\_PostRun](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PostRun.md),  [zmbt::mapping::ModelDefinition::T\_PreRun](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PreRun.md),  [zmbt::mapping::ModelDefinition::T\_Repeat](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Repeat.md),  [zmbt::mapping::ModelDefinition::T\_Tag](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Tag.md),  [zmbt::mapping::ModelDefinition::T\_Take](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Take.md),  [zmbt::mapping::ModelDefinition::T\_TerminatePipe](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__TerminatePipe.md),  [zmbt::mapping::ModelDefinition::T\_Test](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Test.md),  [zmbt::mapping::ModelDefinition::T\_TestRow](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__TestRow.md),  [zmbt::mapping::ModelDefinition::N\_Term](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Term.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual  | [**~BaseTransition**](#function-basetransition) () <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**BaseTransition**](#function-basetransition-14) (detail::DefinitionHelper && m) <br> |
|   | [**BaseTransition**](#function-basetransition-24) () <br> |
|   | [**BaseTransition**](#function-basetransition-34) ([**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md) const &) = delete<br> |
|   | [**BaseTransition**](#function-basetransition-44) ([**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md) &&) = default<br> |
|  detail::DefinitionHelper & | [**state**](#function-state) () <br> |
|  Target | [**transit\_to**](#function-transit_to) () <br> |




## Public Functions Documentation




### function ~BaseTransition 

```C++
inline virtual zmbt::mapping::ModelDefinition::BaseTransition::~BaseTransition () 
```




<hr>
## Protected Functions Documentation




### function BaseTransition [1/4]

```C++
inline zmbt::mapping::ModelDefinition::BaseTransition::BaseTransition (
    detail::DefinitionHelper && m
) 
```




<hr>



### function BaseTransition [2/4]

```C++
inline zmbt::mapping::ModelDefinition::BaseTransition::BaseTransition () 
```




<hr>



### function BaseTransition [3/4]

```C++
zmbt::mapping::ModelDefinition::BaseTransition::BaseTransition (
    BaseTransition const &
) = delete
```




<hr>



### function BaseTransition [4/4]

```C++
zmbt::mapping::ModelDefinition::BaseTransition::BaseTransition (
    BaseTransition &&
) = default
```




<hr>



### function state 

```C++
inline detail::DefinitionHelper & zmbt::mapping::ModelDefinition::BaseTransition::state () 
```




<hr>



### function transit\_to 

```C++
template<class Target>
inline Target zmbt::mapping::ModelDefinition::BaseTransition::transit_to () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_node.hpp`

