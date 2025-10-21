

# File exceptions.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**exceptions.hpp**](core_2exceptions_8hpp.md)

[Go to the documentation of this file](core_2exceptions_8hpp.md)


```C++

#ifndef ZMBT_CORE_EXCEPTIONS_HPP_
#define ZMBT_CORE_EXCEPTIONS_HPP_


#include <boost/throw_exception.hpp>

#include "format_string.hpp"
#include "type_info.hpp"

namespace zmbt {

struct base_error : public std::runtime_error {
    using std::runtime_error::runtime_error;

    template <class... A>
    base_error(boost::json::string_view fmtstr, A&&... args) : std::runtime_error(format(fmtstr, std::forward<A>(args)...))
    {
    }
};


struct serialization_error : public base_error {
    using base_error::base_error;
};

struct environment_error : public base_error {
    using base_error::base_error;
};



namespace detail
{
    void log_exception(char const* type, char const* what);
}

template <class E>
void throw_exception(E&& e)
{
    // auto const dynamic_exception_type = boost::typeindex::type_id_runtime(e).pretty_name();
    detail::log_exception(type_name(e).c_str(), e.what());
    boost::throw_exception(std::forward<E>(e));
}

}  // namespace zmbt

#endif
```


