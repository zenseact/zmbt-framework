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
[![Generate API Reference](https://github.com/zenseact/zmbt-framework/actions/workflows/publish-docs.yml/badge.svg)](https://github.com/zenseact/zmbt-framework/actions/workflows/publish-docs.yml)


## Model-Based testing

[Model-based testing](https://en.wikipedia.org/wiki/Model-based_testing) (MBT) is an application of model-based design for software verification, potentially automating test generation and execution. This approach provides a high-level, abstract representation of the system under test, simplifying test interpretation and management.

MBT is recognized as a high-level verification technique, typically applied during the design stage for model consistency checks or during a later development stages for system testing. The ZMBT framework implements this concept early in development, focusing primarily on unit tests.

The value of this approach in software verification includes:

- **Quality**: The framework enforces a model-driven approach to testing, ensuring early identification of design inconsistencies and improving overall code quality by aligning testing closely with the system’s architecture from the outset.
- **Traceability**: Test script directly reflects the test specification, enhancing the alignment between testing and design objectives.
- **Readability**: This is evident in the rapid creation and review of test cases, and the use of simple syntax makes the test code easily comprehensible, even for non-C++ engineers.

Read next: [Signal Mapping model](docs/signal-mapping.md).


## Dependencies

 - C++ Standard >= 14
 - [Boost](https://www.boost.org/) >= 1.84


## Development Status

The project is currently in active development, with core features functional in a prototype state. For the initial release, we aim to refine the implementation and address technical debt, including the development of the test runner application, enhanced logging and reporting capabilities, and comprehensive documentation and verification.

In the long term, we plan to introduce Finite-State Automata test models that will operate on the same infrastructure as Signal Mapping models.


## Build and test

1. [Install Bazel](https://github.com/bazelbuild/bazel/releases/tag/7.4.1)
2. (optional) Install ccache and doxygen: `sudo apt update && sudo install ccache doxygen`
3. (optional) Install codegen dependencies: `python -m pip install wheezy.template ruamel.yaml`
3. Run `bazel query //...` to list the targets
4. Run `bazel test //...` to compile and run tests
5. (Linux) Run `./docs/generate_doxy.sh` to generate API Reference with Doxygen (see also: [#6](https://github.com/zenseact/zmbt-framework/issues/6))

Alternatively, run tests in Docker with `test/linux-test-backend-cxx.sh <compiler>`
(supported compiler options - `gcc`, `clang-17`).

### Linux setup

With local builds you may face errors related to ccache, depending on it's configuration (see `cchache -p`).
It can be solved by setting the ccache `temporary_dir` to `/run/user/1000/ccache-tmp` (check the permissions)
to enable local ccache in Bazel sandbox mode, or by passing `--spawn_strategy=standalone` to bazel commands to turn off the sandboxing. The problem should be addressed in [#5 issue](https://github.com/zenseact/zmbt-framework/issues/5).


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
