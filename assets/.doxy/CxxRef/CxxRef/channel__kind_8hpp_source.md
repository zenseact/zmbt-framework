

# File channel\_kind.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**channel\_kind.hpp**](channel__kind_8hpp.md)

[Go to the documentation of this file](channel__kind_8hpp.md)


```C++

#ifndef ZMBT_MODEL_CHANNEL_KIND_HPP_
#define ZMBT_MODEL_CHANNEL_KIND_HPP_


#include "zmbt/reflect.hpp"

namespace zmbt {

enum class ChannelKind : unsigned
{
    // Injectable and observable channels

    Args, Return, Exception,

    // Observable-only channels

    Timestamp, ThreadId, CallCount,
    Undefined
};



template<>
struct reflect::custom_serialization<ChannelKind> {

    static boost::json::value
    json_from(ChannelKind const& k)
    {
        switch(k)
        {
            case ChannelKind::Args:       return "args";
            case ChannelKind::Return:     return "return";
            case ChannelKind::CallCount:  return "call_count";
            case ChannelKind::Exception:  return "exception";
            case ChannelKind::Timestamp:  return "ts";
            case ChannelKind::ThreadId:   return "tid";
            default:
                return nullptr;
        }
    }

    static ChannelKind
    dejsonize(boost::json::value const& v)
    {
        if("args"       == v) return ChannelKind::Args;
        if("return"     == v) return ChannelKind::Return;
        if("call_count" == v) return ChannelKind::CallCount;
        if("exception"  == v) return ChannelKind::Exception;
        if("ts"         == v) return ChannelKind::Timestamp;
        if("tid"        == v) return ChannelKind::ThreadId;
        return ChannelKind::Undefined;
    }
};

}  // namespace zmbt

#endif  // ZMBT_MODEL_CHANNEL_KIND_HPP_
```


