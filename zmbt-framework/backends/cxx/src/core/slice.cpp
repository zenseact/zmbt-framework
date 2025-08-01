/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <iostream>


#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>

#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>

#include "zmbt/core/exceptions.hpp"
#include "zmbt/core/slice.hpp"


namespace
{


/// Unsafe slice params: start, stop, step.
using slice_t = std::tuple<std::int64_t, std::int64_t, std::int64_t>;
/// Safe slice stop condition
using slice_stop_cond_t = std::function<bool(std::size_t const)>;
/// Safe slice params: unsigned start, signed step, stop condition fn, slice size.
using safe_slice_t = std::tuple<std::size_t, std::int64_t, slice_stop_cond_t, std::size_t>;

/// get safe unsigned slice from signed slice and container size N
safe_slice_t get_safe_slice(std::size_t const N, slice_t slice)
{
    std::int64_t const start {std::get<0>(slice)};
    std::int64_t const stop {std::get<1>(slice)};
    std::int64_t const step {std::get<2>(slice)};

    if (step == 0)
    {
        throw zmbt::base_error("slice step cannot be zero");
    }

    safe_slice_t dummy {0U,1,[](std::size_t const){return false;}, 0};

    bool const start_is_negat = start < 0;
    bool const stop_is_negat = stop < 0;
    bool const start_in_range = start_is_negat ? (static_cast<std::size_t>(-start) < N) : (static_cast<std::size_t>(start) < N);
    bool const stop_in_range = stop_is_negat ? (static_cast<std::size_t>(-stop) < N) : (static_cast<std::size_t>(stop) < N);
    std::int64_t const step_sign = (step < 0) ? -1 : 1;

    // keep unsafe signed to detect span
    std::int64_t const start_unsafe = start_is_negat ? (N + start) : start;
    std::int64_t const stop_unsafe = stop_is_negat ? (N + stop) : stop;

    std::size_t const start_safe = start_is_negat ? 0U : N - 1;
    std::size_t const stop_safe  = stop_is_negat  ? 0U : N - 1;

    std::size_t const ustart = start_in_range ? static_cast<std::size_t>(start_unsafe) : start_safe;
    std::size_t const ustop = stop_in_range ? static_cast<std::size_t>(stop_unsafe) : stop_safe;

    std::int64_t const sN {static_cast<std::int64_t>(N)};
    bool const span_too_high = (start_unsafe > sN) && (stop_unsafe > sN);
    bool const span_too_low = (start_unsafe < 0) && (stop_unsafe < 0);
    bool const span_is_negative = ((stop_unsafe - start_unsafe) * step_sign) < 0;
    if (span_too_low || span_too_high || span_is_negative)
    {
        return dummy;
    }
    bool const moving_forward = ustop >= ustart;
    // the behavior is similar to python slice (but taken with exclusive stop +1):
    // for x = [0,1,2], x[0:1:1] -> [0], but x[0:1:-1] -> []
    bool const zero_step_wrong_dir = moving_forward ? (step < 0) : (step > 0);
    if (zero_step_wrong_dir)
    {
        return dummy;
    }
    auto const stop_condition = [N, moving_forward, ustop](std::size_t const idx){
        if (idx > (N-1)) return false; // overflow with negative step
        return moving_forward ? (idx <= ustop) : (idx >= ustop);
    };
    std::size_t size = (moving_forward ? ustop - ustart : ustart - ustop) / (step_sign * step) + 1;
    return {ustart, step, stop_condition, size};

}


template <class F, class T>
F make_slice_generator_impl(T begin, T end, safe_slice_t slice)
{
    auto const safe_start = std::get<0>(slice);
    auto const safe_step = std::get<1>(slice);
    auto const it_cond = std::get<2>(slice);

    return [=]{
        std::size_t idx {safe_start};
        return [=]() mutable {
            auto it = begin;
            if (it_cond(idx))
            {
                it += idx;
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
    boost::json::error_code dummy_ec;
    boost::json::array output {};
    auto element = src.cend();
    while ((element = gen()) != src.cend())
    {
        boost::json::value const* subelement = element->find_pointer(jp, dummy_ec);
        if (!subelement)
        {
            throw base_error("slice error: %s not found at %s", jp, *element);
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
    std::array<std::int64_t, 3> out {0,-1,1};
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
        throw zmbt::base_error("invalid str_to_slice_idx(%s)", slice_expr);
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
        throw zmbt::base_error("invalid str_to_slice_idx(%s): %s", slice_expr, e.what());
    }

    return out;
}
} // namespace detail

} // namespace zmbt

