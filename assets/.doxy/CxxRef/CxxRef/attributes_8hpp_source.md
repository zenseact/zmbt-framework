

# File attributes.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**attributes.hpp**](attributes_8hpp.md)

[Go to the documentation of this file](attributes_8hpp.md)


```C++

#ifndef ZMBT_EXPR_KEYWORD_ATTR_HPP_
#define ZMBT_EXPR_KEYWORD_ATTR_HPP_

#include <cstdint>
#include "zmbt/expr/keyword.hpp"

namespace zmbt {
namespace lang {


namespace attr
{
constexpr std::uint32_t is_const       = 1 <<  1;
constexpr std::uint32_t is_unary       = 1 <<  2;
constexpr std::uint32_t is_binary      = 1 <<  3;
constexpr std::uint32_t is_variadic    = 1 <<  4;
constexpr std::uint32_t is_predicate   = 1 <<  5;
constexpr std::uint32_t is_operator    = 1 <<  6;
constexpr std::uint32_t is_hiord       = 1 <<  7;
constexpr std::uint32_t is_noop        = 1 <<  8;
constexpr std::uint32_t is_literal     = 1 <<  9;
constexpr std::uint32_t is_quote       = 1 << 10;
constexpr std::uint32_t is_preproc     = 1 << 11;
constexpr std::uint32_t is_error       = 1 << 12;
constexpr std::uint32_t is_pipe        = 1 << 13;
constexpr std::uint32_t is_fork        = 1 << 14;
constexpr std::uint32_t is_overload    = 1 << 15;
constexpr std::uint32_t is_infix_chain = 1 << 16;
constexpr std::uint32_t is_autogen     = 1 << 17;
constexpr std::uint32_t is_sideeffect  = 1 << 18;
constexpr std::uint32_t is_single_form = 1 << 19;
}


constexpr std::uint32_t attributes(Keyword const& k)
{
    using namespace attr;
    switch (k)
    {
    case Keyword::Literal: return is_unary | is_literal;
    case Keyword::Void: return is_unary;
    case Keyword::LazyToken: return is_unary;
    case Keyword::_Continue: return is_unary;
    case Keyword::_Resolve: return is_unary;
    case Keyword::Noop: return is_const | is_autogen | is_noop;
    case Keyword::Null: return is_const | is_autogen;
    case Keyword::True: return is_const | is_predicate | is_autogen;
    case Keyword::False: return is_const | is_predicate | is_autogen;
    case Keyword::Pi: return is_const | is_autogen;
    case Keyword::E: return is_const | is_autogen;
    case Keyword::Inf: return is_const | is_autogen;
    case Keyword::Eps: return is_const | is_autogen;
    case Keyword::NaN: return is_const | is_autogen;
    case Keyword::Thread: return is_const | is_autogen;
    case Keyword::Sin: return is_unary | is_autogen;
    case Keyword::Cos: return is_unary | is_autogen;
    case Keyword::Tan: return is_unary | is_autogen;
    case Keyword::Asin: return is_unary | is_autogen;
    case Keyword::Acos: return is_unary | is_autogen;
    case Keyword::Atan: return is_unary | is_autogen;
    case Keyword::Sinh: return is_unary | is_autogen;
    case Keyword::Cosh: return is_unary | is_autogen;
    case Keyword::Tanh: return is_unary | is_autogen;
    case Keyword::Asinh: return is_unary | is_autogen;
    case Keyword::Acosh: return is_unary | is_autogen;
    case Keyword::Atanh: return is_unary | is_autogen;
    case Keyword::Exp: return is_unary | is_autogen;
    case Keyword::Erf: return is_unary | is_autogen;
    case Keyword::Erfc: return is_unary | is_autogen;
    case Keyword::Gamma: return is_unary | is_autogen;
    case Keyword::Abs: return is_unary | is_autogen;
    case Keyword::Ceil: return is_unary | is_autogen;
    case Keyword::Floor: return is_unary | is_autogen;
    case Keyword::Round: return is_unary | is_autogen;
    case Keyword::Sqrt: return is_unary | is_autogen;
    case Keyword::Sign: return is_unary | is_autogen;
    case Keyword::Neg: return is_unary | is_operator;
    case Keyword::Add: return is_binary | is_operator;
    case Keyword::Sub: return is_binary | is_operator;
    case Keyword::Mul: return is_binary | is_operator;
    case Keyword::Div: return is_binary | is_operator;
    case Keyword::Pow: return is_binary | is_operator;
    case Keyword::Log: return is_binary | is_operator;
    case Keyword::Mod: return is_binary | is_operator;
    case Keyword::Quot: return is_binary | is_operator;
    case Keyword::BitNot: return is_unary | is_operator;
    case Keyword::BitAnd: return is_binary | is_operator;
    case Keyword::BitOr: return is_binary | is_operator;
    case Keyword::BitXor: return is_binary | is_operator;
    case Keyword::Lshift: return is_binary | is_operator;
    case Keyword::Rshift: return is_binary | is_operator;
    case Keyword::Eq: return is_binary | is_predicate | is_operator;
    case Keyword::Ne: return is_binary | is_predicate | is_operator;
    case Keyword::Lt: return is_binary | is_predicate | is_operator;
    case Keyword::Le: return is_binary | is_predicate | is_operator;
    case Keyword::Gt: return is_binary | is_predicate | is_operator;
    case Keyword::Ge: return is_binary | is_predicate | is_operator;
    case Keyword::Near: return is_binary | is_predicate;
    case Keyword::Union: return is_binary;
    case Keyword::Intersect: return is_binary;
    case Keyword::Diff: return is_binary;
    case Keyword::SetEq: return is_binary | is_predicate | is_operator;
    case Keyword::Subset: return is_binary | is_predicate | is_operator;
    case Keyword::Superset: return is_binary | is_predicate | is_operator;
    case Keyword::PSubset: return is_binary | is_predicate | is_operator;
    case Keyword::PSuperset: return is_binary | is_predicate | is_operator;
    case Keyword::In: return is_binary | is_predicate | is_operator;
    case Keyword::NotIn: return is_binary | is_predicate | is_operator;
    case Keyword::Ni: return is_binary | is_predicate | is_operator;
    case Keyword::NotNi: return is_binary | is_predicate | is_operator;
    case Keyword::Bool: return is_unary | is_predicate | is_operator;
    case Keyword::Not: return is_unary | is_predicate | is_operator;
    case Keyword::And: return is_binary | is_operator;
    case Keyword::Or: return is_binary | is_operator;
    case Keyword::If: return is_variadic;
    case Keyword::Elif: return is_variadic;
    case Keyword::Else: return is_binary;
    case Keyword::Id: return is_unary | is_autogen;
    case Keyword::Transp: return is_unary;
    case Keyword::Cartesian: return is_unary;
    case Keyword::Reverse: return is_unary;
    case Keyword::Uniques: return is_unary;
    case Keyword::Items: return is_unary;
    case Keyword::Keys: return is_unary;
    case Keyword::Values: return is_unary;
    case Keyword::Enumerate: return is_unary;
    case Keyword::Flatten: return is_unary;
    case Keyword::ToList: return is_unary | is_autogen;
    case Keyword::First: return is_unary;
    case Keyword::Last: return is_unary;
    case Keyword::Rand: return is_unary | is_sideeffect;
    case Keyword::RandInt: return is_variadic | is_sideeffect;
    case Keyword::Sequence: return is_binary;
    case Keyword::Arange: return is_unary;
    case Keyword::Parse: return is_unary | is_autogen;
    case Keyword::Str: return is_unary | is_autogen;
    case Keyword::Re: return is_binary | is_predicate;
    case Keyword::Fmt: return is_variadic;
    case Keyword::Capitalize: return is_unary;
    case Keyword::LowerCase: return is_unary;
    case Keyword::UpperCase: return is_unary;
    case Keyword::Card: return is_unary;
    case Keyword::Size: return is_unary;
    case Keyword::Sum: return is_unary;
    case Keyword::Prod: return is_unary;
    case Keyword::Avg: return is_unary;
    case Keyword::Slide: return is_binary;
    case Keyword::Stride: return is_binary;
    case Keyword::Chunks: return is_binary;
    case Keyword::Repeat: return is_binary;
    case Keyword::Cat: return is_binary;
    case Keyword::Push: return is_binary;
    case Keyword::At: return is_binary;
    case Keyword::Del: return is_binary;
    case Keyword::Lookup: return is_binary;
    case Keyword::Cast: return is_unary;
    case Keyword::Uncast: return is_unary;
    case Keyword::Fold: return is_binary | is_hiord;
    case Keyword::Map: return is_binary | is_hiord;
    case Keyword::Filter: return is_binary | is_hiord;
    case Keyword::Count: return is_binary | is_hiord;
    case Keyword::Each: return is_binary | is_predicate | is_hiord;
    case Keyword::Sort: return is_binary | is_hiord;
    case Keyword::Find: return is_binary | is_hiord;
    case Keyword::FindPtr: return is_binary | is_hiord;
    case Keyword::FindIdx: return is_binary | is_hiord;
    case Keyword::Min: return is_binary | is_hiord;
    case Keyword::Max: return is_binary | is_hiord;
    case Keyword::Argmin: return is_binary | is_hiord;
    case Keyword::Argmax: return is_binary | is_hiord;
    case Keyword::Recur: return is_variadic | is_hiord;
    case Keyword::Unfold: return is_variadic | is_hiord;
    case Keyword::Op: return is_unary | is_hiord | is_overload;
    case Keyword::Bind: return is_binary | is_hiord;
    case Keyword::Fn: return is_variadic | is_hiord;
    case Keyword::Link: return is_binary | is_hiord;
    case Keyword::Get: return is_binary | is_hiord;
    case Keyword::EnvLoad: return is_binary | is_hiord | is_sideeffect;
    case Keyword::EnvStore: return is_binary | is_hiord | is_sideeffect;
    case Keyword::Any: return is_variadic | is_predicate | is_hiord;
    case Keyword::All: return is_variadic | is_predicate | is_hiord;
    case Keyword::Saturate: return is_variadic | is_predicate | is_hiord;
    case Keyword::Pipe: return is_variadic | is_hiord | is_pipe;
    case Keyword::Tuple: return is_variadic | is_hiord;
    case Keyword::Fork: return is_variadic | is_hiord | is_fork;
    case Keyword::Flip: return is_binary | is_hiord;
    case Keyword::Dbg: return is_binary | is_hiord;
    case Keyword::Eval: return is_binary | is_hiord;
    case Keyword::Try: return is_binary | is_hiord;
    case Keyword::Kwrd: return is_unary | is_hiord;
    case Keyword::Prms: return is_unary | is_hiord;
    case Keyword::Q: return is_binary | is_autogen | is_quote;
    case Keyword::D: return is_binary | is_autogen;
    case Keyword::Err: return is_unary | is_autogen | is_error;
    case Keyword::IsErr: return is_unary | is_autogen;
    case Keyword::Assert: return is_binary;
    case Keyword::Trace: return is_binary | is_single_form;
    case Keyword::PreProc: return is_binary | is_autogen | is_preproc;
    default:
        return 0U;
    }
}

} // namespace lang
} // namespace zmbt



#endif // ZMBT_MODEL_KEYWORD_HPP_
```


