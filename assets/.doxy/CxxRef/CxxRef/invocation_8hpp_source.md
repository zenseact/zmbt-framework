

# File invocation.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**invocation.hpp**](invocation_8hpp.md)

[Go to the documentation of this file](invocation_8hpp.md)


```C++

#ifndef ZMBT_REFLECT_INTERFACE_HPP_
#define ZMBT_REFLECT_INTERFACE_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "zmbt/core.hpp"


#define DEFAULT_INVOCATION_TYPEDEFS  \
using host_t   = ifc_host_t<type>;   \
using return_t = ifc_return_t<type>; \
using args_t   = ifc_args_t<type>;


namespace zmbt {
namespace reflect {


template <class T, class E = void>
struct invocation;


template <class T, class E = void>
struct custom_invocation;

namespace detail {


template<class T>
using custom_invocation_t = typename custom_invocation<T>::type;

template<class T>
using has_custom_invocation = mp_valid<custom_invocation_t, T>;

template <class Interface>
using enable_default_invocation = first_if_t<void,
    is_ifc_handle<Interface>,
    mp_not<has_custom_invocation<ifc_pointer_t<Interface>>>
>;


template <class Interface>
using enable_custom_invocation = first_if_t<void, has_custom_invocation<ifc_pointer_t<Interface>>>;

} // namespace detail

template <class S, class E = void>
struct default_invocation;



template <class S>
struct default_invocation<S, first_if_t<void, ifc_is_member_handle<S>>>
{
    using type = S;
    DEFAULT_INVOCATION_TYPEDEFS

    template <class H>
    static return_t apply(H&& object, type ifc, args_t args)
    {
        return boost::mp11::tuple_apply(std::mem_fn(ifc), std::tuple_cat(std::forward_as_tuple(object), args));
    }
};

template <class S>
struct default_invocation<S, first_if_any_t<void, ifc_is_fn_handle<S>, ifc_is_functor_ref<S>>>
{
    using type = S;
    DEFAULT_INVOCATION_TYPEDEFS

    static return_t apply(host_t, type ifc, args_t args)
    {
        static_assert(std::is_same<host_t, nullptr_t>::value, "");
        return boost::mp11::tuple_apply(ifc, args);
    }
};

template <class S>
struct default_invocation<S, first_if_t<void, ifc_is_functor_ptr<S>>>
{
    using type = S;
    DEFAULT_INVOCATION_TYPEDEFS

    static return_t apply(host_t, type ifc, args_t args)
    {
        static_assert(std::is_same<host_t, nullptr_t>::value, "");
        return boost::mp11::tuple_apply(*ifc, args);
    }
};




template <class S>
struct invocation<S, detail::enable_default_invocation<S>> : default_invocation<ifc_pointer_t<S>> {};

template <class S>
struct invocation<S, detail::enable_custom_invocation<S>> : custom_invocation<ifc_pointer_t<S>> {};

template<class T, class E = void>
struct has_invocation_for : std::false_type { };

template<class T>
struct has_invocation_for<T, void_t<typename invocation<T>::type>> : std::true_type { };


template <class Interface>
using interface_t = typename reflect::invocation<Interface>::type;


template <class Interface>
using invocation_args_t = typename invocation<Interface>::args_t;

template <class Interface>
using invocation_args_unqf_t = tuple_unqf_t<invocation_args_t<Interface>>;

template <class Interface>
using invocation_has_void_args = std::integral_constant<bool, 0 == boost::mp11::mp_size<invocation_args_t<Interface>>::value >;


template <class Interface>
using invocation_ret_t = typename invocation<Interface>::return_t;

template <class Interface>
using invocation_ret_unqf_t = remove_cvref_t<invocation_ret_t<Interface>>;

template <class Interface>
using invocation_has_void_ret = std::is_void<invocation_ret_t<Interface>>;


template <class Interface>
using invocation_ret_unqf_or_nullptr_t = mp_if<invocation_has_void_ret<Interface>, nullptr_t, invocation_ret_unqf_t<Interface>>;




template <class T>
using invocation_host_t = typename invocation<T>::host_t;

template <class T>
using invocation_unqf_host_t = remove_cvref_t<invocation_host_t<T>>;

}  // namespace reflect
}  // namespace zmbt


#undef DEFAULT_INVOCATION_TYPEDEFS
#endif  // ZMBT_REFLECT_INTERFACE_PLUGIN_DEFAULT_HPP_
```


