

# File model\_definition\_node.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**model\_definition\_node.hpp**](model__definition__node_8hpp.md)

[Go to the documentation of this file](model__definition__node_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_NODE_HPP_

#include "zmbt/mapping/model_definition.hpp"


namespace zmbt {
namespace mapping {


class ModelDefinition::BaseTransition
{
  detail::DefinitionHelper mapping_;

  public:
    virtual ~BaseTransition() { };

  protected:

    BaseTransition(detail::DefinitionHelper && m) : mapping_{m} {}
    BaseTransition() : BaseTransition(detail::DefinitionHelper{}) {}
    BaseTransition(BaseTransition const&) = delete;
    BaseTransition(BaseTransition&&) = default;

    detail::DefinitionHelper& state()
    {
        return mapping_;
    }

    template <class Target>
    Target transit_to()
    {
        return Target(std::move(mapping_));
    }
};


class ModelDefinition::N_Term : protected virtual ModelDefinition::BaseTransition
{
  private:
    friend class ModelDefinition;
    N_Term(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Term(N_Term const&) = delete;
    N_Term(N_Term&&) = default;

  public:
  N_Term() : N_Term(detail::DefinitionHelper{}) {}
    virtual ~N_Term()
    {
    }
};

}  // namespace mapping
}  // namespace zmbt

#endif
```


