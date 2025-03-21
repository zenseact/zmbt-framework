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
#include "zmbt/model/signal_operator_handler.hpp"
#include "zmbt/model/expression.hpp"


#define SET_BIT(x, n) ((x) |= (1UL << (n)))
#define TEST_BIT(x, n) ((x) & (1UL << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1UL << (n)))


namespace
{
using Keyword = zmbt::dsl::Keyword;


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


    boost::json::serializer sr;

    boost::json::string_view f  = (sr.reset(&rec.at(1)), sr.read(buf_f));
    if (!sr.done()) shrink(f, f.size());

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
    os << f << '(' << x << ") = " << fx << '\n';
};


} // namespace


namespace zmbt {

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
        throw zmbt::model_error("Lit expression requires a value");
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

boost::json::array Expression::toArray(std::initializer_list<Expression> const& list) {
    boost::json::array arr(list.size());
    std::transform(list.begin(), list.end(), arr.begin(), [](Expression const& e) {return e.underlying_;});
    return arr;
}

bool Expression::match(boost::json::value const& observed, SignalOperatorHandler const& op) const
{
    auto result = eval(observed, {op, {}, 0});
    if (!result.is_bool())
    {
        throw zmbt::model_error("expr is not a predicate: `%s`", underlying_);
    }
    return result.get_bool();
}

std::ostream& operator<<(std::ostream& os, Expression::EvalLog const& log)
{
    if (!log.stack)
    {
        return os;
    }

    std::uint64_t prev_depth = 0;
    std::size_t vertical_groups = 0;


    for (auto const& item: *log.stack)
    {
        auto const& rec = item.as_array();
        std::uint64_t const depth = rec.at(0).as_uint64();

        if (prev_depth < depth && prev_depth > 0)
        {
            SET_BIT(vertical_groups, prev_depth);
        }

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
    return os;
}

std::string Expression::EvalLog::str() const
{
    if (!stack)
    {
        return "";
    }
    std::stringstream ss;
    ss << *this;
    return ss.str();
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


Expression::EvalConfig Expression::EvalConfig::operator++(int) const
{
    return {op, log, depth + 1};
}

}  // namespace zmbt
