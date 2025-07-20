/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 */

#include <limits>
#include <boost/math/constants/constants.hpp>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/api.hpp"
#include "zmbt/expr/eval_context.hpp"
#include "zmbt/expr/eval_impl.hpp"
#include "zmbt/expr/eval_impl_pp.hpp"


namespace zmbt {
namespace lang {


ZMBT_DEFINE_EVALUATE_IMPL(Near)
{
    // Based on numpy.isclose
    // absolute(a - b) <= (atol + rtol * absolute(b))
    // default rtol=1e-05, atol=1e-08
    constexpr double default_rtol = 1e-05;
    constexpr double default_atol = 1e-08;

    auto const x = lhs().eval();
    auto const param = rhs().eval();

    ASSERT(x.is_number(), "invalid argument")
    ASSERT(param.is_array() || param.is_number(), "invalid parameter")

    double x_value = boost::json::value_to<double>(curr_ctx().op.decorate(x));
    double ref_value = std::numeric_limits<double>::quiet_NaN();
    double rtol      = default_rtol;
    double atol      = default_atol;

    if (param.is_number())
    {
        ref_value = boost::json::value_to<double>(curr_ctx().op.decorate(param));
    }
    else
    {
        auto const& params = param.get_array();
        ASSERT(params.size() >= 1 && params.size() <= 3, "invalid parameter")

        if (params.size() >= 1)
        {
            ref_value = boost::json::value_to<double>(curr_ctx().op.decorate(params.at(0)));
        }
        if (params.size() >= 2)
        {
            rtol = boost::json::value_to<double>(curr_ctx().op.decorate(params.at(1)));
        }
        if (params.size() == 3)
        {
            atol = boost::json::value_to<double>(curr_ctx().op.decorate(params.at(2)));
        }
    }

    return std::abs(x_value - ref_value) <= (atol + rtol * std::abs(ref_value));
}

} // namespace lang
} // namespace zmbt

