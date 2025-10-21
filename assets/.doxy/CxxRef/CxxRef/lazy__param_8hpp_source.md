

# File lazy\_param.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**lazy\_param.hpp**](lazy__param_8hpp.md)

[Go to the documentation of this file](lazy__param_8hpp.md)


```C++

#ifndef ZMBT_EXPR_LAZY_PARAM_HPP_
#define ZMBT_EXPR_LAZY_PARAM_HPP_

#include <functional>
#include <boost/optional.hpp>

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

    [[nodiscard]] V operator()() const;

    operator V() const;

  private:

    std::function<V()> getter_;
    mutable boost::optional<V> cache_;
};

} // namespace lang
} // namespace zmbt

#endif
```


