<!-- (c) Copyright 2025 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Expression DSL {#expression-dsl}

[TOC]

* This section is in progress *

ZMBT utilizes an embedded functional programming language to simplify the test data manipulation and mathicng.

The language resides in zmbt::expr namespace and consist of keywords that can be parametrized and combined in a single expression with function composition or structural joins, defining pure JSON-to-JSON transformations.

Test model runners are evaluating these expressions against test IO data, using them both
as input generators and as matchers.

## Syntax {#expression-dsl-syntax}

There expression keywords compose five groups partitioned by the resulting arity,
counting both design-time and evaluation-time parameters.
The general syntax is either `Keyword` or `Keyword(<Expression list>...)`, where the second form
is a design-time parametrisation, but not yet an evaluation.
Both forms yileds a `zmbt::Expression` with [eval](\ref zmbt::Expression::eval) method, used by the framework in run-time.

### Keyword forms

|Form    | Function Type                                             |Example                               |
|--------|-----------------------------------------------------------|--------------------------------------|
|Const   |\f$ E^C \mapsto (x \mapsto C)                           \f$|`Pi >> null = 3.1415...               `|
|Unary   |\f$ E^f \mapsto (x \mapsto f(x))                        \f$|`Sin >>` \f$\frac{\pi}{2}\f$ `= 1    `|
|Binary₁ |\f$ E^*    \mapsto ([x, y] \mapsto x * y      )         \f$|`Add >> [2,2] = 4                     `|
|Binary₂ |\f$ E^*    \mapsto (x      \mapsto x * default)         \f$|`Max >> [-1,1] = 1                    `|
|Binary₃ |\f$ E^*(y) \mapsto (x      \mapsto x * y      )         \f$|`Eq(42) >> 13 = ⊥                     `|
|Ternary |\f$ E^f(a, b) \mapsto (x \mapsto f(a, b)(x))            \f$|`Recur(Pow(2), 4) >> 4 = 65536        `|
|Variadic|\f$ E^f(a,b,c,...) \mapsto (x \mapsto f(a,b,c,...)(x))  \f$|`All(Gt(5), Le(6)) >> 6 = ⊤           `|
|Literal₁|Evaluated as is where a value is expected                  |`Serialize >> Eq(0) ≢ Serialize >> 0  `|
|Literal₂|Evaluated as `Eq(value)` where a function is expected      |`Filter(42) ≡ Filter(Eq(42))          `|



The **Binary₂** form is used in place of **Binary₁** for expressions that have default value defined,
e.g. `Max(Id)` is equivalent to just `Max`, where the identity expression [Id](\ref zmbt::expr::Id)
is a default parameter (key function).

In the **Binary₃** form the expression parameter is used as the right-hand side operand,
similarly to GoogleTest matchers.

Most of the non-commutative binary operators has a counterpart expression `*From`,
which is evaluated under the same rules but with operands swapped. For other cases,
the composition with [Reverse](\ref zmbt::expr::Reverse) can be utilized in the **Binary₁** form.


### Infix operators

Expression class has overloading for several operators to simplify certain high-order keywords.

|Operator    |Full form                                  | Description|
|----        |----                                       |----------- |
|`E << x`    |[Apply(E, x)](\ref zmbt::expr::Apply)      | bind run-time x at design time |
|`E >> x`    |[E.eval(x)](\ref zmbt::Expression::eval)   | Evaluate expression |
|`E1 & E2`   |[Pack(E1, E2)](\ref zmbt::expr::Pack)      | evaluate and pack results into an array |
|`E1` \| `E2`|[Compose(E2, E1)](\ref zmbt::expr::Compose)| compose expessions |

## Debug evaluation {#expression-dsl-debug}

Complex expressions evaluation

``` c++
Expression::EvalConfig cfg{};
cfg.log = Expression::EvalLog::make();

auto const f = Reduce(Add) & Size | Div;
auto const x = L{1,2,3,42.5};
f.eval(x, cfg);
std::cerr << cfg.log << '\n';
```

Produced output is printed bottom-up in order of evaluation:
```
         ┌── ":add"([1,2]) = 3
         ├── ":add"([3,3]) = 6
         ├── ":add"([6,4.25E1]) = 4.85E1
      ┌── {":reduce":":add"}([1,2,3,4.25E1]) = 4.85E1
      ├── ":size"([1,2,3,4.25E1]) = 4
   ┌── {":pack":[{":reduce":":add"},":size"]}([1,2,3,4.25E1]) = [4.85E1,4]
   ├── ":div"([4.85E1,4]) = 1.2125E1
□  {":compose":[":div",{":pack":[{":reduce":":add"},":size"]}]}([1,2,3,4.25E1]) = 1.2125E1
```
Log lines are formatted as `f(x) = result`, and connected with line-drawing to show the expression terms hierarchy.

In model tests, the evaluation stack is logged on failing tests.
For the bulky log messages the elements are trimmed with `...` while trying to keep evaluation result visible:
``` json
{":compose":[":div",{":pack":[{":reduce":":add"},":size"]}]}([1,2,3,...) = 5
```
For the complete log data refer to JSON log.

## Grammar

![image](expression_dsl_syntax.png)


<!-- plantuml
@startebnf expression_dsl_syntax
title Expression DSL syntax

!define LB "("
!define RB ")"
!define CM ","

!define REST(x) {CM, x}-
!define ONEORMORE(x) x, REST(x)

Expression = Literal | (Keyword, [Parameters]) | Compose | Pack | Apply;
Parameters = (LB, Expression, RB)
    | (LB, Expression, CM, Expression, RB)
    | (LB, ONEORMORE(Expression), RB);


Compose = Expression, '|', Expression;
Pack = Expression, '&', Expression;
Apply = Expression, '<<', Literal;

@endebnf
-->
