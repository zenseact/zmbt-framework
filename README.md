<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Zenseact MBT Framework
==================

C++ Model-based Testing Framework

The framework conceptualizes a software test as a mathematical problem.
It enables users to define test parameters and goals using a declarative
modeling syntax, upon which automated verification procedures are applied.

[![Test and Scan Backends](https://github.com/zenseact/zmbt-framework/actions/workflows/test-and-scan-backends.yml/badge.svg)](https://github.com/zenseact/zmbt-framework/actions/workflows/test-and-scan-backends.yml)
[![Test and Scan Python](https://github.com/zenseact/zmbt-framework/actions/workflows/test-and-scan-python.yml/badge.svg)](https://github.com/zenseact/zmbt-framework/actions/workflows/test-and-scan-python.yml)



## Dependencies

 - C++ Standard >= 14
 - [Boost](https://www.boost.org/) >= 1.84


## Development Status

The project is currently in active development, with core features functional in a prototype state. For the initial release, we aim to refine the implementation and address technical debt, including the development of the test runner application, enhanced logging and reporting capabilities, and comprehensive documentation and verification.

In the long term, we plan to introduce Finite-State Automata test models that will operate on the same infrastructure as Signal Mapping models.


## Build and test

 - `./run-dev-container.sh <compiler>` - run dev container shell. Supported options - `clang-17` (default), `gcc`.
 - `python -m codegen` - run code autogeneration
 - `bazel test //...` - compile and run tests
 - `./docs/generate_doxy.sh` - generate docs


## How to use

For Bazel prejects:

```Starlark
# WORKSPACE
zmbt_tag = ...

http_archive(
    name = "zmbt",
    sha256 = ...,
    strip_prefix = "zmbt-framework-{}".format(zmbt_tag),
    urls = [
        "https://github.com/zenseact/zmbt-framework/archive/{}.tar.gz".format(zmbt_tag),
    ],
)
```

Support for CMake will be included in the [initial release](https://github.com/zenseact/zmbt-framework/milestone/3).
