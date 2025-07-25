

# File environment\_data.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**environment\_data.hpp**](environment__data_8hpp.md)

[Go to the documentation of this file](environment__data_8hpp.md)


```C++

#ifndef ZMBT_MODEL_ENVIRONMENT_DATA_HPP_
#define ZMBT_MODEL_ENVIRONMENT_DATA_HPP_


#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <mutex>
#include <typeindex>
#include <type_traits>


#include "test_failure.hpp"
#include "trigger.hpp"
#include "generator.hpp"
#include "channel_kind.hpp"


namespace zmbt {




struct EnvironmentData {

    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;

    using shared_data_record = std::pair<std::type_index, std::shared_ptr<void>>;
    using shared_data_table = std::map<boost::json::string, shared_data_record>;
    using FailureHandler = std::function<void(boost::json::value const&)>;

    using GeneratorsTable = std::map<boost::json::string,    // jptr
        std::pair<Generator::Shared, lang::Expression>>;


    static boost::json::value init_json_data();

    shared_data_table shared;
    FailureHandler failure_handler {default_test_failure};
    JsonNode json_data {init_json_data()};

    std::map<boost::json::string, std::function<void()>> callbacks;
    std::map<boost::json::string, Trigger> triggers;
    std::map<interface_id, TriggerIfc> trigger_ifcs;
    std::map<object_id, TriggerObj> trigger_objs;
    std::map<object_id,                  // ifc
        std::map<interface_id,           // obj
        std::map<ChannelKind,            // grp
        GeneratorsTable>>> input_generators;


    mutex_t mutable mutex;

    EnvironmentData();

    EnvironmentData(EnvironmentData &&o);

    EnvironmentData& operator=(EnvironmentData &&o);

    EnvironmentData(EnvironmentData const& o);

    EnvironmentData& operator=(EnvironmentData const& o);

    virtual ~EnvironmentData();
};



}  // namespace zmbt

#endif  // ZMBT_MAPPING_ENVIRONMENT_DATA_HPP_
```


