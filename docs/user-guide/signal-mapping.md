<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Signal Mapping model

The ***Signal Mapping*** model represents the system under test (SUT) as a pure
mathematical mapping, abstracting away side effects and explicitly defining the
*role* of each test parameter. This contrasts with traditional scenario-based unit testing methods,
which often mix the definition of test goals with implementation details within a single workflow.

The following guide illustrates the model DSL using code examples from [examples/cxx/signal-mapping-demo.cpp](../examples/cxx/signal-mapping-demo.cpp). For complete documentation, refer to the [Signal Mapping DSL Reference](/dsl-reference/signal-mapping.md).


--8<-- "docs/examples/cxx/signal-mapping-demo.cpp:10:"


## Test Procedure

The following procedure is applied to each model instance
obtained from the parameter resolver over each test vector:

1. Reset the environment (drop all IO test data).
2. Execute pre-run tasks.
3. Inject the stimuli from the test vector into the environment.
4. Execute the trigger.
5. Observe the responses captured in the environment and verify them against expectations.
6. Execute post-run tasks.
7. Report the results.
