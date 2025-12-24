/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_DECORATOR_UNDERLYING_HPP_
#define ZMBT_DECORATOR_UNDERLYING_HPP_

#include <type_traits>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"


namespace zmbt {

namespace decor {



template <class E>
struct underlying
{
    static_assert(std::is_enum<E>::value, "");
    using decorated_type = std::underlying_type_t<E>;


    ~underlying() = default;
    underlying() = default;
    underlying(underlying const&) = default;
    underlying(underlying &&) = default;
    underlying& operator=(underlying const&) = default;
    underlying& operator=(underlying &&) = default;


    underlying(decorated_type const v) : value_{v} {}
    underlying& operator=(decorated_type const v) { value_ = v; return *this; }

    underlying(E const v) : value_{static_cast<decorated_type>(v)} {}
    underlying& operator=(E const v) { value_ = static_cast<decorated_type>(v); return *this; }


    underlying(boost::json::string_view v) : value_{static_cast<decorated_type>(dejsonize<E>(v))} {}
    underlying& operator=(boost::json::string_view v) { value_ = static_cast<decorated_type>(dejsonize<E>(v)); return *this; }


    decorated_type value() const
    {
        return value_;
    }

    std::string stringify() const
    {
        return json_from(static_cast<E>(value())).as_string().c_str();
    }

    operator decorated_type() const
    {
        return value();
    }

    auto operator!()  const { return !value(); }
    auto operator~()  const { return ~value(); }
    auto operator-()  const { return -value(); }

    template <class T> auto operator==(T const& other) const { return value() == other; }
    template <class T> auto operator!=(T const& other) const { return value() != other; }
    template <class T> auto operator>>(T const& other) const { return value() >> other; }
    template <class T> auto operator<<(T const& other) const { return value() << other; }
    template <class T> auto operator<=(T const& other) const { return value() <= other; }
    template <class T> auto operator>=(T const& other) const { return value() >= other; }
    template <class T> auto operator<(T const& other)  const { return value() <  other; }
    template <class T> auto operator>(T const& other)  const { return value() >  other; }
    template <class T> auto operator+(T const& other)  const { return value() +  other; }
    template <class T> auto operator-(T const& other)  const { return value() -  other; }
    template <class T> auto operator*(T const& other)  const { return value() *  other; }
    template <class T> auto operator/(T const& other)  const { return value() /  other; }
    template <class T> auto operator%(T const& other)  const { return value() %  other; }
    template <class T> auto operator&(T const& other)  const { return value() &  other; }
    template <class T> auto operator|(T const& other)  const { return value() |  other; }
    template <class T> auto operator^(T const& other)  const { return value() ^  other; }
    template <class T> auto operator&&(T const& other) const { return value() && other; }
    template <class T> auto operator||(T const& other) const { return value() || other; }

private:
    decorated_type value_;
};


/// @brief underlying<T> decorator
/// @tparam T
template <class T>
constexpr zmbt::type_tag<underlying<T>> Underlying;

} // namespace decor

namespace reflect {

template <class T>
struct custom_serialization<decor::underlying<T>> {


    static boost::json::value json_from(decor::underlying<T> const t)
    {
        boost::json::value v;
        v.emplace_string() = t.stringify();
        return v;
    }

    static decor::underlying<T>
    dejsonize(boost::json::value const& v)
    {
        decor::underlying<T> result;
        switch (v.kind()) {
            case boost::json::kind::string:
                result = v.get_string().data();
                break;
            case boost::json::kind::int64:
                result = v.get_int64();
                break;
            case boost::json::kind::uint64:
                result = v.get_uint64();
                break;
            default:
                throw_exception(zmbt::serialization_error("can't construct zmbt::decor::underlying<T> from `%s`", v));
                break;
        }
        return result;
    }

};

} // namespace reflect
} // namespace zmbt

#endif  // ZMBT_DECORATOR_UNDERLYING_HPP_
