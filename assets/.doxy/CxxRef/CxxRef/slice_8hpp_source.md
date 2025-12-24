

# File slice.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**slice.hpp**](slice_8hpp.md)

[Go to the documentation of this file](slice_8hpp.md)


```C++

#ifndef ZMBT_MODEL_SLICE_ARRAY_HPP_
#define ZMBT_MODEL_SLICE_ARRAY_HPP_

#include <array>
#include <functional>
#include "aliases.hpp"

namespace zmbt {

boost::json::array slice(boost::json::array const& src, boost::json::string_view jp, std::int64_t const start, std::int64_t const stop, std::int64_t const step);

boost::json::array slice(boost::json::array const& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step);

boost::json::string slice(boost::json::string_view const src, std::int64_t const start, std::int64_t const stop, std::int64_t const step);

using js_array_slice_gen = std::function<boost::json::array::iterator()>;
using js_array_slice_const_gen = std::function<boost::json::array::const_iterator()>;
using js_string_slice_gen = std::function<boost::json::string_view::const_iterator()>;

js_array_slice_gen             make_slice_generator(boost::json::array& src           , std::int64_t const start, std::int64_t const stop, std::int64_t const step);
js_array_slice_const_gen make_slice_const_generator(boost::json::array const& src     , std::int64_t const start, std::int64_t const stop, std::int64_t const step);
js_string_slice_gen      make_slice_const_generator(boost::json::string_view const src, std::int64_t const start, std::int64_t const stop, std::int64_t const step);


namespace detail
{
    std::array<std::int64_t, 3> str_to_slice_idx(boost::json::string_view slice_expr);
} // namespace detail
} // namespace zmbt

#endif
```


