

# Struct zmbt::mapping::ModelDefinition::T\_ParamRow

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_ParamRow**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ParamRow.md)



_T\_ParamRow transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_ParamTable](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__ParamTable.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**operator()**](#function-operator()-110) ([**Param**](classzmbt_1_1Param.md) const & p, A &&... args) <br> |
|  Target | [**operator()**](#function-operator()-210) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0) <br> |
|  Target | [**operator()**](#function-operator()-310) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1) <br> |
|  Target | [**operator()**](#function-operator()-410) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2) <br> |
|  Target | [**operator()**](#function-operator()-510) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3) <br> |
|  Target | [**operator()**](#function-operator()-610) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4) <br> |
|  Target | [**operator()**](#function-operator()-710) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5) <br> |
|  Target | [**operator()**](#function-operator()-810) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6) <br> |
|  Target | [**operator()**](#function-operator()-910) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6, boost::json::value const & v7) <br> |
|  Target | [**operator()**](#function-operator()-1010) ([**Param**](classzmbt_1_1Param.md) const & p, boost::json::value const & v0, boost::json::value const & v1, boost::json::value const & v2, boost::json::value const & v3, boost::json::value const & v4, boost::json::value const & v5, boost::json::value const & v6, boost::json::value const & v7, A &&... args) <br> |


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




### function operator() [1/10]

```C++
template<class... A>
inline Target T_ParamRow::operator() (
    Param const & p,
    A &&... args
) 
```




<hr>



### function operator() [2/10]

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0
) 
```




<hr>



### function operator() [3/10]

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1
) 
```




<hr>



### function operator() [4/10]

```C++
inline Target T_ParamRow::operator() (
    Param const & p,
    boost::json::value const & v0,
    boost::json::value const & v1,
    boost::json::value const & v2
) 
```




<hr>



### function operator() [5/10]

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



### function operator() [6/10]

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



### function operator() [7/10]

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



### function operator() [8/10]

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



### function operator() [9/10]

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



### function operator() [10/10]

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

