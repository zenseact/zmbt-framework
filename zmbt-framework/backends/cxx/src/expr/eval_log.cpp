/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <cstdint>
#include <ostream>
#include <sstream>

#include "zmbt/expr/eval_log.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/expression.hpp"


#define SET_BIT(x, n) ((x) |= (1UL << (n)))
#define TEST_BIT(x, n) ((x) & (1UL << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1UL << (n)))


namespace
{
using Keyword = zmbt::lang::Keyword;


void trim_line(std::ostream& os, boost::json::array const& rec)
{
    constexpr std::size_t BuffSize {100};
    constexpr std::size_t MinExpr = 10;

    std::size_t const depth = rec.at(0).as_uint64();
    std::size_t const capacity = BuffSize - 4U*depth;

    if ((capacity < 3*MinExpr) || capacity > BuffSize)
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

    E e_f(rec.at(1));
    E e_x(rec.at(2));
    E e_fx(rec.at(3));

    e_f.prettify_to(buf_f);
    e_x.prettify_to(buf_x);
    e_fx.prettify_to(buf_fx);

    boost::json::string_view f = buf_f;
    boost::json::string_view x = buf_x;
    boost::json::string_view fx = buf_fx;

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


void EvalLog::format(std::ostream& os, boost::json::array const& log, int const indent)
{

    std::uint64_t prev_depth = 0;
    std::size_t vertical_groups = 0;

    auto const notrim = Logger::is_notrim_enabled();


    for (auto const& item: log)
    {
        auto const& rec = item.as_array();
        std::uint64_t const depth = rec.at(0).as_uint64();

        if(!notrim && depth > 20)
        {
            continue;
        }

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

        if (notrim)
        {
            using E = zmbt::lang::Expression;
            E f(rec.at(1));
            E x(rec.at(2));
            E fx(rec.at(3));
            f.prettify_to(os);
            os << " $ ";
            x.prettify_to(os);
            os << " = ";
            fx.prettify_to(os);
            os << '\n';
        }
        else
        {
            trim_line(os, rec);
        }

        prev_depth = depth;
    }
}


boost::json::string EvalLog::str(int const indent) const
{
    if (stack)
    {
        std::stringstream ss;
        EvalLog::format(ss, *stack, indent);
        return ss.str().c_str();
    }
    return "";
}

std::ostream& operator<<(std::ostream& os, EvalLog const& log)
{
    if (log.stack)
    {
        EvalLog::format(os, *log.stack, 0);
    }
    return os;
}


void EvalLog::push(Expression const& expr, Expression const& x, Expression const& result, std::uint64_t const depth) const
{
    if (!stack)
    {
        return;
    }
    stack->push_back({depth, expr.to_json(), x.to_json(), result.to_json()});
}


EvalLog EvalLog::make()
{
    return {std::make_shared<boost::json::array>()};
}


}  // namespace lang
}  // namespace zmbt
