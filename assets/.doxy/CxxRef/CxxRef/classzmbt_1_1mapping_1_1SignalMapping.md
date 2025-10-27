

# Class zmbt::mapping::SignalMapping



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**mapping**](namespacezmbt_1_1mapping.md) **>** [**SignalMapping**](classzmbt_1_1mapping_1_1SignalMapping.md)



_Signal Mapping Test Model._ [More...](#detailed-description)

* `#include <model_definition_main.hpp>`



Inherits the following classes: [zmbt::mapping::ModelDefinition::N\_Main](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SignalMapping**](#function-signalmapping) (boost::json::string\_view name, T &&... args) <br>_Begin the model definition._  |
|   | [**~SignalMapping**](#function-signalmapping) () <br> |


## Public Functions inherited from zmbt::mapping::ModelDefinition::N_Main

See [zmbt::mapping::ModelDefinition::N\_Main](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md)

| Type | Name |
| ---: | :--- |
|   | [**N\_Main**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md#function-n_main-44) () <br> |
| virtual  | [**~N\_Main**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md#function-n_main) () <br> |






















































## Detailed Description




**See also:** [Signal Mapping model](/user-guide/signal-mapping/) 



    
## Public Functions Documentation




### function SignalMapping 

_Begin the model definition._ 
```C++
template<class... T>
inline zmbt::mapping::SignalMapping::SignalMapping (
    boost::json::string_view name,
    T &&... args
) 
```





**Template parameters:**


* `T` 



**Parameters:**


* `name` model name or format pattern string 
* `args` format arguments 




        

<hr>



### function ~SignalMapping 

```C++
inline zmbt::mapping::SignalMapping::~SignalMapping () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/mapping/model_definition_main.hpp`

