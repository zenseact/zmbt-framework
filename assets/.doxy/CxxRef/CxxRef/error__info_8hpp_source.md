

# File error\_info.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**error\_info.hpp**](error__info_8hpp.md)

[Go to the documentation of this file](error__info_8hpp.md)


```C++

#ifndef ZMBT_MODEL_ERROR_INFO_HPP_
#define ZMBT_MODEL_ERROR_INFO_HPP_

#include <boost/json.hpp>


namespace zmbt
{

struct ErrorInfo
{
    boost::json::string type;
    boost::json::string what;
    boost::json::string context;

    boost::json::value to_json() const
    {
        return boost::json::object {
            {"type"   , type   },
            {"what"   , what   },
            {"context", context},
        };
    }
};

} // namespace zmbt

#endif
```


