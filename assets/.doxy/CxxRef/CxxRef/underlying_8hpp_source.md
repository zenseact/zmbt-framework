

# File underlying.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**decor**](dir_23db209f70f961198fdaea8adff9f278.md) **>** [**underlying.hpp**](underlying_8hpp.md)

[Go to the documentation of this file](underlying_8hpp.md)


```C++

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
```


