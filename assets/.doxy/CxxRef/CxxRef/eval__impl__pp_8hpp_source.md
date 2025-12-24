

# File eval\_impl\_pp.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_impl\_pp.hpp**](eval__impl__pp_8hpp.md)

[Go to the documentation of this file](eval__impl__pp_8hpp.md)


```C++

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
```


