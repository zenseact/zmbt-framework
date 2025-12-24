/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#define ZMBT_DEFINE_EVALUATE_IMPL(K)                                            \
template <> struct EvalImpl<Keyword::K>                                         \
    : public EvalImplBase<Keyword::K>                                           \
{                                                                               \
    using EvalImplBase<Keyword::K>::EvalImplBase;                               \
    using EvalImplBase<Keyword::K>::operator();                                 \
    Expression impl() const;                                                    \
};                                                                              \
                                                                                \
template <> Expression dispatch_eval<Keyword::K>(                               \
    ExpressionView const& self, ExpressionView const& x, EvalContext context)   \
{                                                                               \
    return EvalImpl<Keyword::K>(self, x, context)();                            \
}                                                                               \
                                                                                \
Expression EvalImpl<Keyword::K>::impl() const                                   \


#define ASSERT(cond, msg) if (!(cond)) { return ::zmbt::lang::detail::make_error_expr(msg, keyword_to_str());}
