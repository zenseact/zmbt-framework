/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <cstdint>
#include <ostream>
#include <sstream>


#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"


#define SET_BIT(x, n) ((x) |= (1UL << (n)))
#define TEST_BIT(x, n) ((x) & (1UL << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1UL << (n)))


namespace
{
using Keyword = zmbt::lang::Keyword;


boost::json::value const* ptrToParams(Keyword const& keyword, boost::json::value const& underlying)
{
    if (keyword == Keyword::Literal)
    {
        return &underlying;
    }
    return underlying.is_object() ? &(underlying.get_object().cbegin()->value()) : nullptr;
}

boost::json::value get_underlying(Keyword const& keyword, boost::json::value const& params)
{
    return keyword == Keyword::Literal ? params : boost::json::value{{zmbt::json_from(keyword).get_string(), params}};
}

void trim_line(std::ostream& os, boost::json::array const& rec)
{
    constexpr std::size_t BuffSize {100};
    constexpr std::size_t MinExpr = 10;

    auto const depth = rec.at(0).as_uint64();
    std::size_t capacity = BuffSize - 4U*depth;

    if (capacity < 3*MinExpr)
    {
        os << "...\n";
        return;
    }

    char buf_f[BuffSize];
    char buf_x[BuffSize];
    char buf_fx[BuffSize];


    auto const shrink = [](boost::json::string_view& view, std::uint64_t const n){
        if (view.size() < n) return;
        view = view.substr(0, n);
        if (n < 3) return;
        if (n == 3) {
            view = "...";
            return;
        }
        char* buf = const_cast<char*>(view.data());
        buf[n-1] = '.';
        buf[n-2] = '.';
        buf[n-3] = '.';
        return;
    };
    using E = zmbt::lang::Expression;

    // TODO: optimize prettify with static buffer
    E e_f(rec.at(1));
    auto const fstr = e_f.prettify();
    std::size_t len = std::min(fstr.size(), BuffSize - 1);
    std::memcpy(buf_f, fstr.data(), len);
    buf_f[len] = '\0';
    boost::json::string_view f = buf_f;

    boost::json::serializer sr;
    boost::json::string_view x  = (sr.reset(&rec.at(2)), sr.read(buf_x));
    if (!sr.done()) shrink(x, x.size());

    boost::json::string_view fx = (sr.reset(&rec.at(3)), sr.read(buf_fx));
    if (!sr.done()) shrink(fx, fx.size());


    std::size_t total_size = f.size() + x.size() + fx.size();

    if (total_size > capacity)
    {
        shrink(fx, capacity - 2*MinExpr);
        total_size = f.size() + x.size() + fx.size();
    }

    if (total_size > capacity)
    {
        shrink(x, MinExpr);
        total_size = f.size() + x.size() + fx.size();
    }

    if (total_size > capacity)
    {
        std::uint64_t const n =  total_size - capacity;
        shrink(f, (n < fx.size()) ? f.size() - n : MinExpr);
    }
    os << f << " $ " << x  << " = " << fx << '\n';
}


} // namespace


namespace zmbt {
namespace lang {

Expression::Expression(internal_tag, Keyword const& keyword, boost::json::value&& underlying)
    : keyword_{keyword}
    , underlying_{std::move(underlying)}
    , params_ptr_{ptrToParams(keyword, underlying_)}
{
}

Expression::Expression(Expression const& o)
{
    keyword_ = o.keyword_;
    underlying_ = o.underlying_;
    params_ptr_ = ptrToParams(keyword_, underlying_);
}

Expression::Expression(Expression && o)
{
    keyword_ = o.keyword_;
    underlying_ = std::move(o.underlying_);
    params_ptr_ = ptrToParams(keyword_, underlying_);
}

Expression& Expression::operator=(Expression const& o)
{
    keyword_ = o.keyword_;
    underlying_ = o.underlying_;
    params_ptr_ = ptrToParams(keyword_, underlying_);
    return *this;
}

Expression& Expression::operator=(Expression && o)
{
    keyword_ = o.keyword_;
    underlying_ = std::move(o.underlying_);
    params_ptr_ = ptrToParams(keyword_, underlying_);
    return *this;
}

Expression::Expression(Keyword const& keyword, boost::json::value const& params)
    : Expression(internal_tag{}, keyword, get_underlying(keyword, params))
{
}


Expression::Expression(Keyword const& keyword)
    : Expression(internal_tag{}, keyword, json_from(keyword))
{
    if (keyword == Keyword::Literal)
    {
        throw zmbt::expression_error("Lit expression requires a value");
    }
}



struct Expression::json_ctor_params
{
    boost::json::value underlying;
    Keyword keyword;


    json_ctor_params(boost::json::value&& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = std::move(expr);
        }
        else
        {
            keyword = Keyword::Literal;
            underlying = expr;
        }
    }

    json_ctor_params(boost::json::value const& expr)
    {

        if (expr.is_object() && expr.get_object().size() == 1)
        {
            keyword = dejsonize<Keyword>(expr.get_object().cbegin()->key());
        }
        else
        {
            keyword = dejsonize<Keyword>(expr);
        }

        if (keyword != Keyword::Undefined)
        {
            underlying = expr;
        }
        else
        {
            keyword = Keyword::Literal;
            underlying = expr;
        }
    }
};

Expression::Expression(json_ctor_params&& params)
    : Expression(internal_tag{}, params.keyword, std::move(params.underlying))
{
}


Expression::Expression()
    : Expression(internal_tag{}, Keyword::Undefined, nullptr)
{
}

Expression::Expression(boost::json::value const& expr)
    : Expression(json_ctor_params{std::move(expr)})
{
}

Expression::Expression(std::initializer_list<boost::json::value_ref> items)
    : Expression(Keyword::Literal, boost::json::value(items))
{
}

Expression Expression::asPredicate(boost::json::value const& underlying)
{
    Expression expr(underlying);
    if (!expr.is_noop() && expr.is_const())
    {
        expr = Expression(Keyword::Eq, expr.eval());
    }
    return expr;
}

bool Expression::is_const() const
{
    // TODO: test it and clarify param evaluation
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;
    if (is_nonempty_composition())
    {
        return Expression(params().as_array().back()).is_const();
    }
    if (is_nonempty_fork())
    {
        auto const paramlist = parameter_list();
        for (auto const& e: paramlist)
        {
            if (not e.is_const()) return false;
        }
        return true;
    }
    else if (is(Keyword::Op) && has_params())
    {
        return Expression(params().as_array().back()).is_const();
    }
    return is(Keyword::Literal) || is(Keyword::C) || is(Keyword::Err) || (CodegenType::Const == getCodegenType(keyword()));
}

std::string Expression::keyword_to_str() const
{
    auto const k = zmbt::json_from(keyword()).as_string();
    auto const w =  boost::json::string_view(k.c_str());
    return w.substr(std::strlen(ZMBT_KEYWORD_PREFIX)).data();
}

bool Expression::is_boolean() const
{
    using lang::detail::CodegenType;
    using lang::detail::getCodegenType;
    using lang::detail::isBoolean;
    if(is_literal())
    {
        return params().is_bool();
    }
    if (isBoolean(keyword()))
    {
        return true;
    }
    if (is(Keyword::Op) && has_params())
    {
        return Expression(params()).is_boolean();
    }
    if (is_nonempty_composition())
    {
        return Expression(params().as_array().back()).is_boolean();
    }
    return false;
}

bool Expression::is_hiord() const
{
    return lang::detail::isHiOrd(keyword());
}

std::list<Expression> Expression::parameter_list() const
{
    std::list<Expression> result;

    auto const k = keyword();
    bool const high_arity = detail::isVariadic(k) || detail::isTernary(k);
    bool const expect_list = high_arity && params().is_array();

    if (expect_list) {
        for (auto const& v : params().get_array()) {
            result.emplace_back(Expression(v));
        }
    } else if (has_params()) {
        result.emplace_back(subexpr());
    }
    return result;
}


bool Expression::match(boost::json::value const& observed, Operator const& op) const
{
    auto result = eval(observed, {op, {}, 0});
    if (!result.is_bool())
    {
        throw zmbt::expression_error("expr is not a predicate: `%s`", underlying_);
    }
    return result.get_bool();
}


void Expression::EvalLog::format(std::ostream& os, boost::json::array const& log, int const indent)
{

    std::uint64_t prev_depth = 0;
    std::size_t vertical_groups = 0;


    for (auto const& item: log)
    {
        auto const& rec = item.as_array();
        std::uint64_t const depth = rec.at(0).as_uint64();

        if (prev_depth < depth && prev_depth > 0)
        {
            SET_BIT(vertical_groups, prev_depth);
        }

        os << std::string(indent, ' ');

        for (std::uint64_t i = 0; i < depth; ++i)
        {
            os << (TEST_BIT(vertical_groups, i) ? "│   " : "   ");
        }

        if (depth == 0)
        {
            os << "□  "; // QED
        }
        else if (prev_depth == depth || TEST_BIT(vertical_groups, depth))
        {
            os << "├── ";
        }
        else
        {
            os << "┌── ";
        }

        if (TEST_BIT(vertical_groups, depth))
        {
            CLEAR_BIT(vertical_groups, depth);
        }

        trim_line(os, rec);
        prev_depth = depth;
    }
}


boost::json::string Expression::EvalLog::str(int const indent) const
{
    if (stack)
    {
        std::stringstream ss;
        Expression::EvalLog::format(ss, *stack, indent);
        return ss.str().c_str();
    }
    return "";
}

std::ostream& operator<<(std::ostream& os, Expression::EvalLog const& log)
{    
    if (log.stack)
    {
        Expression::EvalLog::format(os, *log.stack, 0);
    }
    return os;
}


void Expression::EvalLog::push(boost::json::value const& expr, boost::json::value const& x, boost::json::value const& result, std::uint64_t const depth) const
{
    if (!stack)
    {
        return;
    }
    stack->push_back({depth, expr, x, result});
}


Expression::EvalLog Expression::EvalLog::make()
{
    return {std::make_shared<boost::json::array>()};
}


Expression::EvalContext Expression::EvalContext::operator++(int) const
{
    return {op, log, depth + 1};
}

}  // namespace lang
}  // namespace zmbt
