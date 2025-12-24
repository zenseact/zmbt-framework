

# File global\_flags.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**global\_flags.hpp**](global__flags_8hpp.md)

[Go to the documentation of this file](global__flags_8hpp.md)


```C++

#ifndef ZMBT_MODEL_GLOBAL_FLAGS_HPP_
#define ZMBT_MODEL_GLOBAL_FLAGS_HPP_

#include <atomic>
#include <chrono>
#include <cstdint>

namespace zmbt
{
namespace flags
{

class TestIsRunning
{
  public:
    static void set() noexcept
    {
        flag().store(true, std::memory_order_release);
    }

    static void clear() noexcept
    {
        flag().store(false, std::memory_order_release);
    }

    static bool status() noexcept
    {
        return flag().load(std::memory_order_acquire);
    }

    static bool exchange(bool const sts) noexcept
    {
        return flag().exchange(sts, std::memory_order_acquire);
    }

private:
    static std::atomic<bool>& flag() noexcept;
};

} // namespace flags
} // namespace zmbt

#endif
```


