<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Zenseact MBT Framework
==================

C++ Model-based Testing Framework

The framework conceptualizes a software test as a mathematical problem.
It enables users to define test parameters and goals using a declarative
modeling syntax, upon which automated verification procedures are applied.

## Quick examples

Simple tabular test:
```c++
auto sum = [](double x, double y){ return x + y; };

SignalMapping("Simple test with non-scalar input")
.OnTrigger(sum)
    .InjectTo  (sum)
    .ObserveOn (sum)
.Test
    ( { 2,   2},    4 )
    ( { 2,  -2},    0 )
    ( {42, 0.1}, 42.1 )
;
```

Example of batch test with mock, generating function on input, and range matcher on output:
```c++
struct Mock {
    double produce() {
        return InterfaceRecord(&Mock::produce).Hook(); //(1)
    }
} mock {};

auto sut = [&mock]() { return mock.produce(); };

SignalMapping("Keep clause")
.OnTrigger(sut) .Repeat(250)
    .InjectTo (&Mock::produce)  .Keep(Add(1)) //(2)
    .ObserveOn(sut).CallRange() .Expect(Slide(2) | Each(Lt)) //(3)
;
```

1. Rerouting mock call to ZMBT environment
1. Generating function `Add(1)` accept index call, producing `[1,2,3...]` for mock call series
2. Sliding window matcher asserting increasing delta

## Model-Based testing

[Model-based testing](https://en.wikipedia.org/wiki/Model-based_testing) (MBT) is an application of model-based design for software verification, potentially automating test generation and execution. This approach provides a high-level, abstract representation of the system under test, simplifying test interpretation and management.

MBT is recognized as a high-level verification technique, typically applied during the design stage for model consistency checks or during a later development stages for system testing. The ZMBT framework implements this concept early in development, focusing primarily on unit and integration tests.

The value of this approach in software verification includes:

- **Quality**: The framework enforces a model-driven approach to testing, ensuring early identification of design inconsistencies and improving overall code quality by aligning testing closely with the system’s architecture from the outset.
- **Traceability**: Model-driven DSL connects the test script connect with specifications, enhancing the alignment between verification and design objectives.
- **Readability**: This is evident in the rapid creation and review of test cases, and the use of simple syntax makes the test code easily comprehensible, even for non-C++ engineers.


!!! warning "Development Status"

    The project is currently in active development, with core features functional in a prototype state. For the initial release, we aim to refine the implementation and address technical debt, including the development of the test runner application, enhanced logging and reporting capabilities, and comprehensive documentation and verification.

    In the long term, we plan to introduce Finite-State Automata test models that will operate on the same infrastructure as Signal Mapping models.

    The present documentation reflects the work in progress and is not yet intended for production use.
