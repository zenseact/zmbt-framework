/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_SERIALIZATION_HPP_
#define ZMBT_CORE_SERIALIZATION_HPP_

#include <functional>
#include <type_traits>

#include <boost/describe.hpp>
#include <boost/json.hpp>

#include "zmbt/core/preprocessor.hpp" // IWYU pragma: export
#include "zmbt/core.hpp"
#include "ducktyping_traits_pp.hpp"
#include "signal_traits.hpp"


#define ZMBT_INJECT_JSON_TAG_INVOKE using zmbt::reflect::tag_invoke;
#define ZMBT_INJECT_OSTREAM_OPERATOR using zmbt::reflect::operator<<;
#define ZMBT_INJECT_SERIALIZATION ZMBT_INJECT_JSON_TAG_INVOKE ZMBT_INJECT_OSTREAM_OPERATOR


#define ZMBT_INJECT_JSON_TAG_INVOKE_INTO(...) ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_JSON_TAG_INVOKE, __VA_ARGS__)
#define ZMBT_INJECT_OSTREAM_OPERATOR_INTO(...) ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_OSTREAM_OPERATOR, __VA_ARGS__)
#define ZMBT_INJECT_SERIALIZATION_INTO(...) ZMBT_PP_INJECT_CODE_INTO_NS(ZMBT_INJECT_SERIALIZATION, __VA_ARGS__)



namespace zmbt {
namespace reflect {

/**
 * @brief Serialization metafunction
 *
 * @tparam T
 */
template <class T, class = void>
struct serialization;

/**
 * @brief User-defined serialization
 *
 * @tparam T
 */
template <class T, class = void>
struct custom_serialization;


namespace detail {


template <class T, class E = void>
struct default_serialization;

template<class T>
using default_serialization_t = decltype(default_serialization<T>::json_from(std::declval<T>()));

template<class T>
using has_default_serialization = mp_valid<default_serialization_t, T>;

template<class T>
using custom_serialization_t = decltype(custom_serialization<T>::json_from(std::declval<T>()));

template<class T>
using has_custom_serialization = mp_valid<custom_serialization_t, T>;


ZMBT_TRAIT_HAS_MEMBER(has_json_operator, operator boost::json::value)

template<class T>
using is_json_convertible = mp_and<
    is_constructible<T, boost::json::value>,
    has_json_operator<T>
>;


template <class T, class R = void>
using enable_for_default_serialization = first_if_t<R,
    mp_not<has_custom_serialization<T>>,
    has_default_serialization<T>
>;

template <class T, class R = void>
using enable_for_custom_serialization = first_if_t<R, has_custom_serialization<T>>;


template <class T, class R = void>
using enable_hermetic_serialization = first_if_any_t<R,
    has_custom_serialization<T>,
    has_default_serialization<T>
>;

template <class T, class R = void>
using disable_hermetic_serialization = first_if_none_t<R,
    has_custom_serialization<T>,
    has_default_serialization<T>
>;



////////////////////////////
// DESCRIBED ENUM SERIALIZER
////////////////////////////

template <class T>
struct default_serialization<T, first_if_t<void,
    boost::describe::has_describe_enumerators<T>
>>
{
    static boost::json::value json_from(T const& t) {
        boost::json::value v;
        using Descr = boost::describe::describe_enumerators<T>;
        using underlying_t = std::underlying_type_t<T>;

        // first assign numeric value if nominal representation does not exist.
        if (std::is_signed<underlying_t>::value) {
            v.emplace_int64() = static_cast<std::underlying_type_t<T>>(t);
        }
        else {
            v.emplace_uint64() = static_cast<std::underlying_type_t<T>>(t);
        }

        boost::mp11::mp_for_each<Descr>([&](auto descr) {
            if (t == descr.value) {
                v.emplace_string() = descr.name;
            }
        });

        return v;
    }

    static T dejsonize(boost::json::value const& v)
    {
        using Descr = boost::describe::describe_enumerators<T>;
        T t = reflect::signal_traits<T>::init();

        if (v.is_number()) {
            using underlying_t = std::underlying_type_t<T>;
            if (std::is_signed<underlying_t>::value) {

                std::int64_t value = v.as_int64();
                std::int64_t test_value = static_cast<std::int64_t>(static_cast<underlying_t>(value));
                if (value != test_value) {
                    throw serialization_error("narrowing enum conversion, can't represent " + std::to_string(value));
                }
                t = static_cast<T>(value);
            }
            else {

                std::uint64_t value = v.as_uint64();
                if (value > std::numeric_limits<underlying_t>::max()) {
                    throw serialization_error("narrowing enum conversion, can't represent " + std::to_string(value));
                }
                t = static_cast<T>(value);
            }
        }
        else if (v.is_string())
        {
            auto const &value = boost::json::string_view {v.as_string()};
            bool found = false;

            boost::mp11::mp_for_each<Descr>([&](auto descr) {
                if (not found and value == boost::json::string_view(descr.name)) {
                    t = T(descr.value);
                    found = true;
                }
            });

            if (not found) {
                throw serialization_error("invalid enum value: %s", value);
            }

        }

        return t;
    }
};


//////////////////////////////
// DESCRIBED STRUCT SERIALIZER
//////////////////////////////


template <class T>
struct default_serialization<T, first_if_t<void,
    boost::describe::has_describe_members<T>
>>
{

    static boost::json::value json_from(T const& t)
    {

        static_assert(not std::is_union<T>::value, "union types are not supported by default, provide a specialization");

        using Descr = boost::describe::describe_members<T, boost::describe::mod_public | boost::describe::mod_inherited | boost::describe::mod_protected | boost::describe::mod_private>;

        boost::json::value v;

        auto& obj = v.emplace_object();

        boost::mp11::mp_for_each<Descr>([&](auto descr) {
            obj[descr.name] = boost::json::value_from(t.*descr.pointer);
        });

        return v;
    }

    static T dejsonize(boost::json::value const& v)
    {

        static_assert(not std::is_union<T>::value, "union types are not supported by default, provide a specialization");

        using Descr = boost::describe::describe_members<T, boost::describe::mod_public | boost::describe::mod_inherited | boost::describe::mod_protected | boost::describe::mod_private>;

        auto const& obj = v.as_object();

        T t = reflect::signal_traits<T>::init();

        boost::mp11::mp_for_each<Descr>([&](auto descr) {

            using TT = std::remove_reference_t<decltype(t.*descr.pointer)>;
            t.*descr.pointer = boost::json::value_to<TT>(obj.at(descr.name));
        });

        return t;
    }
};


template <class T>
struct default_serialization<T, first_if_t<void,
    is_json_convertible<T>,
    mp_not<boost::describe::has_describe_members<T>>
>>
{

    static boost::json::value json_from(T const& t)
    {
        return t.operator boost::json::value();
    }

    static T dejsonize(boost::json::value const& v)
    {
        return T(v);
    }
};

} // namespace detail

template <class T>
struct serialization<T, detail::enable_for_default_serialization<T>> : detail::default_serialization<T> {};

template <class T>
struct serialization<T, detail::enable_for_custom_serialization<T>> : custom_serialization<T> {};

/// ADL injection wrapper enabling boost::json::value_from<T> conversion
template <class T>
detail::enable_hermetic_serialization<T, void>
tag_invoke(boost::json::value_from_tag const&, boost::json::value& v, T const& t)
{
    v = serialization<T>::json_from(t);
}

/// ADL injection wrapper enabling boost::json::value_from<T> conversion
template <class T>
detail::enable_hermetic_serialization<T, T>
tag_invoke(boost::json::value_to_tag<T> const&, boost::json::value const& v)
{
    return serialization<T>::dejsonize(v);
}

/// ADL injection wrapper enabling operator<<
template <class T>
detail::enable_hermetic_serialization<T, std::ostream&>
operator<< (std::ostream& os, T const& value)
{
    os << serialization<T>::json_from(value);
    return os;
}

} // namespace reflect

ZMBT_INJECT_SERIALIZATION


template<class T, class E = void>
struct has_serialization : std::false_type { };

template<class T>
struct has_serialization<T, void_t<typename reflect::serialization<T>::type>> : std::true_type { };


template <class T, class TT = remove_cvref_t<T>>
reflect::detail::disable_hermetic_serialization<TT, boost::json::value>
json_from(T&& t)
{
    return boost::json::value_from(std::forward<T>(t));
}

template <class T>
reflect::detail::disable_hermetic_serialization<T, T>
dejsonize(boost::json::value const& v)
{
    return boost::json::value_to<T>(v);
}

template <class T, class TT = remove_cvref_t<T>>
reflect::detail::enable_hermetic_serialization<TT, boost::json::value>
json_from(T&& t)
{
    return reflect::serialization<TT>::json_from(std::forward<T>(t));
}

template <class T>
reflect::detail::enable_hermetic_serialization<T, T>
dejsonize(boost::json::value const& v)
{
    return reflect::serialization<T>::dejsonize(v);
}

// Trivial conversions not handled by Boost JSON

inline boost::json::value json_from(std::tuple<>)
{
    return boost::json::array{};
}
template <>
inline std::tuple<> dejsonize<std::tuple<>>(boost::json::value const&)
{
    // TODO: check value
    return {};
}


template <> inline void dejsonize<void>(boost::json::value const&)
{
    // TODO: check value
    return;
}




template <class T, std::size_t N>
boost::json::value json_from_array(T const (&array)[N])
{

    boost::json::array out {};
    out.reserve(N);
    for (size_t i = 0; i < N; i++)
    {
        out.push_back(zmbt::json_from(array[i]));
    }
    return out;
}

template <class T, std::size_t N>
void dejsonize_array(boost::json::array const& jarr, T (&array)[N])
{

    if (jarr.size() != N)
    {
        throw serialization_error("JSON array size mismatch in deserialization of `%s[%d]`",
            zmbt::type_name<T>() , N);
    }
    auto jarr_it = jarr.cbegin();
    for (size_t i = 0; i < N; i++)
    {
        array[i] = zmbt::dejsonize<T>(*jarr_it++);
    }
}

} // namespace zmbt


#endif // ZMBT_CORE_SERIALIZATION_HPP_
