

# File permanent\_data.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**permanent\_data.hpp**](permanent__data_8hpp.md)

[Go to the documentation of this file](permanent__data_8hpp.md)


```C++

#ifndef ZMBT_MODEL_PERMANENT_DATA_HPP_
#define ZMBT_MODEL_PERMANENT_DATA_HPP_

#include <functional>
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK
#include <boost/unordered/concurrent_flat_map.hpp>
#include <boost/optional.hpp>



#include "zmbt/core.hpp"
#include "zmbt/reflect/prototypes.hpp"
#include "output_recorder.hpp"


namespace zmbt {

struct PermanentEnvData final
{
    using SetupOutputRecorder = std::function<void(OutputRecorder&)>;

    boost::concurrent_flat_map<interface_id, object_id> default_objects{};
    boost::concurrent_flat_map<interface_id, reflect::Prototypes> prototypes{};
    boost::concurrent_flat_map<interface_id, SetupOutputRecorder> output_recorder_factories{};

    boost::optional<object_id>             get_default_object(interface_id const&) const;
    boost::optional<reflect::Prototypes>   get_prototypes(interface_id const&) const;
    boost::optional<SetupOutputRecorder>   get_output_recorder_factory(interface_id const&) const;
};

} // namespace zmbt

#endif
```


