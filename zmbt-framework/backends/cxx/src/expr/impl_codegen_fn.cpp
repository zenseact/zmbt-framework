/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * app: codegen.expr
 * template: codegen/expr/templates/zmbt-framework/backends/cxx/src/expr/impl_codegen_fn.cpp
 *
 * How to update:
 * 1. Update the template
 * 2. Run `python -m codegen` in the project root
 * 3. Commit changes
 */

#include <cmath>
#include <complex>
#include <boost/json.hpp>

#include "zmbt/expr/operator.hpp"
#include "zmbt/expr/expression.hpp"


namespace zmbt {

boost::json::value zmbt::lang::Expression::eval_CodegenFn(boost::json::value const& x_, EvalContext const& ctx) const
{
    boost::json::value x = Expression(x_).eval(nullptr, ctx++);
    boost::json::value ret{};
    switch(keyword())
    {
    case Keyword::Sin: { ret = json_from(std::sin(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Cos: { ret = json_from(std::cos(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Tan: { ret = json_from(std::tan(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Asin: { ret = json_from(std::asin(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Acos: { ret = json_from(std::acos(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Atan: { ret = json_from(std::atan(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Sinh: { ret = json_from(std::sinh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Cosh: { ret = json_from(std::cosh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Tanh: { ret = json_from(std::tanh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Asinh: { ret = json_from(std::asinh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Acosh: { ret = json_from(std::acosh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Atanh: { ret = json_from(std::atanh(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Exp: { ret = json_from(std::exp(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Erf: { ret = std::erf(dejsonize<double>(x)); break; }
    case Keyword::Erfc: { ret = std::erfc(dejsonize<double>(x)); break; }
    case Keyword::Gamma: { ret = std::tgamma(dejsonize<double>(x)); break; }
    case Keyword::Abs: { ret = json_from(std::abs(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Ceil: { ret = std::ceil(dejsonize<double>(x)); break; }
    case Keyword::Floor: { ret = std::floor(dejsonize<double>(x)); break; }
    case Keyword::Round: { ret = std::round(dejsonize<double>(x)); break; }
    case Keyword::Sqrt: { ret = json_from(std::sqrt(dejsonize<std::complex<double>>(x))); break; }
    case Keyword::Sign: { ret = dejsonize<double>(x) >= 0 ? 1 : -1; break; }
    case Keyword::ToList: { ret = boost::json::array{x}; break; }
    case Keyword::Parse: { ret = boost::json::parse(x.as_string()); break; }
    case Keyword::Serialize: { ret = {boost::json::serialize(x)}; break; }
    case Keyword::Default: { ret = x.is_null() ? params() : x; break; }
    case Keyword::Error: { ret = underlying(); break; }
    default:
        throw expression_error("got invalid unary math expression: %s", underlying());
    }

    if (ret.is_number())
    {
        ret = real_to_number(boost::json::value_to<double>(ret));
    }

    return ret;
}

} // namespace zmbt

