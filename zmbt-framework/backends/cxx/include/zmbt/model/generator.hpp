/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_GENERATOR_EXPR_HPP_
#define ZMBT_MODEL_GENERATOR_EXPR_HPP_

#include <atomic>

#include <boost/json.hpp>

#include "zmbt/expr.hpp"


namespace zmbt {

// Index family expression wrapper with bound atomic counter
class Generator
{
    std::atomic_uint64_t counter_;
    lang::Expression expr_;
public:
    explicit Generator(boost::json::array const& serialized);
    explicit Generator(lang::Expression const& expr);

    bool is_noop() const;

    /// Generate value and return iteration
    std::uint64_t operator()(boost::json::value& value);

    /// Reset atomic counter
    void reset();

    /// Get underlying expression
    lang::Expression const& expression() const;


    Generator& operator=(Generator const&) = default;
    Generator(Generator const&) = default;
    Generator(Generator &&) = default;
    Generator& operator=(Generator&&) = default;
    ~Generator() = default;

    using Shared = std::shared_ptr<Generator>;

};


}  // namespace zmbt

#endif  // ZMBT_MODEL_GENERATOR_EXPR_HPP_
