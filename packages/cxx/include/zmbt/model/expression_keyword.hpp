/**
 * \file
 * \copyright (c) Copyright 2024 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/expression_keyword.hpp`
 * 2. Update the keyword data at `templates/expr/data.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:expression_keyword_hpp > packages/cxx/include/zmbt/model/expression_keyword.hpp`
 * 4. Commit changes
 */

#ifndef ZMBT_MODEL_EXPRESSION_KEYWORD_HPP_
#define ZMBT_MODEL_EXPRESSION_KEYWORD_HPP_

#include "zmbt/reflect.hpp"

#ifndef ZMBT_KEYWORD_PREFIX
    #define ZMBT_KEYWORD_PREFIX ":"
#endif


namespace zmbt {


enum class ExpressionKeyword
{

    /// Identity function
    Id,

    /// No operation
    Noop,

    /// logical constant true
    True,

    /// logical constant false
    False,

    /// constant null-value
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
    /// 
    /// Evaluation rules:
    ///   1. int    q |-> x |-> array x at index q (negative -> reverse)
    ///   2. str    q |-> x |-> value x at JSON Pointer q
    ///   3. array  [q1, q2, ...] |-> x |-> [x at q1, x at q2, ...]
    ///   4. object {"key": q1, "$q2": q3, ...} |-> x |-> {"key1": x at q1, "$(x at q2)": x at q3, ...}
    /// 
    /// Optional second parameter - predicate P:
    ///   At(q, P) <==> Compose(P, At(q))
    /// 
    At,

    /// Summation reduction, real param - initial value
    /// 
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


template<>
struct reflect::custom_serialization<ExpressionKeyword> {

    static boost::json::value json_from(ExpressionKeyword const& kw);
    static ExpressionKeyword dejsonize(boost::json::value const& v);
};

} // namespace zmbt



#endif // ZMBT_MAPPING_EXPRESSION_KEYWORD_HPP_
