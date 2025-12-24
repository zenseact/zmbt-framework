

# File precise\_real.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**decor**](dir_23db209f70f961198fdaea8adff9f278.md) **>** [**precise\_real.hpp**](precise__real_8hpp.md)

[Go to the documentation of this file](precise__real_8hpp.md)


```C++

#ifndef ZMBT_DECORATOR_PRECISE_REAL_HPP_
#define ZMBT_DECORATOR_PRECISE_REAL_HPP_

#include <cstdlib>
#include <cmath>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"


namespace zmbt {

namespace decor {
struct precision_loss_error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


namespace detail {



template<class T>
T strto (boost::json::string_view, boost::json::string_view::pointer*);

template<>
inline float strto<float>(boost::json::string_view str, boost::json::string_view::pointer* end)
{
    return std::strtof(str.cbegin(), end);
}

template<>
inline double strto<double>(boost::json::string_view str, boost::json::string_view::pointer* end)
{
    return std::strtod(str.cbegin(), end);
}
template<>
inline long double strto<long double>(boost::json::string_view str, boost::json::string_view::pointer* end)
{
    return std::strtold(str.cbegin(), end);
}

} // namespace detail


template <class VT>
struct precise
{
    // TODO: document the decoration API
    using decorated_type = VT;

    static_assert(std::is_floating_point<decorated_type>::value, "template paremeter is not a floating number type");


    ~precise() = default;

    precise() : value_ {0} {}
    precise(precise const&) = default;
    precise(precise &&) = default;
    precise& operator=(precise const&) = default;
    precise& operator=(precise &&) = default;


  private:

    template <class T>
    static decorated_type validate(T init_value)
    {
        decorated_type value = static_cast<decorated_type>(init_value);
        if (std::isnormal(init_value) and (T {value} !=  init_value)) {
            throw_exception(precision_loss_error("precision loss when creating zmbt::decor::precise<T>"));
        }
        return value;
    }

    template <class T>
    static decorated_type validate_str_as(boost::json::string_view str)
    {
        char* end {nullptr};

        auto const value_as_t = detail::strto<T>(str, &end);

        if (end == str.cbegin() ) {
            throw_exception(std::invalid_argument("zmbt::decor::precise<T> string parsing error"));
        }

        // test reverse
        if (std::isnormal(value_as_t) and (value_as_t != detail::strto<decorated_type>(str, nullptr))) {
            throw_exception(precision_loss_error("precision loss when creating zmbt::decor::precise<T>"));
        }

        return validate(value_as_t);
    }


    static decorated_type validate_str(boost::json::string_view str)
    {
        // hex notation shall be precise
        if (0 == (std::strncmp(str.cbegin(), "0x", 2) & std::strncmp(str.cbegin(), "0X", 2)))
        {
            return validate_str_as<long double>(str);
        }
        else if (auto const N = str.size())
        {
            switch (str[N-1])
            {
                case 'f':
                case 'F':
                    return validate_str_as<float>(str);
                    break;
                case 'l':
                case 'L':
                    return validate_str_as<long double>(str);
                    break;
                default:
                    return validate_str_as<double>(str);
                    break;
            }
        }
        else {
            throw_exception(std::invalid_argument("zmbt::decor::precise<T> string parsing error"));
            return {};
        }
    }

  public:

    explicit precise(decorated_type v) : value_{v} {}

    template<class T, class = std::enable_if_t<std::is_arithmetic<T>::value>>
    precise(T const v) : value_{validate(v)}
    {
    }

    template <class T , class = std::enable_if_t<std::is_arithmetic<T>::value>>
    precise& operator=(T const v)
    {
        value_ = validate(v);
        return *this;
    }


    template<class T>
    precise(precise<T> other) : value_{validate(other.value())}
    {
    }

    template <class T>
    precise& operator=(precise<T> other)
    {
        value_ = validate(other.value());
        return *this;
    }

    precise(boost::json::string_view str) : value_{validate_str(str)}
    {
    }

    precise& operator=(boost::json::string_view str)
    {
        value_ = validate_str(str);
        return *this;
    }

    decorated_type value() const
    {
        return value_;
    }

    bool operator<(precise other) const
    {
        return value() < other.value();
    }

    bool operator<(boost::json::string_view str) const
    {
        return value() < validate_str(str);
    }

    bool operator==(precise other) const
    {
        return value() == other.value();
    }

    bool operator==(boost::json::string_view str) const
    {
        return value() == validate_str(str);
    }


    operator decorated_type() const
    {
        return value();
    }

    std::string stringify() const
    {
        char buff[256] {};
        std::snprintf(buff, 256, "%a", value());
        return std::string(buff);
    }

  private:
    decorated_type value_;
};

template <class T>
constexpr zmbt::type_tag<precise<T>> Precise;

} // namespace decor

namespace reflect {

template <class T>
struct custom_serialization<decor::precise<T>> {


    static boost::json::value json_from(decor::precise<T> const t)
    {
        return t.stringify().c_str();
    }

    static decor::precise<T>
    dejsonize(boost::json::value const& v)
    {
        decor::precise<T> result;
        switch (v.kind()) {
            case boost::json::kind::string:
                result = v.get_string().data();
                break;
            case boost::json::kind::double_:
                result = v.get_double();
                break;
            case boost::json::kind::int64:
                result = v.get_int64();
                break;
            case boost::json::kind::uint64:
                result = v.get_uint64();
                break;
            default:
                throw_exception(std::invalid_argument("zmbt::decor::precise<T> conversion failure"));
                break;
        }
        return result;
    }

};


} // namespace reflect
} // namespace zmbt

#endif  // ZMBT_MAPPING_PRECISE_REAL_HPP_
```


