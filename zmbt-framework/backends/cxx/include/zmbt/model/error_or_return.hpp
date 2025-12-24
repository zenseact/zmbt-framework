/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

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
