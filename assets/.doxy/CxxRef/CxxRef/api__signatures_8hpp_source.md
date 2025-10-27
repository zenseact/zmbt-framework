

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
    SignatureBase() : Expression(encodeNested(K, {}))
    {}

    operator boost::json::value() const // allowing implicit on expr::*
    {
        return Expression::operator boost::json::value();
    }
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
        return Expression(Expression::encodeNested(K, {param}));
    }

    Expression operator()(Expression && param) const
    {
        return Expression(Expression::encodeNested(K, {std::move(param)}));
    }

    Expression operator()(std::initializer_list<Expression> param) const
    {
        return Expression(Expression::encodeNested(K, {detail::handle_list_init(param)}));
    }
};

template <>
struct SignatureBinary<Keyword::PreProc> : public SignatureBase<Keyword::PreProc>
{
    using SignatureBase<Keyword::PreProc>::SignatureBase;

    Expression operator()(boost::json::string_view const param) const
    {
        return Expression(Expression::encodePreProc(zmbt::format("$[%s]", param).c_str()));
    }

    Expression operator()(std::size_t const param) const
    {
        return Expression(Expression::encodePreProc(zmbt::format("$[%s]", param).c_str()));
    }
};


template <Keyword K>
struct SignatureVariadic : public SignatureBase<K>
{
  private:
    static Expression encodeVariadic(std::initializer_list<zmbt::lang::Expression> params)
    {
        return Expression(Expression::encodeNested(K,  params));
    }

  public:
    using SignatureBase<K>::SignatureBase;

    using E = Expression;
    Expression operator()() const {
        return encodeVariadic({});
    }
    Expression operator()(E const& p0) const {
        return encodeVariadic({p0});
    }
    Expression operator()(E const& p0, E const& p1) const {
        return encodeVariadic({p0, p1});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2) const {
        return encodeVariadic({p0, p1, p2});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3) const {
        return encodeVariadic({p0, p1, p2, p3});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4) const {
        return encodeVariadic({p0, p1, p2, p3, p4});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5) const {
        return encodeVariadic({p0, p1, p2, p3, p4, p5});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6) const {
        return encodeVariadic({p0, p1, p2, p3, p4, p5, p6});
    }
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7) const {
        return encodeVariadic({p0, p1, p2, p3, p4, p5, p6, p7});
    }
    template <class... T>
    Expression operator()(E const& p0, E const& p1, E const& p2, E const& p3, E const& p4, E const& p5, E const& p6, E const& p7, T&&... rest) const {
        return encodeVariadic({p0, p1, p2, p3, p4, p5, p6, p7, zmbt::json_from(rest)...});
    }
};




struct SignatureOp : public SignatureBase<Keyword::Op>
{
    using SignatureBase<Keyword::Op>::SignatureBase;


    Expression operator()(Expression const& type, Expression const& expr) const
    {
        return Expression(Expression::encodeNested(Keyword::Op, {type + expr}));
    }

    template <class T>
    Expression operator()(type_tag<T> tag, Expression const& expr) const
    {
        Operator const op{tag};
        return Expression(Expression::encodeNested(Keyword::Op, {op.annotation() + expr}));
    }
};

struct SignatureCast : public SignatureBinary<Keyword::Cast>
{
    using SignatureBinary<Keyword::Cast>::SignatureBinary;
    using SignatureBinary<Keyword::Cast>::operator();


    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Expression::encodeNested(Keyword::Cast, {op.annotation()}));
    }
};

struct SignatureUncast : public SignatureBinary<Keyword::Uncast>
{
    using SignatureBinary<Keyword::Uncast>::SignatureBinary;
    using SignatureBinary<Keyword::Uncast>::operator();

    template <class T>
    Expression operator()(type_tag<T> tag) const
    {
        Operator const op{tag};
        return Expression(Expression::encodeNested(Keyword::Uncast, {op.annotation()}));
    }
};

struct SignatureErr : public SignatureBase<Keyword::Err>
{
    using SignatureBase<Keyword::Err>::SignatureBase;

    Expression operator()(boost::json::object payload) const
    {
        return Expression(Expression::encodeNested(Keyword::Err, {payload}));
    }

    Expression operator()(boost::json::string_view msg, boost::json::string_view ctx = "") const
    {
        return make_error("", msg, ctx);
    }


    template <class T>
    Expression operator()(type_tag<T>, boost::json::string msg = "", boost::json::string ctx = "") const
    {
        static_assert(std::is_base_of<std::exception, T>::value, "Error type is not supported");
        Expression e = make_error(zmbt::type_name<T>(), msg, ctx);
        make_throw_action(e.error_id(), [msg]{
            throw T(msg.c_str());
        });
        return std::move(e);
    }

    private:
        void make_throw_action(std::string const key, std::function<void()> f) const;
        Expression make_error(boost::json::string_view type, boost::json::string_view msg, boost::json::string_view ctx) const;
};


} // namespace lang
} // namespace zmbt

#endif // ZMBT_EXPR_EXPRESSION_API_SIGNATURES_HPP_
```


