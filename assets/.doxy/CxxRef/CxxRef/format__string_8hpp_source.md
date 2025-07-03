

# File format\_string.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**format\_string.hpp**](format__string_8hpp.md)

[Go to the documentation of this file](format__string_8hpp.md)


```C++

#ifndef ZMBT_CORE_FORMAT_STRING_HPP_
#define ZMBT_CORE_FORMAT_STRING_HPP_


#include <boost/json.hpp>
#include <string>
#include <utility>

namespace zmbt {


namespace detail
{
    std::string format_impl(boost::json::string_view fmtstr, boost::json::array const& args);
}

template <class... A>
std::string format(boost::json::string_view fmtstr, A&&... arg)
{
    return detail::format_impl(fmtstr, {std::forward<A>(arg)...});
}



}  // namespace zmbt

#endif
```


