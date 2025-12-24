

# File api.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**api.hpp**](expr_2api_8hpp.md)

[Go to the documentation of this file](expr_2api_8hpp.md)


```C++

#ifndef ZMBT_EXPR_EXPRESSION_API_HPP_
#define ZMBT_EXPR_EXPRESSION_API_HPP_

#include "api_signatures.hpp"

namespace zmbt {
namespace expr {


extern lang::SignatureConst<::zmbt::lang::Keyword::Noop> const Noop;
extern lang::SignatureConst<::zmbt::lang::Keyword::Noop> const _;

extern lang::SignatureConst<::zmbt::lang::Keyword::Null> const Null;

extern lang::SignatureConst<::zmbt::lang::Keyword::True> const True;

extern lang::SignatureConst<::zmbt::lang::Keyword::False> const False;

extern lang::SignatureConst<::zmbt::lang::Keyword::Pi> const Pi;

extern lang::SignatureConst<::zmbt::lang::Keyword::E> const E;

extern lang::SignatureConst<::zmbt::lang::Keyword::Inf> const Inf;

extern lang::SignatureConst<::zmbt::lang::Keyword::Eps> const Eps;

extern lang::SignatureConst<::zmbt::lang::Keyword::NaN> const NaN;

extern lang::SignatureConst<::zmbt::lang::Keyword::Thread> const Thread;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Sin> const Sin;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Cos> const Cos;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Tan> const Tan;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Asin> const Asin;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Acos> const Acos;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Atan> const Atan;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Sinh> const Sinh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Cosh> const Cosh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Tanh> const Tanh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Asinh> const Asinh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Acosh> const Acosh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Atanh> const Atanh;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Exp> const Exp;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Erf> const Erf;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Erfc> const Erfc;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Gamma> const Gamma;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Abs> const Abs;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Ceil> const Ceil;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Floor> const Floor;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Round> const Round;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Sqrt> const Sqrt;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Sign> const Sign;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Neg> const Neg;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Add> const Add;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Sub> const Sub;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Mul> const Mul;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Div> const Div;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Pow> const Pow;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Log> const Log;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Mod> const Mod;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Quot> const Quot;

extern lang::SignatureUnary<::zmbt::lang::Keyword::BitNot> const BitNot;

extern lang::SignatureBinary<::zmbt::lang::Keyword::BitAnd> const BitAnd;

extern lang::SignatureBinary<::zmbt::lang::Keyword::BitOr> const BitOr;

extern lang::SignatureBinary<::zmbt::lang::Keyword::BitXor> const BitXor;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Lshift> const Lshift;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Rshift> const Rshift;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Eq> const Eq;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Ne> const Ne;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Lt> const Lt;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Le> const Le;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Gt> const Gt;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Ge> const Ge;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Near> const Near;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Near> const Approx;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Union> const Union;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Intersect> const Intersect;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Diff> const Diff;

extern lang::SignatureBinary<::zmbt::lang::Keyword::SetEq> const SetEq;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Subset> const Subset;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Superset> const Superset;

extern lang::SignatureBinary<::zmbt::lang::Keyword::PSubset> const PSubset;

extern lang::SignatureBinary<::zmbt::lang::Keyword::PSuperset> const PSuperset;

extern lang::SignatureBinary<::zmbt::lang::Keyword::In> const In;

extern lang::SignatureBinary<::zmbt::lang::Keyword::NotIn> const NotIn;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Ni> const Ni;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Ni> const Contains;

extern lang::SignatureBinary<::zmbt::lang::Keyword::NotNi> const NotNi;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Bool> const Bool;
extern lang::SignatureUnary<::zmbt::lang::Keyword::Bool> const Truthy;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Not> const Not;
extern lang::SignatureUnary<::zmbt::lang::Keyword::Not> const Falsy;
extern lang::SignatureUnary<::zmbt::lang::Keyword::Not> const Nil;

extern lang::SignatureBinary<::zmbt::lang::Keyword::And> const And;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Or> const Or;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::If> const If;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Elif> const Elif;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Else> const Else;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Id> const Id;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Transp> const Transp;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Cartesian> const Cartesian;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Reverse> const Reverse;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Uniques> const Uniques;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Items> const Items;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Keys> const Keys;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Values> const Values;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Enumerate> const Enumerate;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Flatten> const Flatten;

extern lang::SignatureUnary<::zmbt::lang::Keyword::ToList> const ToList;

extern lang::SignatureUnary<::zmbt::lang::Keyword::First> const First;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Last> const Last;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Rand> const Rand;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::RandInt> const RandInt;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Sequence> const Sequence;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Sequence> const Seq;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Arange> const Arange;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Parse> const Parse;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Str> const Str;
extern lang::SignatureUnary<::zmbt::lang::Keyword::Str> const Serialize;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Re> const Re;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Re> const Regex;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Fmt> const Fmt;
extern lang::SignatureVariadic<::zmbt::lang::Keyword::Fmt> const Format;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Capitalize> const Capitalize;

extern lang::SignatureUnary<::zmbt::lang::Keyword::LowerCase> const LowerCase;

extern lang::SignatureUnary<::zmbt::lang::Keyword::UpperCase> const UpperCase;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Card> const Card;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Size> const Size;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Sum> const Sum;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Prod> const Prod;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Avg> const Avg;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Slide> const Slide;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Stride> const Stride;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Chunks> const Chunks;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Repeat> const Repeat;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Cat> const Cat;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Cat> const Concat;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Push> const Push;

extern lang::SignatureBinary<::zmbt::lang::Keyword::At> const At;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Del> const Del;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Del> const Delete;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Lookup> const Lookup;

extern lang::SignatureCast const Cast;
extern lang::SignatureCast const Decorate;

extern lang::SignatureUncast const Uncast;
extern lang::SignatureUncast const Undecorate;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Fold> const Fold;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Fold> const Reduce;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Map> const Map;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Filter> const Filter;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Count> const Count;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Each> const Each;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Sort> const Sort;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Find> const Find;

extern lang::SignatureBinary<::zmbt::lang::Keyword::FindPtr> const FindPtr;

extern lang::SignatureBinary<::zmbt::lang::Keyword::FindIdx> const FindIdx;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Min> const Min;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Max> const Max;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Argmin> const Argmin;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Argmax> const Argmax;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Recur> const Recur;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Unfold> const Unfold;

extern lang::SignatureOp const Op;
extern lang::SignatureOp const Overload;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Bind> const Bind;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Fn> const Fn;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Link> const Link;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Get> const Get;

extern lang::SignatureBinary<::zmbt::lang::Keyword::EnvLoad> const EnvLoad;

extern lang::SignatureBinary<::zmbt::lang::Keyword::EnvStore> const EnvStore;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Any> const Any;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::All> const All;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Saturate> const Saturate;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Pipe> const Pipe;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Tuple> const Tuple;

extern lang::SignatureVariadic<::zmbt::lang::Keyword::Fork> const Fork;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Flip> const Flip;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Dbg> const Dbg;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Dbg> const Debug;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Eval> const Eval;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Try> const Try;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Kwrd> const Kwrd;

extern lang::SignatureUnary<::zmbt::lang::Keyword::Prms> const Prms;
extern lang::SignatureUnary<::zmbt::lang::Keyword::Prms> const Parameters;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Q> const Q;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Q> const C;
extern lang::SignatureBinary<::zmbt::lang::Keyword::Q> const Const;

extern lang::SignatureBinary<::zmbt::lang::Keyword::D> const D;
extern lang::SignatureBinary<::zmbt::lang::Keyword::D> const Default;

extern lang::SignatureErr const Err;
extern lang::SignatureErr const Error;

extern lang::SignatureUnary<::zmbt::lang::Keyword::IsErr> const IsErr;
extern lang::SignatureUnary<::zmbt::lang::Keyword::IsErr> const Iserror;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Assert> const Assert;

extern lang::SignatureBinary<::zmbt::lang::Keyword::Trace> const Trace;

extern lang::SignatureBinary<::zmbt::lang::Keyword::PreProc> const PreProc;

} // namespace expr
} // namespace zmbt

#endif // ZMBT_MAPPING_EXPRESSION_HPP_
```


