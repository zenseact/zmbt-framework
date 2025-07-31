/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/expr/lazy_param.hpp"
#include "zmbt/expr/expression.hpp"
#include "zmbt/expr/eval_context.hpp"



namespace zmbt {
namespace lang {


LazyParam::LazyParam(std::function<V()> getter)
    : getter_{getter}
{
}

LazyParam::LazyParam(ExpressionView v, EvalContext ctx)
    : getter_{[v, ctx]{ return v.eval_e({}, ctx).to_json(); }}
{

}

LazyParam::LazyParam(V && v)
    : getter_{[v=std::move(v)]{return v;}}
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
    : LazyParam(nullptr)
{
}

LazyParam::V LazyParam::operator()() const
{
    return getter_();
}

LazyParam::operator V() const
{
    return getter_();
}

} // namespace lang
} // namespace zmbt
