#define ZMBT_DEFINE_EVALUATE_IMPL(K)                                            \
template <> struct EvalImpl<Keyword::K>                                         \
    : public EvalImplBase<Keyword::K>                                           \
{                                                                               \
    using EvalImplBase<Keyword::K>::EvalImplBase;                               \
    using EvalImplBase<Keyword::K>::operator();                                 \
    Expression impl(EvalContext const& context) const;                          \
};                                                                              \
                                                                                \
template <> Expression dispatch_eval<Keyword::K>(                               \
    Expression const& self, Expression const& x, EvalContext const& context)    \
{                                                                               \
    return EvalImpl<Keyword::K>(self, x)(context);                              \
}                                                                               \
                                                                                \
Expression EvalImpl<Keyword::K>::impl(EvalContext const& context) const         \
