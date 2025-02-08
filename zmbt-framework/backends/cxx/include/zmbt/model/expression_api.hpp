/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * app: codegen.expr
 * template: codegen/expr/templates/expression_api.hpp
 *
 * How to update:
 * 1. Update the template
 * 2. Run `python -m codegen` in the project root
 * 3. Commit changes
 */

#ifndef ZMBT_MODEL_EXPRESSION_API_HPP_
#define ZMBT_MODEL_EXPRESSION_API_HPP_

#include "expression_api_signatures.hpp"

namespace zmbt {
namespace expr {


/// \brief No operation
static signature::Const<Keyword::Noop> const Noop;
/// \brief Alias for Noop
static signature::Const<Keyword::Noop> const _;

/// \brief Null value
static signature::Const<Keyword::Null> const Null;

/// \brief Logical true
static signature::Const<Keyword::True> const True;

/// \brief Logical false
static signature::Const<Keyword::False> const False;

/// \brief Pi constant
static signature::Const<Keyword::Pi> const Pi;

/// \brief Euler's number
static signature::Const<Keyword::E> const E;

/// \brief Infinity
static signature::Const<Keyword::Inf> const Inf;

/// \brief Machine epsilon
static signature::Const<Keyword::Eps> const Eps;

/// \brief Not a number
static signature::Const<Keyword::NaN> const NaN;

/// \brief Predicate on boolean transform (aka truthy)
static signature::Unary<Keyword::Bool> const Bool;

/// \brief Predicate on boolean transform (aka falsy)
static signature::Unary<Keyword::Nil> const Nil;

/// \brief Logical complement
static signature::Unary<Keyword::Not> const Not;

/// \brief Negate
static signature::Unary<Keyword::Neg> const Neg;

/// \brief Bitwise not
static signature::Unary<Keyword::BitNot> const BitNot;

/// \brief Square root
static signature::Unary<Keyword::Sqrt> const Sqrt;

/// \brief Sinus
static signature::Unary<Keyword::Sin> const Sin;

/// \brief Cosinus
static signature::Unary<Keyword::Cos> const Cos;

/// \brief Tangens
static signature::Unary<Keyword::Tan> const Tan;

/// \brief Arcsin
static signature::Unary<Keyword::Asin> const Asin;

/// \brief Arccos
static signature::Unary<Keyword::Acos> const Acos;

/// \brief Arctan
static signature::Unary<Keyword::Atan> const Atan;

/// \brief Hyperbolic sin
static signature::Unary<Keyword::Sinh> const Sinh;

/// \brief Hyperbolic cos
static signature::Unary<Keyword::Cosh> const Cosh;

/// \brief Hyperbolic tan
static signature::Unary<Keyword::Tanh> const Tanh;

/// \brief Hyperbolic arcsin
static signature::Unary<Keyword::Asinh> const Asinh;

/// \brief Hyperbolic arccos
static signature::Unary<Keyword::Acosh> const Acosh;

/// \brief Hyperbolic arctan
static signature::Unary<Keyword::Atanh> const Atanh;

/// \brief Exponential (e^x)
static signature::Unary<Keyword::Exp> const Exp;

/// \brief Error function
static signature::Unary<Keyword::Erf> const Erf;

/// \brief Error function complement
static signature::Unary<Keyword::Erfc> const Erfc;

/// \brief Gamma function
static signature::Unary<Keyword::Gamma> const Gamma;

/// \brief Absolute value
static signature::Unary<Keyword::Abs> const Abs;

/// \brief Ceil
static signature::Unary<Keyword::Ceil> const Ceil;

/// \brief Floor
static signature::Unary<Keyword::Floor> const Floor;

/// \brief Sign
static signature::Unary<Keyword::Sign> const Sign;

/// \brief Summation reduction, real param - initial value
/// \details
/// Equivalent to Reduce(Add, ...)
static signature::UnaryParam<Keyword::Sum> const Sum;

/// \brief Multiplication reduction, real param - initial value
/// \details
/// Equivalent to Reduce(Mul, ...)
static signature::UnaryParam<Keyword::Prod> const Prod;

/// \brief Round to nearest integer with optional precision
static signature::UnaryParam<Keyword::Round> const Round;

/// \brief Addition
static signature::Binary<Keyword::Add> const Add;

/// \brief Subtraction
static signature::Binary<Keyword::Sub> const Sub;

/// \brief Reverse subtraction
/// \details
/// Subtraction with reverse arguments, i. e.
///   1. [ ] |-> [y, x] |-> x - y
///   2. [x] |-> [y]    |-> x - y
static signature::Binary<Keyword::SubFrom> const SubFrom;

/// \brief Multiplication
static signature::Binary<Keyword::Mul> const Mul;

/// \brief Division
/// \details
/// 1. [ ] |-> [x, y] |-> x / y
/// 2. [y] |-> [x]    |-> x / y
static signature::Binary<Keyword::Div> const Div;

/// \brief Reverse division
/// \details
/// Division with reverse arguments, i. e.
///   1. [ ] |-> [y, x] |-> x / y
///   2. [x] |-> [y]    |-> x / y
static signature::Binary<Keyword::DivFrom> const DivFrom;

/// \brief To power
/// \details
/// X to power p
///   1. [ ] |-> [x, p] |-> x^p
///   2. [p] |-> [x]    |-> x^p
static signature::Binary<Keyword::Pow> const Pow;

/// \brief Reverse power
/// \details
/// Pow with reverse arguments, i. e.
///   1. [ ] |-> [p, x] |-> x^p
///   2. [x] |-> [p]    |-> x^p
static signature::Binary<Keyword::PowFrom> const PowFrom;

/// \brief Logarithm
/// \details
/// Logarithm with base b:
///   1. [ ] |-> [x, b] |-> log_b(x)
///   2. [b] |-> [x]    |-> log_b(x)
static signature::Binary<Keyword::Log> const Log;

/// \brief Reverse logarithm
/// \details
/// Log with reverse arguments, i. e.
///   1. [ ] |-> [b, x] |-> log_b(x)
///   2. [b] |-> [b]    |-> log_b(x)
static signature::Binary<Keyword::LogFrom> const LogFrom;

/// \brief Modulo
/// \details
/// Modulo of x:
///   1. [ ] |-> [x, m] |-> x % m
///   2. [m] |-> [x]    |-> x % m
static signature::Binary<Keyword::Mod> const Mod;

/// \brief Reverse modulo
/// \details
/// Modulo with reverse arguments, i. e.
///   1. [ ] |-> [b, x] |-> log_b(x)
///   2. [b] |-> [b]    |-> log_b(x)
static signature::Binary<Keyword::ModFrom> const ModFrom;

/// \brief Quotient
/// \details
/// Quotient of x:
///   1. [ ] |-> [x, d] |-> x // d
///   2. [d] |-> [x]    |-> x // d
static signature::Binary<Keyword::Quot> const Quot;

/// \brief Reverse quotient
/// \details
/// Quotient with reverse arguments, i. e.
///   1. [ ] |-> [d, x] |-> x // d
///   2. [x] |-> [d]    |-> x // d
static signature::Binary<Keyword::QuotFrom> const QuotFrom;

/// \brief Bitwise and
static signature::Binary<Keyword::BitAnd> const BitAnd;

/// \brief Bitwise or
static signature::Binary<Keyword::BitOr> const BitOr;

/// \brief Bitwise xor
static signature::Binary<Keyword::BitXor> const BitXor;

/// \brief Bitwise left shift
/// \details
/// 1. [ ] |-> [x, s] |-> x << s
/// 2. [s] |-> [x]    |-> x << s
static signature::Binary<Keyword::BitLshift> const BitLshift;

/// \brief Reverse bitwise left shift
/// \details
/// Bitwise left shift with reverse arguments, i. e.
///   1. [ ] |-> [s, x] |-> x << s
///   2. [x] |-> [s]    |-> x << s
static signature::Binary<Keyword::BitLshiftFrom> const BitLshiftFrom;

/// \brief Bitwise right shift
/// \details
/// 1. [ ] |-> [x, s] |-> x >> s
/// 2. [s] |-> [x]    |-> x >> s
static signature::Binary<Keyword::BitRshift> const BitRshift;

/// \brief Reverse bitwise right shift
/// \details
/// Bitwise right shift with reverse arguments, i. e.
///   1. [ ] |-> [s, x] |-> x >> s
///   2. [x] |-> [s]    |-> x >> s
static signature::Binary<Keyword::BitRshiftFrom> const BitRshiftFrom;

/// \brief Is equal
static signature::Binary<Keyword::Eq> const Eq;

/// \brief Not equal
static signature::Binary<Keyword::Ne> const Ne;

/// \brief Lesser than
static signature::Binary<Keyword::Lt> const Lt;

/// \brief Lesser or equal
static signature::Binary<Keyword::Le> const Le;

/// \brief Greater than
static signature::Binary<Keyword::Gt> const Gt;

/// \brief Greater or equal
static signature::Binary<Keyword::Ge> const Ge;

/// \brief Floating point approximately equal
/// \details
/// Based on numpy.isclose: abs(x - ref) <= (atol + rtol * abs(ref))
static signature::Approx const Approx;
/// \brief Alias for Approx
static signature::Approx const Near;

/// \brief Logical and
/// \details
/// Generic behavior:
///   if first operand is truthy, returns second operand, otherwise first
static signature::Binary<Keyword::And> const And;

/// \brief Logical or
/// \details
/// Generic behavior:
///   if first operand is truthy, returns first operand, second otherwise
static signature::Binary<Keyword::Or> const Or;

/// \brief Equal as set
static signature::BinarySetRhs<Keyword::SetEq> const SetEq;

/// \brief Is subset
static signature::BinarySetRhs<Keyword::Subset> const Subset;

/// \brief Is superset
static signature::BinarySetRhs<Keyword::Superset> const Superset;

/// \brief Is proper subset
static signature::BinarySetRhs<Keyword::ProperSubset> const ProperSubset;

/// \brief Is proper superset
static signature::BinarySetRhs<Keyword::ProperSuperset> const ProperSuperset;

/// \brief Element is in
static signature::BinarySetRhs<Keyword::In> const In;

/// \brief Element is not in
static signature::BinarySetRhs<Keyword::NotIn> const NotIn;

/// \brief Contains element
static signature::Binary<Keyword::Ni> const Ni;
/// \brief Alias for Ni
static signature::Binary<Keyword::Ni> const Contains;

/// \brief Not contains element
static signature::Binary<Keyword::NotNi> const NotNi;

/// \brief Set union
static signature::BinarySetRhs<Keyword::Union> const Union;

/// \brief Set intersection
static signature::BinarySetRhs<Keyword::Intersect> const Intersect;

/// \brief Set difference
static signature::BinarySetRhs<Keyword::Diff> const Diff;

/// \brief Reverse set difference
static signature::BinarySetRhs<Keyword::DiffFrom> const DiffFrom;

/// \brief Concatenate sequences
static signature::Variadic<Keyword::Concat> const Concat;

/// \brief Cartesian product
static signature::Variadic<Keyword::Cartesian> const Cartesian;

/// \brief Envelop zero or more values in list
static signature::Variadic<Keyword::List> const List;

/// \brief Transpose multidimensional list, turning rows into columns
/// \details
/// May be used to zip sequences of equal length.
/// Example:
///   [[1, 2, 3], [4, 5, 6]] -> [[1, 4], [2, 5], [3, 6]]
static signature::Unary<Keyword::Transp> const Transp;

/// \brief Filter unique elements
static signature::Unary<Keyword::Uniques> const Uniques;

/// \brief Identity function
static signature::Unary<Keyword::Id> const Id;

/// \brief Set cardinality (uniques count)
static signature::Unary<Keyword::Card> const Card;

/// \brief Sequence size
static signature::Unary<Keyword::Size> const Size;

/// \brief Sliding-window iteration
/// \details
/// Sliding window iteration by specified window width.
static signature::Binary<Keyword::Slide> const Slide;

/// \brief Striding iteration
/// \details
/// Striding iteration by specified step width.
static signature::Binary<Keyword::Stride> const Stride;

/// \brief Repeat value in list
static signature::Binary<Keyword::Repeat> const Repeat;

/// \brief Regular expression match
/// \details
/// If input is not a string, match it's serialized form.
static signature::Binary<Keyword::Re> const Re;
/// \brief Alias for Re
static signature::Binary<Keyword::Re> const Regex;

/// \brief Transform json value with given query
/// \details
/// Query evaluation rules:
/// ```
///   1. q: int  |-> x: list  |-> x at index q (negative -> reverse)
///   2. q: str  |-> x: any   |-> x at JSON Pointer q
///   3. q: list |-> x: any   |-> [x at q1, x at q2, ...]
///   4. {"key": q1, "$q2": q3, ...} |-> x: any |->
///       { "key1": x at q1, "$(x at q2)": x at q3, ...}
/// ```
static signature::Binary<Keyword::At> const At;

/// \brief Apply param expr to every element of sequence
static signature::HiOrd<Keyword::Map> const Map;

/// \brief Filter sequence by predicate param
static signature::HiOrd<Keyword::Filter> const Filter;

/// \brief Count matches by predicate param
static signature::HiOrd<Keyword::Count> const Count;

/// \brief Sort list by comparison parameter expr
static signature::HiOrd<Keyword::Sort> const Sort;

/// \brief Min value
static signature::HiOrd<Keyword::Min> const Min;

/// \brief Max value
static signature::HiOrd<Keyword::Max> const Max;

/// \brief Min value index
static signature::HiOrd<Keyword::Argmin> const Argmin;

/// \brief Max value index
static signature::HiOrd<Keyword::Argmax> const Argmax;

/// \brief Apply recursion to parameter expr
static signature::HiOrdParam<Keyword::Recur> const Recur;

/// \brief Apply expr to literal param
static signature::HiOrdParam<Keyword::Apply> const Apply;

/// \brief Bind parameters
static signature::HiOrdParam<Keyword::Bind> const Bind;

/// \brief Reduce sequence with binary operator and optional initial value
/// \details
/// If initial value is not provided, first element is used.
/// If sequence is empty and initial value is not provided, error is raised.
static signature::HiOrdParamOpt<Keyword::Reduce> const Reduce;

/// \brief Match any predicate
static signature::HiOrdVariadic<Keyword::Any> const Any;

/// \brief Match all predicates
static signature::HiOrdVariadic<Keyword::All> const All;

/// \brief Saturate matches in order
static signature::HiOrdVariadic<Keyword::Saturate> const Saturate;

/// \brief Compose functions
static signature::HiOrdVariadic<Keyword::Compose> const Compose;

} // namespace expr
} // namespace zmbt

#endif // ZMBT_MAPPING_EXPRESSION_HPP_
