/**
 * \file
 * \copyright (c) Copyright 2024 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/keyword.hpp`
 * 2. Update the keyword data at `templates/expr/data.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:keyword_hpp > zmbt-framework/backends/cxx/include/zmbt/model/keyword.hpp`
 * 4. Commit changes
 */

#ifndef ZMBT_MODEL_keyword.hpp_
#define ZMBT_MODEL_keyword.hpp_

#include <boost/json.hpp>

#include "zmbt/reflect.hpp"

#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {

/// ZMBT Expression keyword
enum class Keyword
{

    /// Identity function
    Id,

    /// No operation
    Noop,

    /// Logical true
    True,

    /// Logical false
    False,

    /// Null value
    Null,

    /// Pi constant
    Pi,

    /// Euler's number
    E,

    /// Infinity
    Inf,

    /// Machine epsilon
    Eps,

    /// Not a number
    NaN,

    /// Predicate on boolean transform (aka Truthy)
    Bool,

    /// Predicate on boolean transform (aka Falsy)
    Nil,

    /// Logical complement
    Not,

    /// Logical conjunction
    And,

    /// Logical disjunction
    Or,

    /// Logical exclusive or
    Xor,

    /// Is equal
    Eq,

    /// Not equal
    Ne,

    /// Lesser than
    Lt,

    /// Lesser or equal
    Le,

    /// Greater than
    Gt,

    /// Greater or equal
    Ge,

    /// Contains element
    Ni,

    /// Not contains element
    NotNi,

    /// Equal as set
    SetEq,

    /// Is subset
    Subset,

    /// is superset
    Superset,

    /// Is proper subset
    ProperSubset,

    /// Is proper superset
    ProperSuperset,

    /// Element is in
    In,

    /// Element is not in
    NotIn,

    /// Negate
    Neg,

    /// Absolute value
    Abs,

    /// Sign
    Sign,

    /// Ceil
    Ceil,

    /// Floor
    Floor,

    /// Round
    Round,

    /// Add
    Add,

    /// Substract
    Sub,

    /// Multiply
    Mul,

    /// Divide
    Div,

    /// To power
    Pow,

    /// Logarithm
    Log,

    /// Modulo
    Mod,

    /// Quotient
    Quot,

    /// Square root
    Sqrt,

    /// Sinus
    Sin,

    /// Cosinus
    Cos,

    /// Tangens
    Tan,

    /// Arcsin
    Asin,

    /// Arccos
    Acos,

    /// Arctan
    Atan,

    /// Hyperbolic sin
    Sinh,

    /// Hyperbolic cos
    Cosh,

    /// Hyperbolic tan
    Tanh,

    /// Hyperbolic  arcsin
    Asinh,

    /// Hyperbolic arccos
    Acosh,

    /// Hyperbolic arctan
    Atanh,

    /// Exponential (e^x)
    Exp,

    /// Error function
    Erf,

    /// Error function complement
    Erfc,

    /// Gamma function
    Gamma,

    /// Bitwise not
    BitNot,

    /// Bitwise and
    BitAnd,

    /// Bitwise or
    BitOr,

    /// Bitwise xor
    BitXor,

    /// Bitwise left shift
    BitLshift,

    /// Bitwise right shift
    BitRshift,

    /// Apply param fn to every element of sequence
    Map,

    /// Filter sequence by predicate param
    Filter,

    /// Compose functions
    Compose,

    /// apply recursion to parameter fn
    Recur,

    /// Apply fn to literal param
    Apply,

    /// Bind parameters
    Partial,

    /// Reduce
    Reduce,

    /// Concatenate sequences
    Concat,

    /// Set union
    Union,

    /// Set intersection
    Intersect,

    /// Set difference
    Diff,

    /// Cartesian product
    Cartesian,

    /// Transpose 2-d array, turning rows into columns
    Transp,

    /// Sequence to set
    Set,

    /// Envelop value in array (equivalent to Repeat(1))
    List,

    /// Sliding-window iteration
    Slide,

    /// Striding iteration
    Stride,

    /// Slice sequence by "start:stop:step"
    Slice,

    /// Sort array by comparison parameter fn
    Sort,

    /// Repeat value in array
    Repeat,

    /// Transform value with query pseudo-language
    At,

    /// Summation reduction, real param - initial value
    Sum,

    /// Multiplication reduction, real param - initial value
    Prod,

    /// Set cardinality (uniques count)
    Card,

    /// Sequence size
    Size,

    /// Count matches
    Count,

    /// Min value
    Min,

    /// Max value
    Max,

    /// Min value index
    Argmin,

    /// Max value index
    Argmax,

    /// Match any predicate
    Any,

    /// Match all predicates
    All,

    /// Saturate matches in order
    Saturate,

    /// Regular expression match
    Re,

    /// Approximately equal
    Approx,
    /// Internal utility value
    Undefined
};

// Boost JSON conversion from Keyword
void tag_invoke(boost::json::value_from_tag const&, boost::json::value&, Keyword const& t);

// Boost JSON conversion to Keyword
Keyword tag_invoke(boost::json::value_to_tag<Keyword> const&, boost::json::value const&);


} // namespace zmbt



#endif // ZMBT_MAPPING_keyword.hpp_

