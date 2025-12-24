

# File file\_parser.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**file\_parser.hpp**](file__parser_8hpp.md)

[Go to the documentation of this file](file__parser_8hpp.md)


```C++

#ifndef ZMBT_CORE_FILE_PARSER_HPP_
#define ZMBT_CORE_FILE_PARSER_HPP_

#include <array>
#include <type_traits>

#include "aliases.hpp"

namespace zmbt {


enum class FileFormat
{
    DeduceFromExtension,
    JSON,
    YAML,
    XML
};


}  // namespace zmbt

#endif
```


