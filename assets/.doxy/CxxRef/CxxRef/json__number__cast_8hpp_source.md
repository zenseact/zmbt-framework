

# File json\_number\_cast.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**json\_number\_cast.hpp**](json__number__cast_8hpp.md)

[Go to the documentation of this file](json__number__cast_8hpp.md)


```C++

#ifndef ZMBT_CORE_JSON_NUMBER_CAST_HPP_
#define ZMBT_CORE_JSON_NUMBER_CAST_HPP_

#include <boost/json.hpp>

namespace zmbt
{

boost::json::value real_to_number(double value);

boost::json::value real_to_number(std::int64_t value);

boost::json::value maybe_real_to_number(boost::json::value const& value);

bool maybe_real_to_number_inplace(boost::json::value& value);


} // namespace zmbt

#endif
```


