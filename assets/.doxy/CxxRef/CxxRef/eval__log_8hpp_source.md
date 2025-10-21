

# File eval\_log.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**eval\_log.hpp**](eval__log_8hpp.md)

[Go to the documentation of this file](eval__log_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EVAL_LOG_HPP_
#define ZMBT_EXPR_EVAL_LOG_HPP_

#include <memory>
#include <ostream>
#include <boost/json.hpp>

namespace zmbt {
namespace lang {

class Expression;

struct EvalLog
{
    mutable std::shared_ptr<boost::json::array> stack;

    EvalLog() = default;

    boost::json::string str(int const indent = 0) const;

    void push(Expression const& expr, Expression const& x, Expression const& result, std::uint64_t const depth) const;


    static void format(std::ostream& os, boost::json::array const& log, int const indent = 0);

    friend std::ostream& operator<<(std::ostream& os, EvalLog const& log);

    static EvalLog make();
};

}  // namespace lang
}  // namespace zmbt

#endif
```


