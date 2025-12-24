

# File channel\_handle.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**channel\_handle.hpp**](channel__handle_8hpp.md)

[Go to the documentation of this file](channel__handle_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_CHANNEL_HANDLE_HPP_
#define ZMBT_MAPPING_CHANNEL_HANDLE_HPP_

#include <list>
#include <boost/json.hpp>

#include "zmbt/core.hpp"
#include "zmbt/model/environment.hpp"
#include "zmbt/expr.hpp"

namespace zmbt {
namespace mapping {


class ChannelHandle
{
    JsonNode data_;
    Environment env;

public:
    using ConditionPipe = std::list<ChannelHandle>;
    using Kind = ChannelKind;

    ~ChannelHandle() = default;
    ChannelHandle(JsonNode& model, boost::json::string_view cnl_ptr);

    boost::json::string key() const;

    object_id host() const;
    interface_id interface() const;
    boost::json::string full_path() const;
    boost::json::string signal_path() const;
    Kind kind() const;

    bool operator==(boost::json::value const& v) const;

    operator boost::json::value() const
    {
        return data_.node();
    }

    std::size_t index() const;
    boost::json::value tag() const;

    boost::json::array captures() const;

    lang::Expression transform() const;

    Environment::InterfaceHandle inerface_handle() const;

};



} // namespace mapping
} // namespace zmbt

#endif
```


