

# File test\_parameter\_resolver.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**test\_parameter\_resolver.hpp**](test__parameter__resolver_8hpp.md)

[Go to the documentation of this file](test__parameter__resolver_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_TEST_PARAMETER_RESOLVER_HPP_
#define ZMBT_MAPPING_TEST_PARAMETER_RESOLVER_HPP_

#include <zmbt/core/json_iter.hpp>
#include <zmbt/core/json_node.hpp>
#include <zmbt/model/environment.hpp>
#include <list>
#include <memory>

namespace zmbt {
namespace mapping {

class TestParameterResolver
{
    JsonNode model_;
    std::list<std::shared_ptr<JsonIter>> iterators_{};
    Environment env{};

    void log_debug(JsonNode next_model);
    void resolve_deferred_params(JsonNode& next_model);
    void init_param_iters();

public:

    TestParameterResolver(JsonNode const& model);
    JsonNode Next();
};


} // namespace mapping
} // namespace zmbt

#endif // ZMBT_MAPPING_RESOLVER_HPP_
```


