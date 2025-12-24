

# File eval\_context.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_context.hpp**](eval__context_8hpp.md)

[Go to the documentation of this file](eval__context_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EVAL_CONTEXT_HPP_
#define ZMBT_EXPR_EVAL_CONTEXT_HPP_


#include <map>
#include <memory>
#include <deque>

#include <boost/json.hpp>

#include "operator.hpp"
#include "eval_log.hpp"



namespace zmbt {
namespace lang {

class ExpressionView;
struct EvalContext
{
    static EvalContext make(Operator const& op = {});
    Operator op;
    EvalLog log;
    std::shared_ptr<boost::json::array> traces;
    std::shared_ptr<std::deque<boost::json::object>> capture_links;
    std::shared_ptr<std::map<boost::json::string, ExpressionView>> expr_links;
    // boost::json::object captures;
    std::uint64_t depth;

    EvalContext();

    EvalContext operator++() const;
};

}  // namespace lang
}  // namespace zmbt

#endif
```


