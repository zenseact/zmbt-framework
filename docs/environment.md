<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Environment API
===============

*** note
*This section is in progress*
***

Class: `Environment`

*Environment* is basically a sophisticated associative container. It's only purpose is to provide safe and convenient storage for the test data. It does not interact with any other component on its own.

Considering its implementation as a weak singleton, the environment may be viewed as an internal database for the test I/O data, accessible from any part of the test application.

The Environment class methods are organized in the following groups:

- InterfaceRecord API
- Arbitrary Data Management
- Environment Checkout
- Environment Cleanup


## InterfaceRecord API

Method: `InterfaceRecord`

The method creates an extension to the environment that manages the signal records for particular interface. It owns an instance of environment, prolonging its lifetime.
The following methods may be needed in user code:

- stimuli setters (`InjectArgs, InjectReturn`) and getters (`GetInjectionArgs, GetInjectionReturn`)
- **Hook**: This method registers an interface call in the environment, recording the argument values, and returns an injected stimuli or default values.


## Arbitrary global data

- **SetVar, GetVar, GetVarOrDefault, GetVarOrUpdate**: handle serializable variables associated with objects and string keys.
- **SetShared, GetShared**: handle data of arbitrary types, including non-serializable ones. Getter method requires an explicit template parameter to check the type correctness in run-time.


## Thread safety
Every method in the Environment class is designed to be thread-safe. For scenarios requiring transactional logic, the environment can be secured using a RAII lock object that manages a recursive mutex. The relevant methods for this functionality are **Lock**, **TryLock**, and **DeferLock**.


## Environment Lifetime

The Environment class functions as a proxy to the back-end data and is implemented as a *weak singleton*. This implies that the global back-end instance is constructed upon the creation of the first proxy and remains active as long as there are references to it. Once all proxies are released, the back-end instance expires and may be constructed again.

In the context of testing, both the test model and fixture classes hold an *Environment* object, thereby keeping the global back-end instance alive throughout the testing process. *InterfaceRecord* instances extends the environment lifetime too.

All automated test models are designed to reset their data before each test, ensuring that any leaked or preserved objects between tests do not cause issues. However, the data, managed through the Arbitrary Data Management methods, is preserved.


## Environment Cleanup

The clean-up methods are `ResetInterfaceData, ResetInterfaceDataFor, ResetAll, ResetAllFor`.
