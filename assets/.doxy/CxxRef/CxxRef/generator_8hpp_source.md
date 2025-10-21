

# File generator.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**generator.hpp**](generator_8hpp.md)

[Go to the documentation of this file](generator_8hpp.md)


```C++

#ifndef ZMBT_MODEL_GENERATOR_EXPR_HPP_
#define ZMBT_MODEL_GENERATOR_EXPR_HPP_

#include <atomic>

#include <boost/json.hpp>

#include "zmbt/expr.hpp"


namespace zmbt {

// Index family expression wrapper with bound atomic counter
class Generator
{
    std::atomic_uint64_t counter_;
    lang::Expression expr_;
public:
    explicit Generator(boost::json::array const& serialized);
    explicit Generator(lang::Expression const& expr);

    bool is_noop() const;

    std::uint64_t operator()(boost::json::value& value);

    void reset();

    lang::Expression const& expression() const;


    Generator& operator=(Generator const&) = default;
    Generator(Generator const&) = default;
    Generator(Generator &&) = default;
    Generator& operator=(Generator&&) = default;
    ~Generator() = default;

    using Shared = std::shared_ptr<Generator>;

};


}  // namespace zmbt

#endif  // ZMBT_MODEL_GENERATOR_EXPR_HPP_
```


