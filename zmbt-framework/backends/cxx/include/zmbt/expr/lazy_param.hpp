/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZMBT_EXPR_LAZY_PARAM_HPP_
#define ZMBT_EXPR_LAZY_PARAM_HPP_

#include <functional>
#include <boost/json.hpp>

#include "zmbt/reflect.hpp"

namespace zmbt {
namespace lang {

class ExpressionView;
class EvalContext;

class LazyParam
{
  public:
    using V = boost::json::value;

    LazyParam();

    LazyParam(std::function<V()> getter);

    LazyParam(ExpressionView, EvalContext);

    LazyParam(V && v);

    LazyParam(V const& v);

    LazyParam(std::reference_wrapper<V const> v);

    template <class T>
    LazyParam(T const& v)
        : LazyParam(json_from(v))
    {
    }

    ~LazyParam() = default;

    LazyParam(LazyParam const&) = default;
    LazyParam(LazyParam &&) = default;
    LazyParam& operator=(LazyParam const&) = default;
    LazyParam& operator=(LazyParam &&) = default;

    V operator()() const;

    operator V() const;

  private:

    std::function<V()> getter_;
};

} // namespace lang
} // namespace zmbt

#endif
