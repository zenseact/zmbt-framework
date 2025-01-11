<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Zenseact MBT Framework
==================

C++ Model-based Testing Framework

The framework conceptualizes a software test as a mathematical problem.
It enables users to define test parameters and goals using a declarative
modeling syntax, upon which automated verification procedures are applied.


## Model-Based testing

[Model-based testing](https://en.wikipedia.org/wiki/Model-based_testing) (MBT) is an application of model-based design for software verification, potentially automating test generation and execution. This approach provides a high-level, abstract representation of the system under test, simplifying test interpretation and management.

MBT is recognized as a high-level verification technique, typically applied during the design stage for model consistency checks or during a later development stages for system testing. The ZMBT framework implements this concept early in development, focusing primarily on unit tests.

The value of this approach in software verification includes:

- **Quality**: The framework enforces a model-driven approach to testing, ensuring early identification of design inconsistencies and improving overall code quality by aligning testing closely with the system’s architecture from the outset.
- **Traceability**: Test script directly reflects the test specification, enhancing the alignment between testing and design objectives.
- **Readability**: This is evident in the rapid creation and review of test cases, and the use of simple syntax makes the test code easily comprehensible, even for non-C++ engineers.

Read next: [Signal Mapping model](docs/signal-mapping.md).


# Dependencies

 - C++ Standard >= 14
 - [Boost](https://www.boost.org/) >= 1.84
