/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_SERIALIZATION_DEFAULTS_HPP_
#define ZMBT_CORE_SERIALIZATION_DEFAULTS_HPP_

#include <functional>
#include <type_traits>

#include <boost/describe.hpp>
#include <boost/json.hpp>

#include "serialization.hpp"

#include <complex>



namespace zmbt {
namespace reflect {
namespace detail {

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
                    throw_exception(serialization_error("narrowing enum conversion, can't represent " + std::to_string(value)));
                }
                t = static_cast<T>(value);
            }
            else {

                std::uint64_t value = v.as_uint64();
                if (value > std::numeric_limits<underlying_t>::max()) {
                    throw_exception(serialization_error("narrowing enum conversion, can't represent " + std::to_string(value)));
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
                throw_exception(serialization_error("invalid enum value: %s", value));
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
            obj[descr.name] = zmbt::json_from(t.*descr.pointer);
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
            t.*descr.pointer = zmbt::dejsonize<TT>(obj.at(descr.name));
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


//////////////////////////////
// STD TYPES
//////////////////////////////


template <class T>
struct default_serialization<std::complex<T>>
{

    static boost::json::value json_from(std::complex<T> const& t)
    {

        auto const real = real_to_number(t.real());
        auto const imag = real_to_number(t.imag());
        return t.imag() ? boost::json::array{real, imag} : real;
    }

    static std::complex<T> dejsonize(boost::json::value const& v)
    {
        T real, imag;
        if (v.is_array() && v.get_array().size() == 2)
        {
            real = boost::json::value_to<T>(v.get_array().at(0));
            imag = boost::json::value_to<T>(v.get_array().at(1));
        }
        else if (v.is_number())
        {
            real = boost::json::value_to<T>(v);
            imag = 0;
        }
        else
        {
            throw_exception(serialization_error("can't produce std::complex from `%s`", v));
        }
        return {real, imag};
    }
};

} // namespace detail
} // namespace reflect
} // namespace zmbt


#endif // ZMBT_CORE_SERIALIZATION_DEFAULTS_HPP_
