

# File api\_signatures.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**api\_signatures.hpp**](api__signatures_8hpp.md)

[Go to the documentation of this file](api__signatures_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_
#define ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_

#include <cstddef>
#include <initializer_list>
#include <boost/json.hpp>
#include "zmbt/reflect/serialization.hpp"
#include "keyword.hpp"
#include "expression.hpp"

namespace zmbt {
namespace lang {
namespace detail {
    boost::json::value handle_list_init(std::initializer_list<Expression> set);
}

template <Keyword K>
struct SignatureBase : public Expression
{
    SignatureBase() : Expression(K)
    {}
};

template <Keyword K>
struct SignatureConst : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

template <Keyword K>
struct SignatureUnary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
};

template <Keyword K>
struct SignatureBinary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;

    Expression operator()(Expression const& param) const
    {
        return Expression(K, param);
    }

    Expression operator()(std::initializer_list<Expression> param) const
    {
        return Expression(K, detail::handle_list_init(param));
    }
};

template <Keyword K>
struct SignatureTernary : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;

    Expression operator()(Expression const& expr, Expression const& param) const
    {
        return Expression(K, boost::json::array{expr, param});
    }
};


template <Keyword K>
struct SignatureVariadic : public SignatureBase<K>
{
    using SignatureBase<K>::SignatureBase;
    using E = Expression;
    Expression operator()() const {
        return Expression(K, boost::json::array{});
    }
    Expression operator()(E const& p0) const {
        return Expression(K, boost::json::array{p0});
    }
    Expression operator()(E const& p0, E const& p1) const {
        return Expression(K, {p0, p1});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2) const {
        return Expression(K, {p0, p1, p2});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3) const {
        return Expression(K, {p0, p1, p2, p3});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4) const {
        return Expression(K, {p0, p1, p2, p3, p4});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7});
    }
    template <class... T>
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7, T&&... rest) const {
        return Expression(K, {p0, p1, p2, p3, p4, p5, p6, p7, zmbt::json_from(rest)...});
    }
};




struct SignatureOverload : public SignatureTernary<Keyword::Overload>
{
    using SignatureTernary<Keyword::Overload>::SignatureTernary;
    using SignatureTernary<Keyword::Overload>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag, Expression const& expr) const
    {
        Operator const op{tag};
        return Expression(Keyword::Overload, {op.annotation(), expr});
    }
};

struct SignatureDecorate : public SignatureBinary<Keyword::Decorate>
{
    using SignatureBinary<Keyword::Decorate>::SignatureBinary;
    using SignatureBinary<Keyword::Decorate>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Keyword::Decorate, op.annotation());
    }
};

struct SignatureUndecorate : public SignatureBinary<Keyword::Undecorate>
{
    using SignatureBinary<Keyword::Undecorate>::SignatureBinary;
    using SignatureBinary<Keyword::Undecorate>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Keyword::Undecorate, op.annotation());
    }
};

struct SignatureError : public SignatureBase<Keyword::Error>
{
    using SignatureBase<Keyword::Error>::SignatureBase;

    Expression operator()(boost::json::string_view msg, boost::json::string_view ctx = "") const
    {
        boost::json::object err {{"message", msg}};
        if (!ctx.empty())
        {
            err["context"] = ctx;
        }
        return Expression(Keyword::Error, err);
    }

    template <class T>
    Expression operator()(type_tag<T>, boost::json::string_view msg = "", boost::json::string_view ctx = "") const
    {
        boost::json::object err {{"type", zmbt::type_name<T>()}};
        if (!msg.empty())
        {
            err["message"] = msg;
        }
        if (!ctx.empty())
        {
            err["context"] = ctx;
        }
        return Expression(Keyword::Error, err);
    }
};

#define ZMBT_DEBUG_EXPR(f) ::zmbt::expr::Debug(f, ZMBT_CUR_LOC)

struct SignatureDebug : public SignatureTernary<Keyword::Debug>
{
    using SignatureTernary<Keyword::Debug>::SignatureTernary;

    Expression operator()(Expression const& expr, Expression const& identifier) const
    {
        return SignatureTernary<Keyword::Debug>::operator()(expr, identifier);
    }

    Expression operator()(Expression const& expr) const
    {
        return operator()(expr, "anonymous");
    }
};

} // namespace lang
} // namespace zmbt

#endif // ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_
```


