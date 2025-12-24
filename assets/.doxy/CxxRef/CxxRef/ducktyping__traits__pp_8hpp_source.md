

# File ducktyping\_traits\_pp.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**reflect**](dir_44621b39643a5ee7797a55bb572a295f.md) **>** [**ducktyping\_traits\_pp.hpp**](ducktyping__traits__pp_8hpp.md)

[Go to the documentation of this file](ducktyping__traits__pp_8hpp.md)


```C++

#ifndef ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_
#define ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_

#define CAT(a, b) a##b

#define ZMBT_TRAIT_HAS_TYPE(trait, name)     \
    template <class T, class = void>         \
    struct trait : std::false_type {};       \
    template <class T>                       \
    struct trait<                            \
        T, zmbt::void_t<typename T::name>>   \
    : std::true_type {};


#define ZMBT_TRAIT_HAS_MEMBER(trait, name)           \
    template <class T, class = void>                 \
    struct trait : std::false_type {};               \
    template <class T>                               \
    struct trait<T,                                  \
    zmbt::void_t<decltype(std::declval<T>().name())>>\
    : std::true_type {};


#define ZMBT_HAS_TYPE(name) ZMBT_TRAIT_HAS_TYPE(CAT(has_type_, name), name)
#define ZMBT_HAS_MEMBER(name) ZMBT_TRAIT_HAS_MEMBER(CAT(has_member_, name), name)




#endif // ZMBT_REFLECT_DUCKTYPING_TRAITS_MACROS_HPP_
```


