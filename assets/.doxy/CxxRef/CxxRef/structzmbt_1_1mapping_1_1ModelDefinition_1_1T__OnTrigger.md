

# Struct zmbt::mapping::ModelDefinition::T\_OnTrigger

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_OnTrigger**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__OnTrigger.md)



_T\_OnTrigger transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_Main](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**OnTrigger**](#function-ontrigger-17) (boost::json::string\_view key, T &&... fmtargs) <br> |
|  Target | [**OnTrigger**](#function-ontrigger-27) ([**Param**](classzmbt_1_1Param.md) const & key) <br> |
|  Target | [**OnTrigger**](#function-ontrigger-37) ([**Param**](classzmbt_1_1Param.md) const & ifc, [**Param**](classzmbt_1_1Param.md) const & obj) <br> |
|  require\_literal&lt; O, Interface, Target &gt; | [**OnTrigger**](#function-ontrigger-47) (Interface && ifc, O && obj) <br> |
|  require\_cal&lt; Interface, Target &gt; | [**OnTrigger**](#function-ontrigger-57) (Interface && ifc) <br> |
|  require\_cal&lt; Interface, Target &gt; | [**OnTrigger**](#function-ontrigger-67) (Interface && ifc, [**Param**](classzmbt_1_1Param.md) const & obj) <br> |
|  require\_obj&lt; H, Target &gt; | [**OnTrigger**](#function-ontrigger-77) ([**Param**](classzmbt_1_1Param.md) const & ifc, H && obj) <br> |


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




### function OnTrigger [1/7]

```C++
template<class... T>
inline Target T_OnTrigger::OnTrigger (
    boost::json::string_view key,
    T &&... fmtargs
) 
```




<hr>



### function OnTrigger [2/7]

```C++
inline Target T_OnTrigger::OnTrigger (
    Param const & key
) 
```




<hr>



### function OnTrigger [3/7]

```C++
inline Target T_OnTrigger::OnTrigger (
    Param const & ifc,
    Param const & obj
) 
```




<hr>



### function OnTrigger [4/7]

```C++
template<class O, class Interface>
inline require_literal< O, Interface, Target > T_OnTrigger::OnTrigger (
    Interface && ifc,
    O && obj
) 
```




<hr>



### function OnTrigger [5/7]

```C++
template<class Interface>
inline require_cal< Interface, Target > T_OnTrigger::OnTrigger (
    Interface && ifc
) 
```




<hr>



### function OnTrigger [6/7]

```C++
template<class Interface>
inline require_cal< Interface, Target > T_OnTrigger::OnTrigger (
    Interface && ifc,
    Param const & obj
) 
```




<hr>



### function OnTrigger [7/7]

```C++
template<class H>
inline require_obj< H, Target > T_OnTrigger::OnTrigger (
    Param const & ifc,
    H && obj
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

