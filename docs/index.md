<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Zenseact MBT Framework
==================

C++ Model-based Testing Framework

The framework conceptualizes a software test as a mathematical problem.
It enables users to define test parameters and goals using a declarative
modeling syntax, upon which automated verification procedures are applied.

!!! warning "Development Status"

    The project is currently in active development, with core features functional in a prototype state. For the initial release, we aim to refine the implementation and address technical debt, including the development of the test runner application, enhanced logging and reporting capabilities, and comprehensive documentation and verification.

    In the long term, we plan to introduce Finite-State Automata test models that will operate on the same infrastructure as Signal Mapping models.

    The present documentation reflects the work in progress and is not yet intended for production use.


## Quick example

```c++
auto sum = [](int x, int y){ return x + y; };

SignalMapping("test int summation")
.OnTrigger(sum)
    .InjectTo  (sum)
    .ObserveOn (sum)
.Test
    ( {2, 2}, 4 )
    ( {2,-2}, 0 )
;
```

## Model-Based testing

[Model-based testing](https://en.wikipedia.org/wiki/Model-based_testing) (MBT) is an application of model-based design for software verification, potentially automating test generation and execution. This approach provides a high-level, abstract representation of the system under test, simplifying test interpretation and management.

MBT is recognized as a high-level verification technique, typically applied during the design stage for model consistency checks or during a later development stages for system testing. The ZMBT framework implements this concept early in development, focusing primarily on unit and integration tests.

The value of this approach in software verification includes:

- **Quality**: The framework enforces a model-driven approach to testing, ensuring early identification of design inconsistencies and improving overall code quality by aligning testing closely with the system’s architecture from the outset.
- **Traceability**: Model-driven DSL connects the test script connect with specifications, enhancing the alignment between verification and design objectives.
- **Readability**: This is evident in the rapid creation and review of test cases, and the use of simple syntax makes the test code easily comprehensible, even for non-C++ engineers.
