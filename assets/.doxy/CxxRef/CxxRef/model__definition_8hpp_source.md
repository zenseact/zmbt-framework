

# File model\_definition.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**model\_definition.hpp**](model__definition_8hpp.md)

[Go to the documentation of this file](model__definition_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_HPP_

#include "model_definition_helper.hpp"



namespace zmbt {
namespace mapping {

class ModelDefinition
{


    class BaseTransition;

    friend class SignalMapping;

    struct N_Term;

    // Transition templates
    template <class Target>
    struct T_As;
    template <class Target>
    struct T_At;
    template <class Target>
    struct T_ContinuePipe;
    template <class Target>
    struct T_CustomParam;
    template <class Target>
    struct T_Description;
    template <class Target>
    struct T_Filter;
    template <class Target>
    struct T_OnTrigger;
    template <class Target>
    struct T_Param;
    template <class Target>
    struct T_ParamRow;
    template <class Target>
    struct T_PipeId;
    template <class Target>
    struct T_PostRun;
    template <class Target>
    struct T_PreRun;
    template <class Target>
    struct T_Repeat;
    template <class Target>
    struct T_Tag;
    template <class Target>
    struct T_Take;
    template <class Target>
    struct T_TerminatePipe;
    template <class Target>
    struct T_Test;
    template <class Target>
    struct T_TestRow;

// NODES forward declarations
    class N_Descr;
    class N_Post;
    class N_Pre;
    class N_Param;
    class N_ParamTable;
    class N_Test;
    class N_TestTable;
    class N_Channel;
    class N_MaybeChannel;
    class N_PipeId;
    class N_ChannelEnd;
    class N_Decor;
    class N_CnlTag;
    class N_Take;
    class N_Filter;
    class N_Repeat;
    class N_Main;

};


}  // namespace mapping
}  // namespace zmbt

#endif
```


