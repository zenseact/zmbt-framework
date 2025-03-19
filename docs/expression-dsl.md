<!-- (c) Copyright 2025 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Expression DSL {#expression-dsl}

ZMBT utilizes an embedded functional programming language to simplify the test data manipulation and mathicng.

The language resides in zmbt::expr namespace and consist of keywords that can be parametrized and combined in a single expression with function composition or structural joins, defining JSON to JSON transformations.

``` plantuml
@startebnf expression_dsl_syntax
title Expression DSL syntax

!define LB "("
!define RB ")"
!define CM ","

!define REST(x) {CM, x}-
!define ONEORMORE(x) x, REST(x)

Expression = Literal | (Keyword, [Parameters]) | Compose | Pack | Apply;
Parameters = LB, ONEORMORE(Expression), RB;
Compose = Expression, '|', Expression;
Pack = Expression, '&', Expression;
Apply = Expression, '<<', Literal;

@endebnf
```


``` js
Filter(At(1) | Eq(false)) | Map(At(0))
    >> [["lol",true],["kek",true],["foo",false],["bar",false]]
    = ["foo","bar"]
```