

# File global\_stats.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**global\_stats.hpp**](global__stats_8hpp.md)

[Go to the documentation of this file](global__stats_8hpp.md)


```C++

#ifndef ZMBT_MODEL_GLOBAL_STATS_HPP_
#define ZMBT_MODEL_GLOBAL_STATS_HPP_

#include <atomic>
#include <chrono>
#include <cstdint>

namespace zmbt
{
namespace flags
{

class InjectionTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept;
};

class RecordingTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept;
};

class ConversionTime
{
  public:
    static void add(std::chrono::nanoseconds const duration) noexcept
    {
        counter().fetch_add(static_cast<std::uint64_t>(duration.count()), std::memory_order_relaxed);
    }

    static std::uint64_t value() noexcept
    {
        return counter().load(std::memory_order_relaxed);
    }

    static void reset() noexcept
    {
        counter().store(0, std::memory_order_relaxed);
    }

  private:
    static std::atomic<std::uint64_t>& counter() noexcept;
};

} // namespace flags
} // namespace zmbt

#endif
```


