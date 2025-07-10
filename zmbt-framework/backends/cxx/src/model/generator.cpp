/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include "zmbt/model/generator.hpp"


namespace zmbt {

Generator::Generator(boost::json::array const& serialized)
    : underlying_{serialized} {}

Generator::Generator(lang::Expression const& expr) : underlying_{0UL, expr} {}

boost::json::value Generator::operator()()
{
    auto& counter = underlying_.at(0).as_uint64();
    auto const e = lang::Expression(underlying_.at(1));
    return e.eval(counter++);
}

void Generator::reset()
{
    underlying_.at(0).emplace_uint64();
}

boost::json::array Generator::underlying() const
{
    return underlying_;
}

bool Generator::is_noop() const
{
    return lang::Expression(underlying()).is_noop();
}

std::size_t Generator::counter() const
{
    return underlying_.at(0).as_uint64();
}


boost::json::value Generator::debug(std::size_t const counter, lang::Expression::EvalContext const& ctx) const
{
    auto const e = lang::Expression(underlying_.at(1));
    return e.eval(counter, ctx);
}

std::string Generator::prettify() const
{
    return expr::Compose(underlying_.at(1), underlying_.at(0)).prettify();
}

}  // namespace zmbt
