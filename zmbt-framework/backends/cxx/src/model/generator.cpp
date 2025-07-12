/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include "zmbt/model/generator.hpp"


namespace zmbt {

Generator::Generator(lang::Expression const& expr) : counter_{0}, expr_{expr} {}

std::uint64_t Generator::operator()(boost::json::value& value)
{
    auto const iteration = counter_++;
    value = expr_.eval(iteration);
    return iteration;
}

void Generator::reset()
{
    counter_ = 0;
}

bool Generator::is_noop() const
{
    return expr_.is_noop();
}

lang::Expression const& Generator::expression() const
{
    return expr_;
}



}  // namespace zmbt
