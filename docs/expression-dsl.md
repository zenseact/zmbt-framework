<!-- (c) Copyright 2025 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Expression DSL {#expression-dsl}

* This section is in progress *

ZMBT utilizes an embedded functional programming language to simplify the test data manipulation and mathicng.

The language resides in zmbt::expr namespace and consist of keywords that can be parametrized and combined in a single expression with function composition or structural joins, defining JSON to JSON transformations.

![image](expression_dsl_syntax.png)
<!-- ``` plantuml
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
``` -->


## Debug evaluation

Complex expressions evaluation

``` cpp
Expression::EvalConfig cfg{};
cfg.log = Expression::EvalLog::make();

auto const f = Reduce(Add) & Size | Div;
auto const x = L{1,2,3,42.5};
f.eval(x, cfg);
std::cerr << cfg.log << '\n';
```

Produced output:
``` c
         ┌── ":add"([1,2]) = 3
         ├── ":add"([3,3]) = 6
         ├── ":add"([6,4.25E1]) = 4.85E1
      ┌── {":reduce":":add"}([1,2,3,4.25E1]) = 4.85E1
      ├── ":size"([1,2,3,4.25E1]) = 4
   ┌── {":pack":[{":reduce":":add"},":size"]}([1,2,3,4.25E1]) = [4.85E1,4]
   ├── ":div"([4.85E1,4]) = 1.2125E1
□  {":compose":[":div",{":pack":[{":reduce":":add"},":size"]}]}([1,2,3,4.25E1]) = 1.2125E1
```

In model tests, the evaluation stack is logged on failing tests.

