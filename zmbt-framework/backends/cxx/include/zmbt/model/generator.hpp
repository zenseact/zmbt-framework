/**
 * @file
 * @copyright (c) Copyright 2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_GENERATOR_EXPR_HPP_
#define ZMBT_MODEL_GENERATOR_EXPR_HPP_


#include <boost/json.hpp>

#include "zmbt/expr.hpp"


namespace zmbt {

class Generator
{
    boost::json::array underlying_;
    boost::json::object cache_;
public:
    explicit Generator(boost::json::array const& serialized);
    explicit Generator(lang::Expression const& expr);
    boost::json::array underlying() const;
    bool is_noop() const;
    boost::json::value operator()();
    void reset();

    Generator(Generator&) = default;
    Generator(Generator const&) = default;
    Generator& operator=(Generator&) = default;
    Generator& operator=(Generator const&) = default;
    ~Generator() = default;

    using Shared = std::shared_ptr<Generator>;

};


}  // namespace zmbt

#endif  // ZMBT_MODEL_GENERATOR_EXPR_HPP_
