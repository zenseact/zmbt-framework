

# File preprocessor.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**preprocessor.hpp**](preprocessor_8hpp.md)

[Go to the documentation of this file](preprocessor_8hpp.md)


```C++

#ifndef ZMBT_CORE_PREPROCESSOR_HPP_
#define ZMBT_CORE_PREPROCESSOR_HPP_

#include <boost/preprocessor.hpp> // IWYU pragma: export

#define ZMBT_STRINGIFY(x) ZMBT_STRINGIFY_IMPL(x)
#define ZMBT_STRINGIFY_IMPL(x) #x
#define ZMBT_CUR_LOC __FILE__ "#" ZMBT_STRINGIFY(__LINE__)

#define ZMBT_PP_NS_OPEN(s, state, ns) namespace ns {
#define ZMBT_PP_NS_CLOSE(s, state, ns) }
#define ZMBT_PP_NS_OPEN_ALL(...) BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_OPEN, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define ZMBT_PP_NS_CLOSE_ALL(...) BOOST_PP_SEQ_FOR_EACH(ZMBT_PP_NS_CLOSE, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define ZMBT_PP_INJECT_CODE_INTO_NS(code, ...) \
ZMBT_PP_NS_OPEN_ALL(__VA_ARGS__)               \
code                                            \
ZMBT_PP_NS_CLOSE_ALL(__VA_ARGS__)


#endif // ZMBT_CORE_PREPROCESSOR_HPP_
```


