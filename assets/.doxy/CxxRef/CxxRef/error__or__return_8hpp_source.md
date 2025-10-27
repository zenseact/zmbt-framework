

# File error\_or\_return.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**error\_or\_return.hpp**](error__or__return_8hpp.md)

[Go to the documentation of this file](error__or__return_8hpp.md)


```C++

#ifndef ZMBT_MODEL_ERROR_OR_RETURN_HPP_
#define ZMBT_MODEL_ERROR_OR_RETURN_HPP_

#include <boost/json.hpp>
#include <boost/variant2.hpp>
#include <boost/optional.hpp>

#include "error_info.hpp"


namespace zmbt
{

template <class Return>
class ErrorOr
{
    using variant_t = boost::variant2::variant<ErrorInfo, Return>;

    variant_t error_or_return;

  public:
    ErrorOr() = default;

    static ErrorOr<Return> MakeError(ErrorInfo err)
    {
        using boost::variant2::in_place_index;
        ErrorOr<Return> e{};
        e.error_or_return = variant_t{in_place_index<0>, err};
        return e;
    }

    static ErrorOr<Return> MakeValue(Return ret)
    {
        using boost::variant2::in_place_index;
        ErrorOr<Return> e{};
        e.error_or_return = variant_t{in_place_index<1>, ret};
        return e;
    }

    ErrorInfo const& as_error() const
    {
        return boost::variant2::get<0>(error_or_return);
    }

    Return const& as_return() const
    {
        return boost::variant2::get<1>(error_or_return);
    }

    bool is_null() const
    {
        return (error_or_return.index() == 0) && as_error().type.empty();
    }

    bool is_error() const
    {
        return (error_or_return.index() == 0) && !as_error().type.empty();
    }

    bool is_return() const
    {
        return (error_or_return.index() == 1);
    }

    void dump_to(boost::json::object& obj) const
    {
        if (is_error())
        {
            obj["exception"] = boost::variant2::get<0>(error_or_return).to_json();
        }
        else if (is_return())
        {
            obj["return"] = json_from(boost::variant2::get<1>(error_or_return));
        }
        else
        {
            // noop
        }
    }
};



} // namespace zmbt

#endif
```


