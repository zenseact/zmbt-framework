

# File json\_traverse.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**json\_traverse.hpp**](json__traverse_8hpp.md)

[Go to the documentation of this file](json__traverse_8hpp.md)


```C++

#ifndef ZMBT_CORE_JSON_TRAVERSE_HPP_
#define ZMBT_CORE_JSON_TRAVERSE_HPP_

#include <functional>
#include <string>
#include "boost/json.hpp"



namespace zmbt {



class JsonTraverse
{
    void traverse(boost::json::value const& v, std::string const jptr) const;
public:

    typedef std::function<bool(boost::json::value const&, std::string const)> visitor_fn;

    JsonTraverse(visitor_fn visit) : visit_{visit} {}

    void operator()(boost::json::value const& v) const
    {
        return traverse(v, "");
    }
private:
    visitor_fn const visit_;
};

}

#endif // ZMBT_CORE_JSON_TRAVERSE_HPP_
```


