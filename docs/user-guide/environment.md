<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Environment API
===============


:construction: *This document is in progress* :construction:

Class: `zmbt::Environment`

*Environment* is basically a sophisticated associative container. It's only purpose is to provide safe and convenient storage for the test data. It does not interact with any other component on its own, but test model runners may use it for interaction with triggers and mocks.

Considering its implementation as a weak singleton, the environment may be viewed as an internal database for the test I/O data, accessible from any part of the test application. The lifetime of the environment affects observability of the manageable effects - it is recommended to keep an instance of `zmbt::Environment` in the test fixture, allowing the RAII to reset the environment between tests.

The Environment class methods are organized in the following groups:

- InterfaceRecord API
- Arbitrary Data Management
- Environment Checkout
- Environment Cleanup


## InterfaceRecord API

Method: `zmbt::InterfaceRecord`

The method creates an extension to the environment that manages the signal records for particular interface. It owns an instance of environment, prolonging its lifetime.

The following methods may be needed in user code:

- Mock call rerouting: [`Hook(args...)`](\ref zmbt::Environment::TypedInterfaceHandle::Hook)
    - This method registers an interface call in the environment, recording the argument values, and returns an injected stimuli or default values. 
    <!-- See [examples](#signal-mapping-model-overview-mocks). -->

- stimuli setters:
[`InjectArgs(value, json_ptr = "", nofcall = -1)`](\ref zmbt::Environment::InterfaceHandle::InjectArgs),
[`InjectReturn(value, json_ptr = "", nofcall = -1)`](\ref zmbt::Environment::InterfaceHandle::InjectReturn)
    - Set corresponding value subsignal at specified JSON pointer and nofcall. The nofcall parameter corresponds to the interface call counter, where -1 is equivalent to `last value`, i.e. default.

- stimuli getters:
[`GetInjectionArgs(json_ptr, nofcall = -1)`](\ref zmbt::Environment::InterfaceHandle::GetInjectionArgs),
[`GetInjectionReturn(json_ptr, nofcall = -1)`](\ref zmbt::Environment::InterfaceHandle::GetInjectionReturn)
    - Set corresponding values at JSON pointer and nofcall. Negative nofcall parameters used for reverse indexation.


## Arbitrary data management

A set of interfaces not managed by test runners directly, but allowing the user to utilize safe storage for globally-accessed data.

JSON-serializable variables managed by:
- `zmbt::Environment::SetVar`
- `zmbt::Environment::GetVar`
- `zmbt::Environment::GetVarOrDefault`
- `zmbt::Environment::GetVarOrUpdate`

Interfaces managing non-serializable data:
- `zmbt::Environment::SetShared`
- `zmbt::Environment::GetShared`: requires an explicit template parameter to check the type correctness in run-time.


## Thread safety
Every method in the Environment class is designed to be thread-safe. For scenarios requiring transactional logic, the environment can be secured using a RAII lock object that manages a recursive mutex. The relevant methods for this functionality are
 - `zmbt::Environment::Lock`
 - `zmbt::Environment::TryLock`
 - `zmbt::Environment::DeferLock`


## Environment Lifetime

The Environment class functions as a proxy to the back-end data and is implemented as a *weak singleton*. This implies that the global back-end instance is constructed upon the creation of the first proxy and remains active as long as there are references to it. Once all proxies are released, the back-end instance expires and may be constructed again.

In the context of testing, both the test model and fixture classes hold an *Environment* object, thereby keeping the global back-end instance alive throughout the testing process. *InterfaceRecord* instances extends the environment lifetime too.

All automated test models are designed to reset their data before each test, ensuring that any leaked or preserved objects between tests do not cause issues. However, the data, managed through the Arbitrary Data Management methods, is preserved.


## Environment Cleanup

The clean-up methods are
 - `zmbt::Environment::ResetInterfaceData`
 - `zmbt::Environment::ResetInterfaceDataFor`
 - `zmbt::Environment::ResetAll`
 - `zmbt::Environment::ResetAllFor`
