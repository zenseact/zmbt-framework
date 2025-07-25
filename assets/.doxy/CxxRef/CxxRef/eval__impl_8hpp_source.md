

# File eval\_impl.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_impl.hpp**](eval__impl_8hpp.md)

[Go to the documentation of this file](eval__impl_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EVAL_IMPL_HPP_
#define ZMBT_EXPR_EVAL_IMPL_HPP_


#include "expression.hpp"
#include "eval_context.hpp"
#include "eval_params.hpp"
#include "eval_validator.hpp"
#include "keyword.hpp"
#include "keyword_info.hpp"

namespace zmbt {
namespace lang {

template <Keyword K>
struct EvalImpl; // specializations shall use CRTP : public EvalImplBase<K>
// {
    // Expression impl() const;
// };


template <Keyword K, class Validator = EvalValidator<K>>
class EvalImplBase : public FixedEvalParams<K>
{
    EvalContext curr_ctx_;
  public:

    EvalContext curr_ctx() const { return curr_ctx_; }

    EvalImplBase(ExpressionView const& e, ExpressionView const& x, EvalContext ctx)
        : FixedEvalParams<K>(e, x, ctx)
        , curr_ctx_{++ctx}
    {
    }

    Expression operator()() &&
    {
        auto const v = Validator{this->lhs(), this->rhs()};
        if (v.is_invalid()) return v.status();

        auto ctx = this->context();
        auto const result = static_cast<EvalImpl<K> const*>(this)->impl();
        ctx.log.push(this->self(), this->x(), result, ctx.depth);
        return result;
    }
};

template <Keyword K>
Expression dispatch_eval(ExpressionView const& self, ExpressionView const& x, EvalContext context);


}  // namespace lang
}  // namespace zmbt

#endif
```


