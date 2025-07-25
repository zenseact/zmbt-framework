

# File keyword\_info.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**keyword\_info.hpp**](keyword__info_8hpp.md)

[Go to the documentation of this file](keyword__info_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_INFO_HPP_
#define ZMBT_EXPR_KEYWORD_INFO_HPP_

#include "keyword.hpp"
#include "attributes.hpp"


namespace zmbt {
namespace lang {

template <Keyword k>
struct KeywordInfo
{
    constexpr Keyword keyword() { return k; }
    constexpr std::uint32_t attributes() { constexpr std::uint32_t a = ::zmbt::lang::attributes(k); return a; }
    static boost::json::string_view keyword_to_str() { return ::zmbt::lang::keyword_to_str(k); }
};

}  // namespace lang
}  // namespace zmbt

#endif
```


