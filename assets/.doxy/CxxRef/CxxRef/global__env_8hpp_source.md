

# File global\_env.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**global\_env.hpp**](global__env_8hpp.md)

[Go to the documentation of this file](global__env_8hpp.md)


```C++

#ifndef ZMBT_EXPR_GLOBAL_ENV_HPP_
#define ZMBT_EXPR_GLOBAL_ENV_HPP_

#include <memory>
#include <ostream>
#include <boost/json.hpp>

namespace zmbt {
namespace lang {


class GlobalEnv
{
    class Impl;
    std::shared_ptr<Impl> impl_;
public:

    GlobalEnv();

    GlobalEnv(GlobalEnv const&) = default;
    GlobalEnv(GlobalEnv&&) = default;
    GlobalEnv& operator=(GlobalEnv const&) = default;
    GlobalEnv& operator=(GlobalEnv&&) = default;

    ~GlobalEnv();

    // Store global variable at given JSON Pointer and return value if success, otherwise return error expression
    boost::json::value Store(boost::json::string_view const json_pointer, boost::json::value const& value) noexcept;

    // Load global variable if exist, otherwise return null
    boost::json::value Load(boost::json::string_view const json_pointer) const noexcept;

    // Reset global env state
    void Reset() noexcept;

    friend std::ostream& operator<<(std::ostream& os, GlobalEnv const& env);

};

}  // namespace lang
}  // namespace zmbt


#endif
```


