

# File parameter.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**parameter.hpp**](parameter_8hpp.md)

[Go to the documentation of this file](parameter_8hpp.md)


```C++

#ifndef ZMBT_CORE_PARAMETER_HPP_
#define ZMBT_CORE_PARAMETER_HPP_

#include <boost/json.hpp>

namespace zmbt {


class Param final
{
    boost::json::string value_;

    Param() = default;
public:


    static bool isParam(boost::json::value const& v);

    static Param parse(boost::json::value const& v);

    boost::json::value serialize() const
    {
        return value_;
    }

    Param(boost::json::value v) : Param{parse(v)} {}

    Param(Param const&) = default;
    Param(Param &&) = default;
    Param& operator=(Param const&) = default;
    Param& operator=(Param &&) = default;
    ~Param() = default;

    operator boost::json::value() const
    {
        return serialize();
    }

    boost::json::string to_string() const
    {
        return value_;
    }

};


}

#endif
```


