
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/keyword.cpp`
 * 2. Update the keyword data at `templates/expr/keywords.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:keyword_cpp > zmbt-framework/backends/cxx/src/model/keyword.cpp`
 * 4. Commit changes
 */


#include <boost/spirit/include/qi.hpp>
#include <boost/json.hpp>


#include "zmbt/reflect.hpp"
#include "zmbt/model/keyword.hpp"
#include "zmbt/model/keyword_grammar.hpp"


#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {
void tag_invoke(boost::json::value_from_tag const&, boost::json::value& v, Keyword const& kw)
{
    switch(kw)
    {

    case Keyword::Noop: { v = ZMBT_KEYWORD_PREFIX "noop"; break; }
    case Keyword::True: { v = ZMBT_KEYWORD_PREFIX "true"; break; }
    case Keyword::False: { v = ZMBT_KEYWORD_PREFIX "false"; break; }
    case Keyword::Null: { v = ZMBT_KEYWORD_PREFIX "null"; break; }
    case Keyword::Pi: { v = ZMBT_KEYWORD_PREFIX "pi"; break; }
    case Keyword::E: { v = ZMBT_KEYWORD_PREFIX "e"; break; }
    case Keyword::Inf: { v = ZMBT_KEYWORD_PREFIX "inf"; break; }
    case Keyword::Eps: { v = ZMBT_KEYWORD_PREFIX "eps"; break; }
    case Keyword::NaN: { v = ZMBT_KEYWORD_PREFIX "nan"; break; }
    case Keyword::Id: { v = ZMBT_KEYWORD_PREFIX "id"; break; }
    case Keyword::Bool: { v = ZMBT_KEYWORD_PREFIX "bool"; break; }
    case Keyword::Nil: { v = ZMBT_KEYWORD_PREFIX "nil"; break; }
    case Keyword::Not: { v = ZMBT_KEYWORD_PREFIX "not"; break; }
    case Keyword::Neg: { v = ZMBT_KEYWORD_PREFIX "neg"; break; }
    case Keyword::Abs: { v = ZMBT_KEYWORD_PREFIX "abs"; break; }
    case Keyword::Sign: { v = ZMBT_KEYWORD_PREFIX "sign"; break; }
    case Keyword::Ceil: { v = ZMBT_KEYWORD_PREFIX "ceil"; break; }
    case Keyword::Floor: { v = ZMBT_KEYWORD_PREFIX "floor"; break; }
    case Keyword::BitNot: { v = ZMBT_KEYWORD_PREFIX "b-not"; break; }
    case Keyword::Concat: { v = ZMBT_KEYWORD_PREFIX "concat"; break; }
    case Keyword::Union: { v = ZMBT_KEYWORD_PREFIX "union"; break; }
    case Keyword::Intersect: { v = ZMBT_KEYWORD_PREFIX "intersect"; break; }
    case Keyword::Diff: { v = ZMBT_KEYWORD_PREFIX "set-diff"; break; }
    case Keyword::Cartesian: { v = ZMBT_KEYWORD_PREFIX "cartesian"; break; }
    case Keyword::Transp: { v = ZMBT_KEYWORD_PREFIX "transp"; break; }
    case Keyword::Set: { v = ZMBT_KEYWORD_PREFIX "set"; break; }
    case Keyword::List: { v = ZMBT_KEYWORD_PREFIX "list"; break; }
    case Keyword::Card: { v = ZMBT_KEYWORD_PREFIX "card"; break; }
    case Keyword::Size: { v = ZMBT_KEYWORD_PREFIX "size"; break; }
    case Keyword::Sqrt: { v = ZMBT_KEYWORD_PREFIX "sqrt"; break; }
    case Keyword::Sin: { v = ZMBT_KEYWORD_PREFIX "sin"; break; }
    case Keyword::Cos: { v = ZMBT_KEYWORD_PREFIX "cos"; break; }
    case Keyword::Tan: { v = ZMBT_KEYWORD_PREFIX "tan"; break; }
    case Keyword::Asin: { v = ZMBT_KEYWORD_PREFIX "asin"; break; }
    case Keyword::Acos: { v = ZMBT_KEYWORD_PREFIX "acos"; break; }
    case Keyword::Atan: { v = ZMBT_KEYWORD_PREFIX "atan"; break; }
    case Keyword::Sinh: { v = ZMBT_KEYWORD_PREFIX "sinh"; break; }
    case Keyword::Cosh: { v = ZMBT_KEYWORD_PREFIX "cosh"; break; }
    case Keyword::Tanh: { v = ZMBT_KEYWORD_PREFIX "tanh"; break; }
    case Keyword::Asinh: { v = ZMBT_KEYWORD_PREFIX "asinh"; break; }
    case Keyword::Acosh: { v = ZMBT_KEYWORD_PREFIX "acosh"; break; }
    case Keyword::Atanh: { v = ZMBT_KEYWORD_PREFIX "atanh"; break; }
    case Keyword::Exp: { v = ZMBT_KEYWORD_PREFIX "exp"; break; }
    case Keyword::Erf: { v = ZMBT_KEYWORD_PREFIX "erf"; break; }
    case Keyword::Erfc: { v = ZMBT_KEYWORD_PREFIX "erfc"; break; }
    case Keyword::Gamma: { v = ZMBT_KEYWORD_PREFIX "gamma"; break; }
    case Keyword::Round: { v = ZMBT_KEYWORD_PREFIX "round"; break; }
    case Keyword::Sum: { v = ZMBT_KEYWORD_PREFIX "sum"; break; }
    case Keyword::Prod: { v = ZMBT_KEYWORD_PREFIX "prod"; break; }
    case Keyword::Eq: { v = ZMBT_KEYWORD_PREFIX "eq"; break; }
    case Keyword::Ne: { v = ZMBT_KEYWORD_PREFIX "ne"; break; }
    case Keyword::Lt: { v = ZMBT_KEYWORD_PREFIX "lt"; break; }
    case Keyword::Le: { v = ZMBT_KEYWORD_PREFIX "le"; break; }
    case Keyword::Gt: { v = ZMBT_KEYWORD_PREFIX "gt"; break; }
    case Keyword::Ge: { v = ZMBT_KEYWORD_PREFIX "ge"; break; }
    case Keyword::Ni: { v = ZMBT_KEYWORD_PREFIX "ni"; break; }
    case Keyword::NotNi: { v = ZMBT_KEYWORD_PREFIX "not-ni"; break; }
    case Keyword::And: { v = ZMBT_KEYWORD_PREFIX "and"; break; }
    case Keyword::Or: { v = ZMBT_KEYWORD_PREFIX "or"; break; }
    case Keyword::Add: { v = ZMBT_KEYWORD_PREFIX "add"; break; }
    case Keyword::Sub: { v = ZMBT_KEYWORD_PREFIX "sub"; break; }
    case Keyword::SubFrom: { v = ZMBT_KEYWORD_PREFIX "sub-from"; break; }
    case Keyword::Mul: { v = ZMBT_KEYWORD_PREFIX "mul"; break; }
    case Keyword::Div: { v = ZMBT_KEYWORD_PREFIX "div"; break; }
    case Keyword::DivFrom: { v = ZMBT_KEYWORD_PREFIX "div-from"; break; }
    case Keyword::Pow: { v = ZMBT_KEYWORD_PREFIX "pow"; break; }
    case Keyword::PowFrom: { v = ZMBT_KEYWORD_PREFIX "pow-from"; break; }
    case Keyword::Log: { v = ZMBT_KEYWORD_PREFIX "log"; break; }
    case Keyword::LogFrom: { v = ZMBT_KEYWORD_PREFIX "log-from"; break; }
    case Keyword::Mod: { v = ZMBT_KEYWORD_PREFIX "mod"; break; }
    case Keyword::ModFrom: { v = ZMBT_KEYWORD_PREFIX "mod-from"; break; }
    case Keyword::Quot: { v = ZMBT_KEYWORD_PREFIX "quot"; break; }
    case Keyword::QuotFrom: { v = ZMBT_KEYWORD_PREFIX "quot-from"; break; }
    case Keyword::BitAnd: { v = ZMBT_KEYWORD_PREFIX "b-and"; break; }
    case Keyword::BitOr: { v = ZMBT_KEYWORD_PREFIX "b-or"; break; }
    case Keyword::BitXor: { v = ZMBT_KEYWORD_PREFIX "b-xor"; break; }
    case Keyword::BitLshift: { v = ZMBT_KEYWORD_PREFIX "b-lshift"; break; }
    case Keyword::BitLshiftFrom: { v = ZMBT_KEYWORD_PREFIX "b-lshift-from"; break; }
    case Keyword::BitRshift: { v = ZMBT_KEYWORD_PREFIX "b-rshift"; break; }
    case Keyword::BitRshiftFrom: { v = ZMBT_KEYWORD_PREFIX "b-rshift-from"; break; }
    case Keyword::Slide: { v = ZMBT_KEYWORD_PREFIX "slide"; break; }
    case Keyword::Stride: { v = ZMBT_KEYWORD_PREFIX "stride"; break; }
    case Keyword::Repeat: { v = ZMBT_KEYWORD_PREFIX "repeat"; break; }
    case Keyword::Re: { v = ZMBT_KEYWORD_PREFIX "re"; break; }
    case Keyword::At: { v = ZMBT_KEYWORD_PREFIX "at"; break; }
    case Keyword::SetEq: { v = ZMBT_KEYWORD_PREFIX "set-eq"; break; }
    case Keyword::Subset: { v = ZMBT_KEYWORD_PREFIX "subset"; break; }
    case Keyword::Superset: { v = ZMBT_KEYWORD_PREFIX "superset"; break; }
    case Keyword::ProperSubset: { v = ZMBT_KEYWORD_PREFIX "p-subset"; break; }
    case Keyword::ProperSuperset: { v = ZMBT_KEYWORD_PREFIX "p-superset"; break; }
    case Keyword::In: { v = ZMBT_KEYWORD_PREFIX "in"; break; }
    case Keyword::NotIn: { v = ZMBT_KEYWORD_PREFIX "not-in"; break; }
    case Keyword::Map: { v = ZMBT_KEYWORD_PREFIX "map"; break; }
    case Keyword::Filter: { v = ZMBT_KEYWORD_PREFIX "filter"; break; }
    case Keyword::Count: { v = ZMBT_KEYWORD_PREFIX "count"; break; }
    case Keyword::Sort: { v = ZMBT_KEYWORD_PREFIX "sort"; break; }
    case Keyword::Min: { v = ZMBT_KEYWORD_PREFIX "min"; break; }
    case Keyword::Max: { v = ZMBT_KEYWORD_PREFIX "max"; break; }
    case Keyword::Argmin: { v = ZMBT_KEYWORD_PREFIX "argmin"; break; }
    case Keyword::Argmax: { v = ZMBT_KEYWORD_PREFIX "argmax"; break; }
    case Keyword::Recur: { v = ZMBT_KEYWORD_PREFIX "recur"; break; }
    case Keyword::Apply: { v = ZMBT_KEYWORD_PREFIX "apply"; break; }
    case Keyword::Bind: { v = ZMBT_KEYWORD_PREFIX "bind"; break; }
    case Keyword::Reduce: { v = ZMBT_KEYWORD_PREFIX "reduce"; break; }
    case Keyword::Any: { v = ZMBT_KEYWORD_PREFIX "any"; break; }
    case Keyword::All: { v = ZMBT_KEYWORD_PREFIX "all"; break; }
    case Keyword::Saturate: { v = ZMBT_KEYWORD_PREFIX "saturate"; break; }
    case Keyword::Compose: { v = ZMBT_KEYWORD_PREFIX "compose"; break; }
    case Keyword::Approx: { v = ZMBT_KEYWORD_PREFIX "approx"; break; }
        // TODO: throw
        default: v = ":undefined";
    }
}

Keyword
tag_invoke(boost::json::value_to_tag<Keyword> const&, boost::json::value const& v)
{
    if (not v.is_string())
    {
        return Keyword::Undefined;
    }

    static KeywordGrammar const keyword_parser {};

    auto const& str = v.as_string();
    auto iter = str.cbegin();
    auto end = str.cend();
    Keyword keyword_out {Keyword::Undefined};
    static_cast<void>(boost::spirit::qi::parse(iter, end, keyword_parser, keyword_out));
    return keyword_out;
}
} // namespace zmbt

