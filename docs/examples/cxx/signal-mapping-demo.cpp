/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
/*


**Signal Mapping** models the interaction with the system under test (SUT) as a trace-level mapping
from generated input sequences to observable outputs, evaluated via matchers.
While the SUT may retain internal state or side effects, the test model abstracts it as a function over time,
capturing only what is exposed through interfaces.

Instead of embedding logic in procedural test code, the framework defines inputs, outputs,
and expectations as condition pipes — structured, composable expressions tied to interface contracts.
It shifts focus from “what happens when” to “what is observed where”, utilizing powerful and expressive
generators and matchers, and structural introspection of the test model.

Its embedded DSL enforces valid composition rules and eliminates ambiguity, guiding users toward correct-by-construction
test designs and making misconfigurations easier to catch — often at compile time.

The following guide illustrates the model DSL using code examples from
[examples/cxx/signal-mapping-demo.cpp](../examples/cxx/signal-mapping-demo.cpp).
For complete documentation, refer to the [Signal Mapping DSL Reference](../dsl-reference/signal-mapping.md).


## Quick Start

### Requirements

- *C++ 14* or later
- *Boost C++ 1.84* or later

```c++
*/
#include <boost/test/unit_test.hpp> //(1)
#include <zenseact-mbt.hpp>

namespace utf = boost::unit_test;
using namespace zmbt::api;  //(2)
using namespace zmbt::expr; //(3)
/*
```

1. These examples use Boost.Test as a host framework, but ZMBT can be embedded into any other. A standalone suite runner is also in development.
2. Main testing API namespace
3. Expression Language namespace



### Minimal Example

```c++
*/
BOOST_AUTO_TEST_CASE(QuickExample)
{
    ZMBT_LOG_JSON(INFO) << "Hello, World! JSON";
    ZMBT_LOG(DEBUG) << "Hello, World Debug!";
    auto sum = [](int x, int y){ return x + y; };

    SignalMapping("Simple function test")
    .OnTrigger(sum)
        .At(sum) .Args(0)  .Inject()
        .At(sum) .Args(1)  .Inject()
        .At(sum) .Return() .Expect()
    .Test
        ( 2,  2,  4 )
        ( 2, -2,  0 )
    .Description("inject x and y and check returning sum")
    ;
}
/*
```

This example defines a basic signal mapping model with two inputs and one output.

 * `OnTrigger(sum)` defines the entry point: the function under test.
 * `At(sum).Args(n).Inject()` maps inputs by position in the argument tuple.
 * `At(sum).Return().Expect()` defines the expected output to observe.


The `.Test(...)` clause defines list of test vectors using chained operator() calls.
Each column corresponds 1:1 with an input or output channel defined in the pipe configuration above.

In this case, the runner:

1. Takes the first two values as inputs to sum
2. Calls `sum(x, y)`
3. Checks whether the result matches the third value



The Test clause defines a list of test vectors using cascade of `operator()` calls,
where each line represents one test case.
The `At(sum)...` lines form a list of signal pipes, which can be viewed as headers
for the corresponding columns in the test matrix. The channel-to-column indexation
is 1-to-1 for pipes with empty `Inject()` or `Expect()` clauses;

For each test case, the runner invokes `sum` with the first two values as inputs
and checks whether the result matches the third value.

The same test can be expressed with a single channel for both input arguments:
```c++
*/
BOOST_AUTO_TEST_CASE(NonScalarValues)
{
    auto sum = [](double x, double y){ return x + y; };

    SignalMapping("Simple test with non-scalar input")
    .OnTrigger(sum)
        .At(sum) .Inject()
        .At(sum) .Expect()
    .Test
        ( { 2,   2},    4 )
        ( { 2,  -2},    0 )
        ( {42, 0.1}, 42.1 )
    ;
}
/*
```

Following the default configuration rules, `.At(sum).Inject()` is equivalent to `.At(sum).Args("").Inject()`
for non-unary triggers. This means the entire argument tuple is treated as a JSON array and passed as a single value.

## Basic Concepts

### Structure Overview

**Signal Mapping** tests are defined declaratively using a **Domain-Specific Language (DSL)** embedded in C++.
Each test case specifies how the system under test (SUT) is exercised and observed using the following elements:

* [**Clauses**](../dsl-reference/signal-mapping.md#clauses) — chained method calls that define test configuration.
* **Trigger** — the interface that initiates test execution.
* **Test Conditions** — input generators and output matchers.
* **Channels** — individual signal selectors associated with interface.
* **Condition Pipes** — combinations of channels that share a test condition.


### Pipes and Channels

* A **channel** represents a specific interface point (e.g., function input, return, or exception) with optional filters.
* A **pipe** is a group of one or more channels that share the same condition.
* Channels can be grouped into pipes using `.Group()` or `.Blend()` to express different composition logic (see below).

```c++
// One-channel pipe
.At(f).Args(0).Inject(42);

// Multi-channel pipe
.At(x).Args(0).Group().At(y).Args(0).Inject(42);
```

### Test Conditions


An **input condition** is a **generating function** — a mapping from the call index to a value.
An **output condition** is a **matcher** — a predicate that maps an observed value to a boolean result.

Literals, constants, and constant expressions are interpreted differently depending on context:

* **Input**: constants are treated as a subset of generators that always return the same value. See [Input generators](#input-generators).
* **Output**: constants are implicitly converted to equality matchers (`Eq(value)`).

The embedded [expression language](#expressions) supports constructing conditions of arbitrary complexity in both input and output contexts.

Test conditions can be specified:

* **Inline**, directly on pipes using `.Inject` and `.Expect*` or `.Assert*` clause parameters, or
* **Externally**, using the `.Test` clause to provide tabular test vectors.

#### 1. **Inline Conditions**

Attach expressions directly to the pipe using `.Inject` or `.Expect`.

```c++
*/
BOOST_AUTO_TEST_CASE(InlineConditions)
{
    auto sum = [](double x, double y){ return x + y; };

    SignalMapping("Inline condition example")
    .OnTrigger(sum).Repeat(100) //(1)
        .At(sum).Args(0).Inject(42) //(2)
        .At(sum).Args(1).Inject(Add(1)) //(3)
        .At(sum).Return().Expect(Each(Ge(42))) //(4)
        .At(sum).Return().Expect(Slide(2) | Each(Lt)) //(5)
    ;
}
/*
```
1. Repeat sum 100 times and apply matchers to an array of collected outputs instead of an atomic value
2. The first argument is fixed to 42.
3. The second input increments per call.
4. Check all outputs are >= 42
5. Check the output is increasing using sliding window matcher


#### 2. **Tabular Conditions**

Leave `.Inject()` or `.Expect()` empty, and use `.Test` to supply values:

```c++
*/
BOOST_AUTO_TEST_CASE(TabularConditions)
{
    auto sum = [](double x, double y){ return x + y; };

    SignalMapping("Tabular condition example")
    .OnTrigger(sum).Repeat(100)
        .At(sum).Args(0).Inject()
        .At(sum).Args(1).Inject()
        .At(sum).Return().Expect()
        .At(sum).Return().Expect()
    .Test
        ( 42, Add(1) , Each(Ge(42)), Slide(2) | Each(Lt) )
        ( 13, Mul(-1), Each(Le(13)), At(-1) | Lt(0)      )
    ;
}
/*
```

Each value in a test vector under `.Test` clause corresponds to one tabular pipe, by the order of declaration.


#### 3. **Mixed Mode**

Mix inline and tabular pipes in the same test:

```c++
*/
BOOST_AUTO_TEST_CASE(MixedConditions)
{
    auto sum = [](double x, double y){ return x + y; };

    SignalMapping("Mixed condition example")
    .OnTrigger(sum)
        .At(sum).Args(0).Inject(42)    // Fixed
        .At(sum).Args(1).Inject()      // Tabular
        .At(sum).Return().Expect()     // Tabular
    .Test
        (  1, 43 )
        ( -1, 41 )
    ;
}
/*
```

Here, `.Inject(42)` is constant across all test cases, while the second input and output vary per row.

### Test Procedure

The following procedure is applied to each model instance
obtained from the parameter resolver over each test vector:

1. Reset the environment (drop all input test data).
2. Execute pre-run tasks.
3. Inject the input generators into the environment.
4. Execute the trigger.
5. Observe the responses captured in the environment and verify them against expectations.
6. Execute post-run tasks.
7. Report the results.


## Advanced Topics

### Expressions

The framework provides an embedded functional programming language
that enables flexible matchers and more complex test data manipulation:

```c++
*/
BOOST_AUTO_TEST_CASE(ExpressionsExample)
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("Expressions example")
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        ( 42                , Eq(42)                             )
        ( 42                , 42     /*(1)*/                     )
        ( {1,2,3}           , Size|3 /*(2)*/                     ) ["Expect structure size equal 3"] //(3)
        ( {1,2,3}           , Saturate(1, Ne(1), Ge(2)) /*(4)*/  )
        ( {1,2,3,4}         , Fold(Add) & Size | Div | Eq(2.5)   ) //(5)
        ( Pi | Div(2) | Sin , Approx(1)                          ) //(6)
        ( Pi | ~Div(1)      , Approx(1.0/3.14159265359)          ) //(7)
        ( "5:1:-1" | Arange , {5,4,3,2}                          ) //(8)
    ;
}
/*
```

1. `Eq` may be omitted in the context of predicate.
2. Expression composition with pipe operator. Equivalent to `Pipe(Size, Eq(3))`.
3. Optional comment.
4. Saturate the matchers over sequence input in given order.
5. Ampersand operator packs evaluation into array as `f & g ↦ x ↦ [f(x), g(x)]`.
    Output is processed as `(((Reduce(Add) & Size) | Div) | Eq(2.5))`
    in the following steps:
    1. `Reduce(Add) & Size` on input `[1,2,3,4]` produces [10, 4]
    2. `Div` on input `[10, 4]` produces [2.5]
    3. `Eq(2.5)` on input `[2.5]` produces `true`
6. A constant function can be used on input channel instead of literal
7. Flip expression (prefix ~) swaps the operands, which is useful for non-commutative transforms
8. `Arange` is a generator similar to
    [numpy.arange](https://numpy.org/doc/stable/reference/generated/numpy.arange.html)


For more detailes, see [Expression Guide](../user-guide/expressions.md) and [Expression Language Reference](../dsl-reference/expressions.md).

### Input generators

Inputs in SignalMapping are not limited to fixed values - they can be
dynamic generators that produce a new value on each call, created from
expressions of type $\mathbb{N}^0 \mapsto JSON$, (aka [indexed families](https://en.wikipedia.org/wiki/Indexed_family)).

A constant input like `Inject(42)` is just a trivial generator that ignores the index.
Generators allows to define input streams that evolve over time, for example:

- `Add(1)` $\mapsto 1, 2, 3, ...$
- `Format("item_%d", Id)` $\mapsto$ `"item_0", "item_1", ...`

Internally, the framework keeps track on generator counters and allocation to channels.
On each interface call, either trigger or mock, the test runner pulls values from a set of
allocated generators and compose a returning value.
In `Blend` pipes, the test condition is a unique generator shared between all pipe channels,
See [Combining channels in pipes](#combining-channels-in-pipes) for details.


```c++
*/
BOOST_AUTO_TEST_CASE(InputsGenerators)
{
    auto const id = [](boost::json::value const& x) { return x; };

    SignalMapping("Input generators")
    .OnTrigger(id).Repeat(3)
        .At(id)             .Inject()
        .At(id)             .Expect()
    .Test
        (42/*(1)*/          , {42, 42, 42}         )
        (Pi/*(2)*/          , At(0)|Approx(3.14159))
        (Add(42)/*(3)*/     , {42, 43, 44}         )
        (Lookup({1,2})|D(42), {1,2,42}             )//(4)
    ;
}
/*
```

1. Top level JSON literal input stands for constant, a canonical form would be `C(42)`
2. Mathematical constant
3. Simple generating function
4. Lookup function with Default 42 as generator


#### Order matters

The input injections are evaluated in the order of their definition in a channel list,
with exception that all inline inuts evaluated before tabular inputs
(so it is recommended to group them at the beginning for clarity).

The injection may be overwritten fully or partially by the following injections when channels share
the same interface and selector.

The input evaluation order can be utilized to do some preparations before injecting more specific subsignal.
It is especially useful with dynamic structures like STL containers or variant types like boost::json::value itself.

E.g. having an interface with vector input,
```c++
auto sut = [](std::vector<int> const& x){ ... };
```
we may need to initialize a non-empty input before injecting to speciic elements:

```c++
    .At(sut) .Args("/0").Inject(0|Repeat(64))
    .At(sut) .Args("/0/1") // (1)
```

1. Injecting to the second element will fail without the previous line, as the default vector is empty.


### Output matchers

Output condition are **matchers** of type $JSON \mapsto bool$ (predicates).

In this context, any **constant expression** like `42` is interpreted as an equality check:
`Expect(42)` is equivalent to `Expect(Eq(42))`.


ZMBT provides several matcher clauses with varying behavior:

* `Expect`, `ExpectOne`, `ExpectBatch`
* `Assert`, `AssertOne`, `AssertBatch`

The difference between the `Expect*` and `Assert*` sets is in test control:

* `Expect*` records matcher failure but **allows the test to continue**.
* `Assert*` triggers an **immediate failure**, stopping execution.


#### Batch vs atomic sample

The `*Batch` clause variants provide captured sample series directly to the matcher.
The `*One` clause flattens the response if there is a single value (i.e., passes this sample rather then array),
or yields a failure otherwise.

The adaptive base clauses `Expect` and `Assert` switches to one of these behaviors
automatically based on test configuration and the number of observed values:

1. Tests with .Repeat(N) (where N > 1) or `Blend` pipes never flatten the response (i.e., same as `*Batch`)
2. Same for captures with more then one sample - they are passed to matcher as is
3. Otherwise, flatten if a single sample captured or yield a failure if none.


This implicit flattening makes tests ergonomic:

```c++
OnTrigger(f)
    .At(f).Expect(42);
```
vs
```c++
OnTrigger(f).Repeat(N)
    .At(f).Expect(Each(Eq(42)));
```


### JSON Serialization

ZMBT internally operates on JSON data, using Boost.JSON as the backend.
All injected inputs, observed outputs, and expressions are evaluated in terms of JSON values.
This means that **any type used in the model shall be serializable to and from JSON**.
Out of the box, it works for fundamental types and STL containers,
and any user-defined types can be supported with Boost.Describe library
or by specializing `zmbt::reflect::custom_serialization` template (see [Reflection guide](../user-guide/reflection/)).


#### JSON Pointer

ZMBT supports **signal path addressing** using [JSON Pointer](https://tools.ietf.org/html/rfc6901) strings.
This allows fine-grained control over which subsignals (fields or array elements) are injected or matched:

```cpp
.At(sut).Args("/0/foo/bar")  .Inject()
.At(sut).Args("/1/foo/bar")  .Inject()
.At(sut).Return("/foo/bar")  .Expect()
```

Each path refers to a subsignal within the arguments or return value. The pointer syntax resembles Unix-style paths, with key differences:

* `""` (empty string) means the root of the JSON structure.
* `"/"` addresses anonymous fields.
* `"/0"`, `"/1"` refer to elements in the argument tuple (index-based).

Recall the default behavior:

> *Unary function arguments resolve to path `"/0"`*, making scalar injection and observation intuitive.

#### Custom Types

ZMBT uses Boost.JSON with Boost.Describe to serialize and access custom C++ types:

```cpp
*/
namespace {
enum class Foo { A, B, C };

struct Bar {
    Foo foo;
    int x;
    Bar() = delete;
    Bar(Foo br, int x) : foo{br}, x{x} {}
};

BOOST_DESCRIBE_ENUM(Foo, A, B, C) //(1)
BOOST_DESCRIBE_STRUCT(Bar, (void), (foo, x)) //(2)
ZMBT_INJECT_JSON_TAG_INVOKE //(3)
}

ZMBT_DEFINE_CUSTOM_INIT(Bar, (Foo::A, 0)) //(4)
/*
```

1. **Boost.Describe** reflection for enum `Foo`.
2. **Boost.Describe** struct support for `Bar` (with no base class).
3. **JSON serialization support** via argument-dependent lookup.
4. **Default initialization** for custom types, required for signals with deleted default constructor.

Example usage:

```cpp
*/
BOOST_AUTO_TEST_CASE(UserDataTypes)
{
    auto sut = [](Bar a, Bar b) {
        Foo foo = a.x > b.x ? a.foo : b.foo;
        int x = a.x + b.x;
        return Bar {foo, x};
    };

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .At(sut).Args("/0/x")   .Inject()
        .At(sut).Args("/1/x")   .Inject()
        .At(sut).Args("/0/foo") .Inject()
        .At(sut).Args("/1/foo") .Inject()
        .At(sut).Return("/foo") .Expect()
    .Test
        (1, 0, Foo::A, Foo::B, Foo::A)
        (0, 1, Foo::A, Foo::B, Foo::B)
    ;
}
/*
```

This test validates logic involving structured inputs and enum matching via JSON field access.

#### Parametrized Paths

`Args` and `Return` accept [constant expressions](#expressions),
which are evaluated to a JSON pointer string or integer index.
This allows dynamic construction of paths based on parameters:

```cpp
.At(sut).Args("/%d/%s/bar" | Fmt(Index, Field))
// or printf-like overload:
.At(sut).Args("/%d/%s/bar", Index, Field)
```

`Index` and `Field` can be constants or literals, as well as [ZMBT model parameters](#parametrization)
which are resolved during test model instantiation.


#### Operator overloading

ZMBT serialization mechanism lacks type information, and Expressions by default operate on JSON values
following a most intuitive interpretation of their type, e.g. pair of real numbers is treated
as complex `[real, img]` in arithmetic operations.

`Overload` expression can be utilized to apply certain type-specific operators in expressions:

```cpp
Overload(type<Foo>, f)
```

The operator handler constructed from `type<Foo>` tag will provide to the enveloped `f`
expression all existing operators for this type. If specific type does not support certain operators,
they will be substituted with stubs that will fail in runtime without blocking compilation.


The `.As` clause that precedes pipe condition can be used to specify the default overload per
pipe to avoid repetition in `.Test` table. In addition to overloading, it also decorates the result
of non-boolean constant expressions by piping it to Cast expression, reducing the conversion
boilerplate in both inputs and outputs.

```c++
*/
BOOST_AUTO_TEST_CASE(DecorUnderlying)
{

    auto const id = [](int x) -> int { return x; };
    using decor::Underlying;

    SignalMapping("Decorate int -> int as Foo -> Foo")
    .OnTrigger(id)
        .At(id).As(Underlying<Foo>).Inject()
        .At(id).As(Underlying<Foo>).Expect()
    .Test
        (Foo::A              , Foo::A              )
        (Foo::B              , Foo::B              )
        (Foo::A              , Ne(Foo::B)          )
        (Foo::B              , Ne(Foo::C)          )
        (Foo::A | Add(Foo::B), Foo::B | Add(Foo::A))
    ;


    SignalMapping("Equivalent test with verbose casts and overloads")
    .OnTrigger(id)
        .At(id).Inject()
        .At(id).Expect()
    .Test
        (Foo::A | Cast(Underlying<Foo>), Cast(type<Foo>) | Eq(Foo::A))
        (Foo::B | Cast(Underlying<Foo>), Cast(type<Foo>) | Eq(Foo::B))
        (Foo::B | Cast(Underlying<Foo>), Cast(type<Foo>) | Eq(Foo::B))

        (Foo::A | Cast(Underlying<Foo>), Cast(type<Foo>) | Ne(Foo::B))
        (Foo::B | Cast(Underlying<Foo>), Cast(type<Foo>) | Ne(Foo::C))

        (
            Overload(Underlying<Foo>, Foo::A | Add(Foo::B)),
            Eq(Overload(Underlying<Foo>, Foo::B | Add(Foo::A)))
        )
    ;
}

/*
```

In this example we utilize a signal decorator `zmbt::decor::Underlying`,
that maps input Foo to int before passing to trigger function. Without this cast
the test will yield an error on attempt to cast string representation of Foo:A to integer,
or on trying to evaluate `Foo::A | Add(Foo::B)` as `Foo::A + Foo::B` rather then
`static_cast<std::underlying_t<Foo>(Foo::A) + static_cast<std::underlying_t<Foo>(Foo::B)`.

Note that in the verbose example the `Cast(Underlying<Foo>)` cannot be replaced with
`Cast(type<int>)`, as the conversion is done on serialized JSON values and not on C++ types.
The `Cast(type<Foo>)` in this context is equivalent to `Uncast(Underlying<Foo>)`.

Any user-defined type can be used as an argument to `Overload` and `Cast/Uncast` expressions
as `type<MyType>` tag. If default MyType JSON serialization is not sufficient for all test conversions,
it is possible to customize it by creating a decorator class with `decorated_type` member type,
that should provide non-explicit constructor and conversion operator for `decorated_type`.


### Mocks

So far, we’ve used the same function reference for `OnTrigger` and `At` clauses.
The reason each clause accepts a function reference is that you can also attach to mock interfaces,
processing side effects the same way as trigger arguments and return values:

```c++
*/
BOOST_AUTO_TEST_CASE(SingletoneMock)
{
    struct Mock {
        bool set_values(int& x_out, int& y_out) {
            return InterfaceRecord(&Mock::set_values) // (1)
                .Hook(x_out, y_out);  // (2)
        }
    };

    struct Sut {
        Mock mock {}; // (3)
        int operator()()
        {
            int x, y;
            bool ok = mock.set_values(x, y);
            return ok ? (x + y) : 0;
        }
    } sut {}; // (4)

    SignalMapping("Test with mock")
    .OnTrigger(sut)
        .At(&Mock::set_values) .Return() .Inject() // (5)
        .At(&Mock::set_values) .Args(0)  .Inject() // (6)
        .At(&Mock::set_values) .Args(1)  .Inject()
        .At(sut)               .Return() .Expect()
    .Test
        (true , 2,  2, 4)
        (true , 2, -2, 0)
        (false, 2,  2, 0) ["values discarded when set_values returns false"]
    ;
}
/*
```

1. Access the `Environment` record associated with this interface
2. Reroute call arguments to the `Environment`
3. A common way to inject mock into the sut is to separate mock interface and implementation,
    and link the mocked implementation to the test executable.
    For brevity we keep it simple in the present guide.
    When using dependency injection, you may also utilize inheritance-based
    mocks, as it is done in GMock.
4. In this example the trigger is a functor - an object with a single `operator()` overload.
5. `Return()` clause observes the mock’s return value.
    This can be omitted - it’s the default expansion for `At(mock)` channels.
6. `Args(index)` clause gives access to individual mock arguments by index.
    `At(mock).Args(...).Inject()` will handle the mutable references as outputs,
    but their incoming values are also accessible separately with `At(mock).Args(...).Expect()`.

In this example, the stimuli are injected into the mock object
rather than directly into the trigger.

The mocking mechanism uses `InterfaceRecord`, which reroutes calls to the `Environment`
- the central test harness state accessed by the test runner.
This API works uniformly for all callables, including templates, deducing arguments and return type
from callable signature.

By default, the mock behaves as a singleton — all Mock instances delegate to the same record.
If a singleton mock is not sufficient and you need to differentiate
between multiple mock instances, you can pass an additional reference object to the `InterfaceRecord`.
This allows the `Environment` to distinguish calls from separate instances:

```c++
*/
BOOST_AUTO_TEST_CASE(MultiInstanceMock)
{
    struct Mock {
        void set_value(int& x) const {
            return InterfaceRecord(&Mock::set_value, this) // (1)
                .Hook(x);
        }
    };

    struct ConsumeAndSubstract {
        Mock const& mock_x;
        Mock const& mock_y;

        ConsumeAndSubstract(Mock const& mx, Mock const& my) : mock_x{mx}, mock_y{my} {}

        int operator()() {
            int x, y;
            mock_x.set_value(x);
            mock_y.set_value(y);
            return x - y; //(2)
        }
    };

    Mock mock_x {};
    Mock mock_y {};
    ConsumeAndSubstract sut {mock_x, mock_y};

    SignalMapping("Test with multi-instance mock")
    .OnTrigger(sut)
        .At(&Mock::set_value, mock_x) .Args() .Inject()
        .At(&Mock::set_value, mock_y) .Args() .Inject()
        .At(sut) .Expect()
    .Test
        ( 2,  2,  0)
        ( 2, -2,  4)
        (-2,  2, -4)
    ;
}
/*
```

1. The `this` pointer is used as a reference object here, but it can be any
    `void*` pointer - it simply serves as an identifier to find the interface
    record in the `Environment`. The default reference object is `nullptr`.
2. Using non-commutative op here to demonstrate the mock instance tracing.


Example of using string key instead of this pointer to reference the mock instance in test:

```c++
*/
BOOST_AUTO_TEST_CASE(MockReferencingByString)
{
    struct Mock {
        std::string const id_;

        Mock(std::string const id) : id_{id} {}

        void foo() {
            return InterfaceRecord(&Mock::foo, id_).Hook();
        }
    };

    auto SUT = []{
        Mock("lol").foo();
        Mock("kek").foo();
    };

    SignalMapping("Test interface associated with string")
    .OnTrigger(SUT)
        .At(&Mock::foo, "lol").CallCount().Expect(1)
        .At(&Mock::foo, "kek").CallCount().Expect(1)
    ;
}
/*
```

### Parametrization

The previous examples used invariant test models with all components and condition values hardcoded.
The example below demonstrates how to define parametrized test models for greater flexibility and reusability:

```c++
*/
BOOST_AUTO_TEST_CASE(ZipParametrization)
{
    struct Mock {
        void log(std::string msg) {
            return InterfaceRecord(&Mock::log).Hook(msg);
        }
    };

    struct Sut {
        Mock mock {};

        void foo() { mock.log("Sut::foo"); };
        void bar() { mock.log("Sut::bar"); };
        void baz() { mock.log("Sut::baz"); };
    };

    auto sut = std::make_shared<Sut>();

    Param const Method  {"test interface"}; // (1)
    Param const Name {"interface name"};


    SignalMapping("Test with zip params on %s", Name /*(2)*/)
    .OnTrigger(Method, sut)
        .At(&Mock::log)
            .Expect( All(Contains(Name/*(3)*/), Contains("Sut")))
    .Zip // (4)
        (Name   , "Sut::foo", "Sut::bar", "Sut::baz") // (5)
        (Method , &Sut::foo , &Sut::bar , &Sut::baz ) // (6)

    .Description("Method: %s, Name: %s", Method, Name) // (7)
    ;
}
/*
```

1. `Param` is a placeholder object with string or int identifier.
    It must be unique within the test model. These identifiers are also
    used for diagnostics and reporting, so meaningful names are encouraged.
2. Strings can be parametrized as expressions, deferring the string formatting.
3. Parameters can be nested deep in the expressions.
4. Use `Zip` or `Pro` to generate multiple test instances with different parameter combinations.
5. Each parameter list follows the syntax: $(key, x_1, x_2, ..., x_n)$.
6. Any model element can be parametrized - including member function pointers.
7. An optional test description clause.


In this example, the `Zip` clause introduces model parametrization.
Like `Test`, it initiates a list of parameters in an `operator()`
cascade. Each row in this table begins with a parameter key (created with
`Param`), followed by corresponding values.

Before the model runner comes into play, the model resolver
transforms such a definition into a series of invariant instances,
resulting in three tests in this case.

Another feature in this example is a channel parameters deduction
- we can omit `Args(0)` and `Return()` clauses, as they are expanded
by model resolver from the interface by the following rule:

 - *on trigger*: inject to args and observe on return,
 - *on mock*: inject to return and observe on args,
 - *unary function args* resolves to value, the rest
     resolve to the arguments tuple (more on this later).

The `Zip` clause requires all parameter lists to be of equal length or
containing a single element which intended to be repeated with other parameters:

```cpp
.Zip
    (X, 1, 2, 3)
    (Y, 1, 2, 3)
    (Z, 4)
```
This clause yields `(1,1,4), (2,2,4), (3,3,4)`, but the following one will fail
at runtime due to inconsistent zip parameters count:
```cpp
.Zip
    (X, 1, 2, 3)
    (Y, 1, 2)
    (Z, 4)
```


You can also repeat the same parameter key in multiple rows to avoid super-wide tables
like this:

```cpp
.Zip
    (X, 1, 2, 3)
    (Y, 1, 2, 3)

    (X, 4, 5, 6)
    (Y, 4, 5, 6)
```

which is equivalent to

```cpp
.Zip
    (X, 1, 2, 3, 4, 5, 6)
    (Y, 1, 2, 3, 4, 5, 6)
```

The order of parameter keys doesn't matter, and you may notice that parameters
table oriented horizontally, i. e. *row -> parameter*, unlike the test matrix,
which is *column -> channel*.

Another parametrization clause is `Prod`, which stands for *Cartesian product*,
s.t.
```cpp
.Prod
    (X,  1,  2)
    (Y, 10, 20)
```
yields a set of 4 model instances with *X, Y* equals
`(1, 10), (1, 20), (2, 10), (20, 20)` correspondingly.


Parameter clauses can be used multiple times - each clause creates an independent group.
`Prod` and `Zip` clauses also can be chained

```cpp
.Prod
    (X,  1)
    (Y, 10, 20)
.Prod
    (X,  2)
    (Y, 30, 40)
.Zip
    (X,  3,  4)
    (Y, 50, 60)
```

Here each clause initiates a separate product or zip set, resulting in
```
(1, 10), (1, 20), (2, 30), (2, 40), (3, 50), (3, 60), (4, 50), (4, 60)
```

#### Registering non-serializable components

Any non-serializable model component may be registered in the `Environment` and
referenced by a string key. This is what actually happens under the hood when we
placed mfp literals in zip parameters in one of the examples above:

```cpp
.Zip
    (Method, &Sut::foo , &Sut::bar , &Sut::baz )
    (Name  , "Sut::foo", "Sut::bar", "Sut::baz")
```

We can do that registration explicitly:

```c++
*/
BOOST_AUTO_TEST_CASE(UsingRegistry)
{
    struct Mock {
        void log(std::string msg) {
            return InterfaceRecord(&Mock::log).Hook(msg);
        }
    };

    struct Sut {
        Mock mock {};

        void foo() { mock.log("Sut::foo"); };
        void bar() { mock.log("Sut::bar"); };
        void baz() { mock.log("Sut::baz"); };
    };

    auto sut = std::make_shared<Sut>();

    Environment env{};
    env.RegisterTrigger("Sut::foo", &Sut::foo, sut);
    env.RegisterTrigger("Sut::bar", &Sut::bar, sut);
    env.RegisterTrigger("Sut::baz", &Sut::baz, sut);
    env.RegisterInterface("Mock::log", &Mock::log);

    Param const Method {"interface method"};


    SignalMapping("Test with zip params on %s", Method)
    .OnTrigger(Method)
        .At("Mock::log")
            .Expect(Eq(Method))
    .Zip(Method, "Sut::foo", "Sut::bar", "Sut::baz")
    ;
}
/*
```

In the same way tasks and operators may be registered and referenced by key.

Benefits from this approach are:

 - meaningful interface names in test report.
 - enabled deferred string formatting for interfaces.
 - reuse string parameters in different roles (like `Method` in the example)


### Combining channels in pipes

When testing the relationship between different channels, it may be necessary to apply a single condition on them.
It can be made with clauses like `Group` or `Blend`, placed in place of terminal `Inject` or `Expect` clauses.
Channels separated by such clauses will be combined in a single *condition pipe*.

Input pipes will reuse generator, and output pipes will compose observed samples into a single structure
as following:

|        | Group                           | Blend                                         |
| ------ | ------------------------------- | --------------------------------------------- |
| Input  | Copy generator for each channel | Share single generator between channels       |
| Output | Pack samples into a tuple       | Merge samples into a series of `[id, sample]` |

For piped outputs, the test runner will combine the captured samples from the chained channels according to certain rules.
The result is passed then to the matcher expression specified at `Expect` clause argument or on the corresponding test matrix column.

Input `Group` pipe simply reduces repetition by reusing the same condition for different channels,
while keeping their evaluation independent.


Input `Blend` pipe will share the same generator instance, and the difference with `Group` will be
visible on non-constant generators.


The output `Blend` pipr merges captured samples in a time series, producing a list of pairs `[id, signal]`, sorted by timestamp.
Id field, if not defined explicitly with `Alias` clause, is a channel absolute index (ignoring the pipe boundaries).

In combination with `Saturate` or other custom matchers, the `Blend` output can be utilized for testing a strict or partial order on mock calls.

```c++
*/
BOOST_AUTO_TEST_CASE(TestMultichannelPipes)
{

    auto const repack = [](int a, int b, int c){
        return boost::json::object{
            {"a", a},
            {"b", b},
            {"c", c},
        };
    };

    SignalMapping("Test Group on input")
    .OnTrigger(repack).Repeat(16)
        .At(repack).Args(0).Group()
        .At(repack).Args(1).Group()
        .At(repack).Args(2).Inject(42)

        .At(repack).Return("/a").Group()
        .At(repack).Return("/b").Group()
        .At(repack).Return("/c").Expect(Each(42|Repeat(16)))
    ;


    SignalMapping("Test Blend on input")
    .OnTrigger(repack).Repeat(2)
        .At(repack).Args(0).Blend()
        .At(repack).Args(1).Blend()
        .At(repack).Args(2).Inject(Id)

        .At(repack).Return("/a").Group()
        .At(repack).Return("/b").Group()
        .At(repack).Return("/c").Expect({{0,3},{1,4},{2,5}})
    ;


    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    } mock;
    bool flip {};
    int count{};

    auto const SUT = [&](){
        if (flip = !flip) mock.foo(count++);
        else mock.bar(count++);
    };

    Param const N{"N"};
    Param const Match{"Match"};

    SignalMapping("Test Blend on output")
    .OnTrigger(SUT).Repeat(N)
        .At(SUT) .Inject()
        .At(&Mock::foo).Alias("f").Blend()
        .At(&Mock::bar).Alias("b").Expect(Match)
    .Zip
        (N,  2)
        (Match, Serialize | R"([["f",0],["b",1]])")
    .Zip
        (N, 42)
        (Match, Map(At(0)) | All(Count("f")|21, Count("b")|21))
        (Match, Map(At(1)) | Slide(2) | Map(Sub) | Count(1) | 41)
    ;
}
/*
```

As it is stated previously, the list of non-fixed pipes acts as a table header for the test matrix (1).
{ .annotate }

1. Defined with the `Test` clause


### Multithreading

ZMBT mocks implemented with `InterfaceRecord(...).Hook(...)` calls
are thread safe:

```c++
*/
BOOST_AUTO_TEST_CASE(TestMultithreading)
{
    struct Mock {
        int produce() const {
            return InterfaceRecord(&Mock::produce, this).Hook();
        }
        void consume(int const x) {
            return InterfaceRecord(&Mock::consume, this).Hook(x);
        }
    } producer, consumer;

    auto const task = [&](){
        while(true) {
            auto const item = producer.produce();
            if (item <= 0) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            consumer.consume(item);
        }
    };

    auto const SUT = [&](){
        constexpr int N {8};
        std::vector<std::thread> threads(N);
        for(int i = 0; i < N; ++i) threads.emplace_back(task);
        for(auto& thread: threads) {
            if (thread.joinable())  thread.join();
        }
    };


    SignalMapping("Test Blend on output")
    .OnTrigger(SUT)
        .At(&Mock::produce, producer).Inject(~Sub(42)) // xi = 42 - i
        .At(&Mock::consume, consumer).Take(Max).Expect(42)
        .At(&Mock::consume, consumer).CallCount().Expect(42)
        .At(&Mock::consume, consumer).ThreadId().Expect(Card|8)
    ;
}
/*
```


### Fixture tasks

The signal mapping models are tested in a presumably clean environment, which
mean that you can't affect the model execution from outside by injecting some
values to the mocks - the test runner will clean the environment test data
before each test case. In many cases we may need some preparation or cleanup
to be done outside the test, a typical task for the test fixture.
This can be done with `PreRun` and `PostRun` methods, that takes a list of tasks:

```c++
*/
BOOST_AUTO_TEST_CASE(FixtureTasks)
{
    struct Mock {
        int get_value() {
            return InterfaceRecord(&Mock::get_value).Hook();
        }
    };

    auto sut = []() { return Mock().get_value(); };

    SignalMapping("Test pre- and post-run tasks")
    .OnTrigger(sut)
        .At(&Mock::get_value).Inject()
        .At(sut)             .Expect()
    .Test
        (13    , 13)
        (Noop  , 42)
    .PreRun([]{
        InterfaceRecord(&Mock::get_value).InjectReturn(42);
    })
    .PostRun([]{
        BOOST_CHECK_EQUAL(InterfaceRecord(&Mock::get_value).ObservedCalls(), 1);
    })
    ;
}
/*
```

The pre-run task will reset the stimulus to 42 before each test.
On the first test case, this stimulus is overwritten by the value from the
test vector, but in the second case, `Noop` tells the runner to skip injection.

The order of `InjectReturn` or `InjectArgs` calls on `InterfaceRecord` has the same effect as order
of injections on channel clauses (see [Order matters](#order-matters)).
For such a such simple logic as in the given example it is recommended to use [inline conditions](#inline-conditions)
instead, but the tasks can be useful in handling side-effects not accessible in Expressions,
like SUT reset or extra logging.

### Diagnostic output

Consider the following example:
```cpp
*/
BOOST_AUTO_TEST_CASE(ExpressionDiagnostics, * utf::disabled())
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("SignalMapping test")
    .OnTrigger(id)
        .At(id).Inject("1:5"|Arange)
        .At(id).Expect(Reduce(Add) & Size | Div | Eq(2.5) | Not) //(1)
    ;
}
/*
```

1. Naive average computation - use Avg instead for real tasks.

Negation at the matcher end lead to test failure, and the log message is following:

```yaml
  - ZMBT FAIL:
      model: "SignalMapping test"
      message: "expectation match failed"
      expected: (Fold(Add) & Size) | Div | Eq(2.5E0) | Not
      observed: [1,2,3,4]
      condition: {"pipe":1}
      expression eval stack: |-
        ---
                 ┌── Add $ [1,2] = 3
                 ├── Add $ [3,3] = 6
                 ├── Add $ [6,4] = 10
              ┌── Fold(Add) $ [1,2,3,4] = 10
              ├── Size $ [1,2,3,4] = 4
           ┌── Fold(Add) & Size $ [1,2,3,4] = [10,4]
           ├── Div $ [10,4] = 2.5E0
           ├── Eq(2.5E0) $ 2.5E0 = true
           ├── Not $ true = false
        □  (Fold(Add) & Size) | Div | Eq(2.5E0) | Not $ [1,2,3,4] = false
```

To enable pretty-printing for JSON items, pass `--zmbt_log_prettify` command line argument.

*/
