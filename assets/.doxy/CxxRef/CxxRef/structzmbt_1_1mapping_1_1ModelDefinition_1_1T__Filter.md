

# Struct zmbt::mapping::ModelDefinition::T\_Filter

**template &lt;class Target&gt;**



[**ClassList**](annotated.md) **>** [**T\_Filter**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Filter.md)



_T\_Filter transition._ 




Inherits the following classes: [zmbt::mapping::ModelDefinition::BaseTransition](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)


Inherited by the following classes: [zmbt::mapping::ModelDefinition::N\_Filter](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Filter.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Target | [**Args**](#function-args-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e="$(default)") <br> |
|  Target | [**Args**](#function-args-22) (boost::json::string\_view fmt, T && arg1, Rest &&... args\_rest) <br>_Interface argument clause with printf-like format._  |
|  Target | [**CallCount**](#function-callcount) () <br> |
|  Target | [**Exception**](#function-exception) () <br>_Interface exception._  |
|  Target | [**Return**](#function-return-12) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & e="") <br> |
|  Target | [**Return**](#function-return-22) (boost::json::string\_view fmt, T && arg1, Rest &&... args\_rest) <br>_Interface return clause with printf-like format._  |
|  Target | [**ThreadId**](#function-threadid) () <br>_Output capture thread id._  |
|  Target | [**Timestamp**](#function-timestamp) () <br>_Output capture timestamp._  |


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




### function Args [1/2]

```C++
inline Target T_Filter::Args (
    lang::Expression const & e="$(default)"
) 
```



Interface argument clause Refers to the arguments subsignal at the given JSON Pointer 


        

<hr>



### function Args [2/2]

_Interface argument clause with printf-like format._ 
```C++
template<class T, class... Rest>
inline Target T_Filter::Args (
    boost::json::string_view fmt,
    T && arg1,
    Rest &&... args_rest
) 
```




<hr>



### function CallCount 

```C++
inline Target T_Filter::CallCount () 
```




<hr>



### function Exception 

_Interface exception._ 
```C++
inline Target T_Filter::Exception () 
```




<hr>



### function Return [1/2]

```C++
inline Target T_Filter::Return (
    lang::Expression const & e=""
) 
```



Interface return clause Refers to the return subsignal at the given JSON Pointer 


        

<hr>



### function Return [2/2]

_Interface return clause with printf-like format._ 
```C++
template<class T, class... Rest>
inline Target T_Filter::Return (
    boost::json::string_view fmt,
    T && arg1,
    Rest &&... args_rest
) 
```




<hr>



### function ThreadId 

_Output capture thread id._ 
```C++
inline Target T_Filter::ThreadId () 
```




<hr>



### function Timestamp 

_Output capture timestamp._ 
```C++
inline Target T_Filter::Timestamp () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_transitions.hpp`

