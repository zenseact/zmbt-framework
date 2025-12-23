/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <cstddef>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <tuple>
#include <limits>

#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>

#include "zmbt/core/exceptions.hpp"
#include "zmbt/core/slice.hpp"


namespace
{


/// Unsafe slice params: start, stop, step.
using slice_t = std::tuple<std::int64_t, std::int64_t, std::int64_t>;
/// Safe slice stop condition
using slice_stop_cond_t = std::function<bool(std::int64_t const)>;
/// Safe slice params: unsigned start, signed step, stop condition fn, slice size.
using safe_slice_t = std::tuple<std::size_t, std::int64_t, slice_stop_cond_t, std::size_t>;

/// get safe unsigned slice from signed slice and container size N
safe_slice_t get_safe_slice(std::size_t const N, slice_t slice)
{
    auto const start_raw = std::get<0>(slice);
    auto const stop_raw = std::get<1>(slice);
    auto const step = std::get<2>(slice);

    if (step == 0)
    {
        throw_exception(zmbt::base_error("slice step cannot be zero"));
    }

    constexpr std::int64_t none = std::numeric_limits<std::int64_t>::max();
    constexpr std::int64_t limit = std::numeric_limits<std::int64_t>::max();
    std::int64_t const length = std::min<std::int64_t>(static_cast<std::int64_t>(N), limit);

    safe_slice_t const empty_slice {0U, step, [](std::int64_t const){ return false; }, 0U};

    if (length <= 0)
    {
        return empty_slice;
    }

    if (step > 0)
    {
        std::int64_t start = (start_raw == none) ? 0 : start_raw;
        std::int64_t stop = (stop_raw == none) ? length : stop_raw;

        if (start < 0) start += length;
        if (start < 0) start = 0;
        if (start > length) start = length;

        if (stop < 0) stop += length;
        if (stop < 0) stop = 0;
        if (stop > length) stop = length;

        if (start >= stop)
        {
            return empty_slice;
        }

        auto const slice_size = static_cast<std::size_t>((stop - start + step - 1) / step);
        auto const stop_exclusive = stop;
        auto const stop_condition = [stop_exclusive, length](std::int64_t const idx){
            if (idx < 0 || idx >= length) return false;
            return idx < stop_exclusive;
        };

        return {
            static_cast<std::size_t>(start),
            step,
            stop_condition,
            slice_size
        };
    }
    else
    {
        std::int64_t start = (start_raw == none) ? (length - 1) : start_raw;
        std::int64_t stop = (stop_raw == none) ? -1 : stop_raw;

        if (start < 0) start += length;
        if (start < 0) start = -1;
        if (start >= length) start = length - 1;

        if (stop_raw != none && stop < 0) stop += length;
        if (stop < -1) stop = -1;
        if (stop >= length) stop = length - 1;

        auto const step_abs = -step;

        if ((start < 0) || (start <= stop))
        {
            return empty_slice;
        }

        auto const slice_size = static_cast<std::size_t>((start - stop - 1) / step_abs + 1);
        auto const stop_exclusive = stop;
        auto const stop_condition = [stop_exclusive, length](std::int64_t const idx){
            if (idx < 0 || idx >= length) return false;
            return idx > stop_exclusive;
        };

        return {
            static_cast<std::size_t>(start),
            step,
            stop_condition,
            slice_size
        };
    }

}


template <class F, class T>
F make_slice_generator_impl(T begin, T end, safe_slice_t slice)
{
    auto const safe_start = std::get<0>(slice);
    auto const safe_step = std::get<1>(slice);
    auto const it_cond = std::get<2>(slice);

    return [=]{
        std::int64_t idx {static_cast<std::int64_t>(safe_start)};
        return [=]() mutable {
            if (it_cond(idx))
            {
                auto it = begin;
                it += static_cast<std::size_t>(idx);
                idx += safe_step;
                return it;
            }
            else {
                return end;
            }
        };
    }();
}
} // namespace


namespace zmbt {



boost::json::array slice(boost::json::array const& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return slice(src, "", start, stop, step);
}

boost::json::array slice(boost::json::array const& src, boost::json::string_view jp, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    auto gen = make_slice_const_generator(src, start, stop, step);
    boost::system::error_code dummy_ec;
    boost::json::array output {};
    auto element = src.cend();
    while ((element = gen()) != src.cend())
    {
        boost::json::value const* subelement = element->find_pointer(jp, dummy_ec);
        if (!subelement)
        {
            throw_exception(base_error("slice error: %s not found at %s", jp, *element));
        }
        output.emplace_back(*subelement);
    }

    return output;
}

boost::json::string slice(boost::json::string_view const src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    auto gen = make_slice_const_generator(src, start, stop, step);

    auto const slice_size = std::get<3>(get_safe_slice(src.size(), {start, stop, step}));
    boost::json::string output(slice_size, '\n');
    auto element = src.cend();
    std::size_t i {0};
    while ((element = gen()) != src.cend())
    {
        output.at(i++) = *element;
    }

    return output;
}




js_array_slice_const_gen make_slice_const_generator(boost::json::array const& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return make_slice_generator_impl<js_array_slice_const_gen>(src.cbegin(), src.cend(), get_safe_slice(src.size(), {start, stop, step}));
}

js_array_slice_gen make_slice_generator(boost::json::array& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return make_slice_generator_impl<js_array_slice_gen>(src.begin(), src.end(), get_safe_slice(src.size(), {start, stop, step}));
}

js_string_slice_gen make_slice_const_generator(boost::json::string_view const src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return make_slice_generator_impl<js_string_slice_gen>(src.cbegin(), src.cend(), get_safe_slice(src.size(), {start, stop, step}));
}



namespace detail
{
std::array<std::int64_t, 3> str_to_slice_idx(boost::json::string_view slice_expr)
{
    std::array<std::int64_t, 3> out {std::numeric_limits<std::int64_t>::max(), std::numeric_limits<std::int64_t>::max(), 1};
    std::int64_t& start = out.at(0);
    std::int64_t& stop = out.at(1);
    std::int64_t& step = out.at(2);

    std::vector<boost::json::string> tokens;
    boost::split(
        tokens,
        slice_expr,
        boost::is_any_of(":")
    );

    if (tokens.size() < 2 || tokens.size() > 3)
    {
        throw_exception(zmbt::base_error("invalid slice: \"%s\"", slice_expr));
    }

    try
    {
        if (!tokens[0].empty())
        {
            start = boost::json::value_to<std::int64_t>(boost::json::parse(tokens[0]));
        }
        if (!tokens[1].empty())
        {
            stop = boost::json::value_to<std::int64_t>(boost::json::parse(tokens[1]));
        }
        if (3 == tokens.size() && !tokens[2].empty())
        {
            step = boost::json::value_to<std::int64_t>(boost::json::parse(tokens[2]));
        }
    }
    catch (std::exception const& e)
    {
        throw_exception(zmbt::base_error("invalid slice: \"%s\" , error: `%s`", slice_expr, e.what()));
    }

    return out;
}
} // namespace detail

} // namespace zmbt
