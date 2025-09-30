/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <utility>

#include "zmbt/expr/lazy_param.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"



namespace zmbt {
namespace lang {


LazyParam::LazyParam(std::function<V()> getter)
    : getter_{std::move(getter)}
{
    if (!getter_)
    {
        getter_ = []{ return V{}; };
    }
}

LazyParam::LazyParam(ExpressionView v, EvalContext ctx)
    : getter_{[view = std::move(v), context = std::move(ctx)]
        { return view.eval_e({}, context).to_json(); }}
{

}

LazyParam::LazyParam(V && v)
    : getter_{[value = std::move(v)]{return value;}}
{
}

LazyParam::LazyParam(V const& v)
    : getter_{[v]{return v;}}
{
}

LazyParam::LazyParam(std::reference_wrapper<V const> v)
    : getter_{[v]{return v;}}
{
}


LazyParam::LazyParam()
    : LazyParam(std::function<V()>{})
{
}

LazyParam::V LazyParam::operator()() const
{
    if (!cache_)
    {
        cache_ = getter_();
    }
    return *cache_;
}

LazyParam::operator V() const
{
    return operator()();
}

} // namespace lang
} // namespace zmbt
