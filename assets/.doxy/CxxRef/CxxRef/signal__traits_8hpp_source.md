

# File signal\_traits.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**signal\_traits.hpp**](signal__traits_8hpp.md)

[Go to the documentation of this file](signal__traits_8hpp.md)


```C++

#ifndef ZMBT_REFLECT_SIGNAL_TRAITS_HPP_
#define ZMBT_REFLECT_SIGNAL_TRAITS_HPP_

#include <functional>
#include <type_traits>

#include "zmbt/core.hpp"

// TODO: move to common header
#define ZMBT_PP_UNPACK(...) __VA_ARGS__

#define ZMBT_DEFINE_CUSTOM_INIT(Type, Args) \
template<> struct zmbt::reflect::custom_signal_traits<Type> { \
    static Type init() { return Type(ZMBT_PP_UNPACK Args); } \
};

namespace zmbt {
namespace reflect {

template <class T, class E = void>
struct signal_traits;


template <class T, class E = void>
struct custom_signal_traits;

namespace detail {


template <class T, class E = void>
struct default_signal_traits;

template<class T>
using default_signal_traits_t = decltype(default_signal_traits<T>::init());

template<class T>
using has_default_signal_traits = mp_valid<default_signal_traits_t, T>;

template<class T>
using custom_signal_traits_t = decltype(custom_signal_traits<T>::init());

template<class T>
using has_custom_signal_traits = mp_valid<custom_signal_traits_t, T>;


template <class T>
using enable_default_signal_traits = mp_if<mp_and<has_default_signal_traits<T>, mp_not<has_custom_signal_traits<T>>>, void>;

template <class T>
using enable_custom_signal_traits = mp_if<has_custom_signal_traits<T>, void>;

template <class T>
using enable_initialization_trap = first_if_none_t<void, has_custom_signal_traits<T>, has_default_signal_traits<T>>;


template <class T>
struct default_signal_traits<T, first_if_t<void, is_default_constructible<T>>>
{
    static constexpr T
    init ()
    {
        return T{};
    }
};


template <template <class...> class C, class... T>
struct default_signal_traits< C<T...>, first_if_none_t<void, is_default_constructible<C<T...>>>>
{
    static C<T...>
    init() {
        return C<T...> {signal_traits<T>::init()...};
    }
};



template <class T, std::size_t N>
struct array_initializer {
    using tuple_form = boost::mp11::mp_repeat_c<std::tuple<T>, N>;
    std::array<T, N> value;

    array_initializer() : array_initializer(
        signal_traits<tuple_form>::init(),
        std::make_index_sequence<N>()) {}

    private:
    template <std::size_t... I>
    array_initializer (tuple_form tuple, std::index_sequence<I...>)
    : value{std::get<I>(tuple)...} {}
};


template <class T, std::size_t N>
struct default_signal_traits<std::array<T, N>, first_if_none_t<void, is_default_constructible<std::array<T, N>>>>
{
    static std::array<T, N>
    init() {
        return detail::array_initializer<T, N>().value;
    }
};


} // ns detail


template <class T>
struct signal_traits<T, detail::enable_default_signal_traits<T>> : detail::default_signal_traits<T> {};

template <class T>
struct signal_traits<T, detail::enable_custom_signal_traits<T>> : custom_signal_traits<T> {};


template <class T>
struct signal_traits<T, detail::enable_initialization_trap<T>>
{
    static constexpr T
    init()
    {
        static_assert(sizeof(T) & 0, R"(
            Type T has no defined zmbt::reflect::signal_traits specialization.
            To resolve this error, specialize custom_signal_traits for T
            with the static function init() -> T)");
        return *static_cast<T*>(nullptr);
    }
};


template<> struct signal_traits<void>
{
    static void
    init() {}
};

namespace detail
{
template <class T>
struct init_tuple_impl;

template <class... T>
struct init_tuple_impl<std::tuple<T...>>
{
    static constexpr std::tuple<T...> init()
    {
        return {signal_traits<T>::init()...};
    }
};
} // namespace detail

template <class T>
constexpr T init_tuple()
{
    return detail::init_tuple_impl<T>::init();
}

template <class T>
constexpr T init_value()
{
    return signal_traits<T>::init();
}


// deprecated - use signal_traits instead
template <class T, class E = void>
using initialization = signal_traits<T, E>;

// deprecated - use custom_signal_traits instead
template <class T, class E = void>
using custom_initialization = custom_signal_traits<T, E>;

} // namespace reflect
} // namespace zmbt

#endif // ZMBT_REFLECT_SIGNAL_TRAITS_HPP_
```


