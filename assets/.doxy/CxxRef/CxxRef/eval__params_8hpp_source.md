

# File eval\_params.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_params.hpp**](eval__params_8hpp.md)

[Go to the documentation of this file](eval__params_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EVAL_PARAMS_HPP_
#define ZMBT_EXPR_EVAL_PARAMS_HPP_

#include "expression.hpp"
#include "keyword_info.hpp"
#include "eval_context.hpp"


namespace zmbt {
namespace lang {

class EvalParams
{
    Expression lhs_maybe_owned_;
    Expression rhs_maybe_owned_;
    ExpressionView lhs_;
    ExpressionView rhs_;
    ExpressionView const& self_;
    ExpressionView const& x_;
    EvalContext ctx_;

    bool has_default_rhs() const;

  public:

    EvalParams(ExpressionView const& e, ExpressionView const& x, EvalContext ctx);

    ExpressionView const& self() const { return self_; }
    ExpressionView const& x()    const { return x_; }
    ExpressionView const& lhs()  const { return lhs_; }
    ExpressionView const& rhs()  const { return rhs_; }
    EvalContext context()  const { return ctx_; }

};

template <Keyword K>
struct FixedEvalParams : public EvalParams, public KeywordInfo<K>
{
    using EvalParams::EvalParams;
};


}  // namespace lang
}  // namespace zmbt

#endif
```


