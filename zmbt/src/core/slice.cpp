/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/json.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>

#include "zmbt/core/exceptions.hpp"
#include "zmbt/core/slice.hpp"


namespace
{


/// Unsafe slice params: start, stop, step.
using slice_t = std::tuple<std::int64_t, std::int64_t, std::int64_t>;
/// Safe slice stop condition
using slice_stop_cond_t = std::function<bool(std::size_t const)>;
/// Safe slice params: unsigned start, signed step, stop condition fn.
using safe_slice_t = std::tuple<std::size_t, std::int64_t, slice_stop_cond_t>;

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

    safe_slice_t output {0U,1,[](std::size_t const){return false;}};

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
        return output;
    }
    bool const move_fwd = ustop >= ustart;
    // the behavior is similar to python slice (but taken with exclusive stop +1):
    // for x = [0,1,2], x[0:1:1] -> [0], but x[0:1:-1] -> []
    bool const zero_step_wrong_dir = move_fwd ? (step < 0) : (step > 0);
    if (zero_step_wrong_dir)
    {
        return output;
    }
    auto const stop_condition = [N, move_fwd, ustop](std::size_t const idx){
        if (idx > (N-1)) return false; // overflow with negative step
        return move_fwd ? (idx <= ustop) : (idx >= ustop);
    };
    return {ustart,step,stop_condition};

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



js_array_slice_const_gen make_slice_const_generator(boost::json::array const& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return make_slice_generator_impl<js_array_slice_const_gen>(src.cbegin(), src.cend(), get_safe_slice(src.size(), {start, stop, step}));
}

js_array_slice_gen make_slice_generator(boost::json::array& src, std::int64_t const start, std::int64_t const stop, std::int64_t const step)
{
    return make_slice_generator_impl<js_array_slice_gen>(src.begin(), src.end(), get_safe_slice(src.size(), {start, stop, step}));
}

}

