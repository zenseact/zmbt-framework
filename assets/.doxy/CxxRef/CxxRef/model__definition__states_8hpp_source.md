

# File model\_definition\_states.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**model\_definition\_states.hpp**](model__definition__states_8hpp.md)

[Go to the documentation of this file](model__definition__states_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_MODEL_DEFINITION_STATES_HPP_
#define ZMBT_MAPPING_MODEL_DEFINITION_STATES_HPP_

#include "zmbt/mapping/model_definition.hpp"
#include "zmbt/mapping/model_definition_node.hpp"
#include "zmbt/mapping/model_definition_transitions.hpp"

namespace zmbt {
namespace mapping {

class ModelDefinition::N_Descr 
    : public ModelDefinition::N_Term
    , public ModelDefinition::T_Description<ModelDefinition::N_Term>
{
  private:
    friend class ModelDefinition;
    N_Descr(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Descr(N_Descr const&) = delete;
    N_Descr(N_Descr&&) = default;

  public:
    N_Descr() : N_Descr(detail::DefinitionHelper{}) {}
    virtual ~N_Descr()
    {
    }
};

class ModelDefinition::N_Post 
    : public ModelDefinition::N_Descr
    , public ModelDefinition::T_PostRun<ModelDefinition::N_Descr>
{
  private:
    friend class ModelDefinition;
    N_Post(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Post(N_Post const&) = delete;
    N_Post(N_Post&&) = default;

  public:
    N_Post() : N_Post(detail::DefinitionHelper{}) {}
    virtual ~N_Post()
    {
    }
};

class ModelDefinition::N_Pre 
    : public ModelDefinition::N_Post
    , public ModelDefinition::T_PreRun<ModelDefinition::N_Post>
{
  private:
    friend class ModelDefinition;
    N_Pre(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Pre(N_Pre const&) = delete;
    N_Pre(N_Pre&&) = default;

  public:
    N_Pre() : N_Pre(detail::DefinitionHelper{}) {}
    virtual ~N_Pre()
    {
    }
};

class ModelDefinition::N_Param 
    : public ModelDefinition::N_Pre
    , public ModelDefinition::T_Param<ModelDefinition::N_ParamTable>
    , public ModelDefinition::T_CustomParam<ModelDefinition::N_Param>
{
  private:
    friend class ModelDefinition;
    N_Param(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Param(N_Param const&) = delete;
    N_Param(N_Param&&) = default;

  public:
    N_Param() : N_Param(detail::DefinitionHelper{}) {}
    virtual ~N_Param()
    {
    }
};

class ModelDefinition::N_ParamTable 
    : public ModelDefinition::N_Param
    , public ModelDefinition::T_ParamRow<ModelDefinition::N_ParamTable>
{
  private:
    friend class ModelDefinition;
    N_ParamTable(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_ParamTable(N_ParamTable const&) = delete;
    N_ParamTable(N_ParamTable&&) = default;

  public:
    N_ParamTable() : N_ParamTable(detail::DefinitionHelper{}) {}
    virtual ~N_ParamTable()
    {
    }
};

class ModelDefinition::N_Test 
    : public ModelDefinition::N_Param
    , public ModelDefinition::T_Test<ModelDefinition::N_TestTable>
{
  private:
    friend class ModelDefinition;
    N_Test(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Test(N_Test const&) = delete;
    N_Test(N_Test&&) = default;

  public:
    N_Test() : N_Test(detail::DefinitionHelper{}) {}
    virtual ~N_Test()
    {
    }
};

class ModelDefinition::N_TestTable 
    : public ModelDefinition::N_Param
    , public ModelDefinition::T_TestRow<ModelDefinition::N_TestTable>
{
  private:
    friend class ModelDefinition;
    N_TestTable(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_TestTable(N_TestTable const&) = delete;
    N_TestTable(N_TestTable&&) = default;

  public:
    N_TestTable() : N_TestTable(detail::DefinitionHelper{}) {}
    virtual ~N_TestTable()
    {
    }
};

class ModelDefinition::N_Channel 
    : public ModelDefinition::T_At<ModelDefinition::N_Filter>
{
  private:
    friend class ModelDefinition;
    N_Channel(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Channel(N_Channel const&) = delete;
    N_Channel(N_Channel&&) = default;

  public:
    N_Channel() : N_Channel(detail::DefinitionHelper{}) {}
    virtual ~N_Channel()
    {
    }
};

class ModelDefinition::N_MaybeChannel 
    : public ModelDefinition::N_Test
    , public ModelDefinition::T_At<ModelDefinition::N_Filter>
{
  private:
    friend class ModelDefinition;
    N_MaybeChannel(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_MaybeChannel(N_MaybeChannel const&) = delete;
    N_MaybeChannel(N_MaybeChannel&&) = default;

  public:
    N_MaybeChannel() : N_MaybeChannel(detail::DefinitionHelper{}) {}
    virtual ~N_MaybeChannel()
    {
    }
};

class ModelDefinition::N_PipeId 
    : public ModelDefinition::N_MaybeChannel
    , public ModelDefinition::T_PipeId<ModelDefinition::N_MaybeChannel>
{
  private:
    friend class ModelDefinition;
    N_PipeId(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_PipeId(N_PipeId const&) = delete;
    N_PipeId(N_PipeId&&) = default;

  public:
    N_PipeId() : N_PipeId(detail::DefinitionHelper{}) {}
    virtual ~N_PipeId()
    {
    }
};

class ModelDefinition::N_ChannelEnd 
    : public ModelDefinition::T_TerminatePipe<ModelDefinition::N_PipeId>
    , public ModelDefinition::T_ContinuePipe<ModelDefinition::N_Channel>
{
  private:
    friend class ModelDefinition;
    N_ChannelEnd(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_ChannelEnd(N_ChannelEnd const&) = delete;
    N_ChannelEnd(N_ChannelEnd&&) = default;

  public:
    N_ChannelEnd() : N_ChannelEnd(detail::DefinitionHelper{}) {}
    virtual ~N_ChannelEnd()
    {
    }
};

class ModelDefinition::N_Decor 
    : public ModelDefinition::N_ChannelEnd
    , public ModelDefinition::T_As<ModelDefinition::N_ChannelEnd>
{
  private:
    friend class ModelDefinition;
    N_Decor(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Decor(N_Decor const&) = delete;
    N_Decor(N_Decor&&) = default;

  public:
    N_Decor() : N_Decor(detail::DefinitionHelper{}) {}
    virtual ~N_Decor()
    {
    }
};

class ModelDefinition::N_CnlTag 
    : public ModelDefinition::N_Decor
    , public ModelDefinition::T_Tag<ModelDefinition::N_Decor>
{
  private:
    friend class ModelDefinition;
    N_CnlTag(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_CnlTag(N_CnlTag const&) = delete;
    N_CnlTag(N_CnlTag&&) = default;

  public:
    N_CnlTag() : N_CnlTag(detail::DefinitionHelper{}) {}
    virtual ~N_CnlTag()
    {
    }
};

class ModelDefinition::N_Take 
    : public ModelDefinition::N_CnlTag
    , public ModelDefinition::T_Take<ModelDefinition::N_CnlTag>
{
  private:
    friend class ModelDefinition;
    N_Take(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Take(N_Take const&) = delete;
    N_Take(N_Take&&) = default;

  public:
    N_Take() : N_Take(detail::DefinitionHelper{}) {}
    virtual ~N_Take()
    {
    }
};

class ModelDefinition::N_Filter 
    : public ModelDefinition::N_Take
    , public ModelDefinition::T_Filter<ModelDefinition::N_Take>
{
  private:
    friend class ModelDefinition;
    N_Filter(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Filter(N_Filter const&) = delete;
    N_Filter(N_Filter&&) = default;

  public:
    N_Filter() : N_Filter(detail::DefinitionHelper{}) {}
    virtual ~N_Filter()
    {
    }
};

class ModelDefinition::N_Repeat 
    : public ModelDefinition::N_MaybeChannel
    , public ModelDefinition::T_Repeat<ModelDefinition::N_MaybeChannel>
{
  private:
    friend class ModelDefinition;
    N_Repeat(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Repeat(N_Repeat const&) = delete;
    N_Repeat(N_Repeat&&) = default;

  public:
    N_Repeat() : N_Repeat(detail::DefinitionHelper{}) {}
    virtual ~N_Repeat()
    {
    }
};

class ModelDefinition::N_Main 
    : public ModelDefinition::T_OnTrigger<ModelDefinition::N_Repeat>
{
  private:
    friend class ModelDefinition;
    N_Main(detail::DefinitionHelper&& m) : ModelDefinition::BaseTransition(std::move(m))
    {
    }
    N_Main(N_Main const&) = delete;
    N_Main(N_Main&&) = default;

  public:
    N_Main() : N_Main(detail::DefinitionHelper{}) {}
    virtual ~N_Main()
    {
    }
};


extern template class ModelDefinition::T_Description<ModelDefinition::N_Term>;
extern template class ModelDefinition::T_PostRun<ModelDefinition::N_Descr>;
extern template class ModelDefinition::T_PreRun<ModelDefinition::N_Post>;
extern template class ModelDefinition::T_Param<ModelDefinition::N_ParamTable>;
extern template class ModelDefinition::T_CustomParam<ModelDefinition::N_Param>;
extern template class ModelDefinition::T_ParamRow<ModelDefinition::N_ParamTable>;
extern template class ModelDefinition::T_Test<ModelDefinition::N_TestTable>;
extern template class ModelDefinition::T_TestRow<ModelDefinition::N_TestTable>;
extern template class ModelDefinition::T_At<ModelDefinition::N_Filter>;
extern template class ModelDefinition::T_PipeId<ModelDefinition::N_MaybeChannel>;
extern template class ModelDefinition::T_TerminatePipe<ModelDefinition::N_PipeId>;
extern template class ModelDefinition::T_ContinuePipe<ModelDefinition::N_Channel>;
extern template class ModelDefinition::T_As<ModelDefinition::N_ChannelEnd>;
extern template class ModelDefinition::T_Tag<ModelDefinition::N_Decor>;
extern template class ModelDefinition::T_Take<ModelDefinition::N_CnlTag>;
extern template class ModelDefinition::T_Filter<ModelDefinition::N_Take>;
extern template class ModelDefinition::T_Repeat<ModelDefinition::N_MaybeChannel>;
extern template class ModelDefinition::T_OnTrigger<ModelDefinition::N_Repeat>;

}  // namespace mapping
}  // namespace zmbt

#endif
```


