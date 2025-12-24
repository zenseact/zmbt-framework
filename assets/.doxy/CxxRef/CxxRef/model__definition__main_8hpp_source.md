

# File model\_definition\_main.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**model\_definition\_main.hpp**](model__definition__main_8hpp.md)

[Go to the documentation of this file](model__definition__main_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_MAIN_HPP_

#include "zmbt/expr/api.hpp"
#include "zmbt/mapping/model_definition.hpp"
#include "zmbt/mapping/model_definition_states.hpp"
#include "zmbt/mapping/model_definition_transitions.hpp"


namespace zmbt {
namespace mapping {

class SignalMapping : public ModelDefinition::N_Main
{
  public:

    template <class... T>
    SignalMapping(boost::json::string_view name, T&&... args) : N_Main()
    {
        this->state().set_deferred_param("/name", name | expr::Fmt(args...));
    }

    ~SignalMapping()
    {
        this->state().execute();
    }
};


}  // namespace mapping
}  // namespace zmbt

#endif
```


