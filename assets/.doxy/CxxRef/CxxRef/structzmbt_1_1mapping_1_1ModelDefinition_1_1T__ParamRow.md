

# Struct zmbt::mapping::ModelDefinition::T\_ParamRow

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_ParamRow**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ParamRow.md)



_T\_ParamRow transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_ParamTable](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__ParamTable.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**operator()**](#function-operator) ([**Param**](classzmbt_1_1Param.md) const & p, A &&... args) <br> |
|  Target | [**operator()**](#function-operator_1) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0) <br> |
|  Target | [**operator()**](#function-operator_2) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1) <br> |
|  Target | [**operator()**](#function-operator_3) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2) <br> |
|  Target | [**operator()**](#function-operator_4) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3) <br> |
|  Target | [**operator()**](#function-operator_5) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4) <br> |
|  Target | [**operator()**](#function-operator_6) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5) <br> |
|  Target | [**operator()**](#function-operator_7) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6) <br> |
|  Target | [**operator()**](#function-operator_8) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6, boost::json::value const & v7) <br> |
|  Target | [**operator()**](#function-operator_9) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6, boost::json::value const & v7, A &&... args) <br> |


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




### function operator() 

```C++
template<class... A>
inline Target T_ParamRow::operator() (
    Param const & p,
    A &&... args
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3,
    boost::json::value const & v4
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3,
    boost::json::value const & v4,
    boost::json::value const & v5
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3,
    boost::json::value const & v4,
    boost::json::value const & v5,
    boost::json::value const & v6
) 
```




<hr>



### function operator() 

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3,
    boost::json::value const & v4,
    boost::json::value const & v5,
    boost::json::value const & v6,
    boost::json::value const & v7
) 
```




<hr>



### function operator() 

```C++
template<class... A>
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2,
    boost::json::value const & v3,
    boost::json::value const & v4,
    boost::json::value const & v5,
    boost::json::value const & v6,
    boost::json::value const & v7,
    A &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

