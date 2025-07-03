

# File expression.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**expression.hpp**](expression_8hpp.md)

[Go to the documentation of this file](expression_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EXPRESSION_V2_HPP_
#define ZMBT_EXPR_EXPRESSION_V2_HPP_

#include <ostream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "operator.hpp"
#include "keyword.hpp"
#include "keyword_grammar.hpp"
#include "keyword_codegen_type.hpp"
#include "exceptions.hpp"


namespace zmbt {
namespace lang {

class Expression
{
public:
    using V = boost::json::value;
    using Keyword = lang::Keyword;

    struct EvalLog
    {
        mutable std::shared_ptr<boost::json::array> stack;

        EvalLog() = default;

        std::string str() const;

        void push(boost::json::value const& expr, boost::json::value const& x, boost::json::value const& result, std::uint64_t const depth) const;


        static void format(std::ostream& os, boost::json::array const& log, int const indent = 0);

        friend std::ostream& operator<<(std::ostream& os, EvalLog const& log);

        static EvalLog make();
    };

    struct EvalContext
    {
        Operator op;
        EvalLog log;
        std::uint64_t const depth;

        EvalContext operator++(int) const;
    };

private:
    Keyword keyword_;
    boost::json::value underlying_;
    boost::json::value const* params_ptr_;
    boost::json::value const dummy_params_{};

    struct internal_tag{};
    Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying);

    struct json_ctor_params;
    Expression(json_ctor_params&&);

    void handle_binary_args(V const& x, V const*& lhs, V const*& rhs) const;

    boost::json::value eval_Const(boost::json::value const&) const;
    boost::json::value eval_UnaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_BinaryOp(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_CodegenFn(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_HiOrd(boost::json::value const&, EvalContext const&) const;
    boost::json::value eval_Special(boost::json::value const&, EvalContext const&) const;

    Expression(internal_tag, Operator const& op);

public:

    static Expression asPredicate(boost::json::value const& underlying);

    Expression(std::initializer_list<boost::json::value_ref> items);
    Expression(boost::json::value const& expr);
    Expression(Keyword const& keyword, boost::json::value const& params);
    explicit Expression(Keyword const& keyword);

    Expression();

    template <class T>
    Expression(T const& sample) : Expression(json_from(sample)) {}

    Expression(Expression const& o);
    Expression(Expression && o);
    Expression& operator=(Expression const& o);
    Expression& operator=(Expression && o);

    friend Expression operator|(Expression const& lhs, Expression const& rhs);

    friend Expression operator&(Expression const& lhs, Expression const& rhs);

    friend V operator*(Expression const& expr, Expression const& x);

    friend V operator*(Expression const& expr);




    ~Expression() = default;

    boost::json::value const& underlying() const
    {
        return underlying_;
    }

    boost::json::value const& serialize() const
    {
        return underlying();
    }

    bool operator==(Expression const& o) const
    {
        return (keyword_ == o.keyword_) && (underlying_ == o.underlying_);
    }

    bool operator!=(Expression const& o) const
    {
        return !operator==(o);
    }

    Keyword keyword() const
    {
        return keyword_;
    }

    boost::json::value const& params() const
    {
        return has_params() ? *params_ptr_ : dummy_params_;
    }

    boost::json::value const& subexpr() const
    {
        return params();
    }

    bool has_params() const
    {
        return nullptr != params_ptr_;
    }

    bool is(Keyword const kwrd) const
    {
        return kwrd == keyword_;
    }

    bool is_literal() const
    {
        return is(Keyword::Literal);
    }

    bool is_noop() const
    {
        return is(Keyword::Noop);
    }

    bool is_const() const;

    bool is_hiord() const;

    bool is_boolean() const;


    operator boost::json::value() const
    {
        return underlying();
    }

    boost::json::value eval(boost::json::value const& x = nullptr, EvalContext const& ctx = {}) const;

    bool match(boost::json::value const& observed, Operator const& op = {}) const;

};

ZMBT_INJECT_SERIALIZATION
}  // namespace lang

template<class T>
struct reflect::custom_serialization<T, mp_if<is_base_of<lang::Expression, T>, void>> {

    static boost::json::value
    json_from(T const& t)
    {
        return t.underlying();
    }

    static T
    dejsonize(boost::json::value const& v)
    {
        return static_cast<T>(lang::Expression(v));
    }
};

}  // namespace zmbt

#endif
```


