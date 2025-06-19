/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


/*

We'll start the examples by including the following dependencies:

```c++
*/
#include <boost/test/unit_test.hpp>
#include <zenseact-mbt.hpp>

using namespace zmbt::api;  //(1)
using namespace zmbt::expr; //(2)
/*
```

1. Main testing API namespace
2. Expression Language namespace


## Basic example

```c++
*/
BOOST_AUTO_TEST_CASE(BasicExample)
{
    auto sum = [](int x, int y){ return x + y; };

    SignalMapping("Simple function test")
    .OnTrigger(sum)
        .InjectTo  (sum) .Args(0)
        .InjectTo  (sum) .Args(1)
        .ObserveOn (sum) .Return()
    .Test
        ( 2,  2,  4 )
        ( 2, -2,  0 )
    .Description("inject x and y and check returning sum")
    ;
}
/*
```

This example defines a simple mapping with two inputs and one output.

 * `OnTrigger(sum)` specifies the trigger - an entry point of the test execution.
 * `InjectTo(sum).Args(n)` tells the model runner where to inject input values.
 * `ObserveOn(sum).Return()` specifies where to observe the function's return value.

The Test clause defines a list of test vectors using cascade of `operator()` calls,
where each line represents one test case.
The `InjectTo` and `ObserveOn` lines form a list of channels, which act as headers
for the corresponding columns in the test matrix. The channel-to-column indexation
is 1-to-1 for most cases; however, certain clauses, such as `Keep`, `Expect`, `Union`, and `With`,
will implicitly reduce the expected test vector size.

For each test case, the runner invokes `sum` with the first two values as inputs
and checks whether the result matches the third value.

The same test can be expressed with a single channel for both arguments:
```c++
*/
BOOST_AUTO_TEST_CASE(NonScalarValues)
{
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
}
/*
```

The default `InjectTo(sum)` expands to `InjectTo(sum).Args("")` for non-unary triggers.
This means the entire argument tuple is treated as a JSON array and passed as a single value.
More on the `Args` parameters later.

## Expressions

The framework provides an embedded functional scripting language
that enables flexible matchers and more complex test data manipulation:

```c++
*/
BOOST_AUTO_TEST_CASE(ExpressionsExample)
{
    auto id = [](boost::json::value const& x){ return x; };

    SignalMapping("Expressions example")
    .OnTrigger(id)
        .InjectTo  (id)
        .ObserveOn (id)
    .Test
        ( 42             , Eq(42)                             )
        ( 42             , 42     /*(1)*/                     )
        ( {1,2,3}        , Size|3 /*(2)*/                     ) ["Expect structure size equal 3"] //(3)
        ( {1,2,3}        , Saturate(1, Ne(1), Ge(2)) /*(4)*/  )
        ( {1,2,3,4}      , Reduce(Add) & Size | Div | Eq(2.5) ) //(5)
        ( Pi|Div(2)|Sin  , Approx(1)                          ) //(6)
        ( "5:1:-1"|Arange, {5,4,3,2}                          ) //(7)
    ;
}
/*
```

1. `Eq` may be omitted in this context (predicate is expected).
2. Expression composition with pipe operator. Equivalent to `Compose(Eq(3), Size)`.
3. Optional comment.
4. Saturate matchers in order over sequence input.
5. Ampersand operator packs evaluation into array as `f & g ↦ x ↦ [f(x), g(x)]`.
    Output is processed as `(((Reduce(Add) & Size) | Div) | Eq(2.5))`
    in the following steps:
    1. `Reduce(Add) & Size` on input `[1,2,3,4]` produces [10, 4]
    2. `Div` on input `[10, 4]` produces [2.5]
    3. `Eq(2.5)` on input `[2.5]` produces `true`
6. A constant function can be used on input channel instead of literal
7. `Arange` is a generator similar to
    [numpy.arange](https://numpy.org/doc/stable/reference/generated/numpy.arange.html)


For more detailes, see [Expression Guide](expressions.md) and [Expression Language Reference](../dsl-reference/expressions.md).


## Mocks

So far, we’ve used the same function reference for `OnTrigger`, `InjectTo`, and `ObserveOn` clauses.
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
        .InjectTo  (&Mock::set_values) .Return() // (5)
        .InjectTo  (&Mock::set_values) .Args(0)  // (6)
        .InjectTo  (&Mock::set_values) .Args(1)
        .ObserveOn (sut)               .Return()
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
    This can be omitted - it’s the default expansion for `InjectTo(mock)` channels.
6. `Args(index)` clause gives access to individual mock arguments by index.
    `InjectTo(mock).Args(...)` will handle the mutable references as outputs,
    but their incoming values are also accessible with `ObserveOn`.

In this example, the stimuli are injected into the mock object
rather than directly into the trigger.
The mock also returns a boolean flag to indicate success.
As before, we inject conditions with `.Args(0)` and `.Args(1)`, but this time the
interface is mock. We also control the mock’s return value as part of the test.

The mocking mechanism uses `InterfaceRecord`, which reroutes calls to the `Environment`
- the central test harness state accessed by the test runner.
This API works uniformly for all callables, including templates.

By default, the mock behaves as a singleton — all Mock instances delegate to the same record.
If a singleton mock is not sufficient and you need to differentiate
between multiple mock instances, you can pass an additional reference object to the `InterfaceRecord`.
This allows the `Environment` to distinguish calls from separate instances:

```c++
*/
BOOST_AUTO_TEST_CASE(MultipleMocks)
{
    struct Mock {
        void set_value(int& x) const {
            return InterfaceRecord(&Mock::set_value, this) // (1)
                .Hook(x);
        }
    };

    struct Sut {
        Mock const& mock_x;
        Mock const& mock_y;

        Sut(Mock const& mx, Mock const& my) : mock_x{mx}, mock_y{my} {}

        int operator()() {
            int x, y;
            mock_x.set_value(x);
            mock_y.set_value(y);
            return x + y;
        }
    };

    Mock mock_x {};
    Mock mock_y {};
    Sut sut {mock_x, mock_y};

    SignalMapping("Test with multiple mocks")
    .OnTrigger(sut)
        .InjectTo  (&Mock::set_value, mock_x) .Args()
        .InjectTo  (&Mock::set_value, mock_y) .Args()
        .ObserveOn (sut)
    .Test
        ( 2,  2, 4)
        ( 2, -2, 0)
    ;
}
/*
```

1. The `this` pointer is used as a reference object here, but it can be any
    `void*` pointer - it simply serves as an identifier to find the interface
    record in the `Environment`. The default reference object is `nullptr`.


```c++
*/
BOOST_AUTO_TEST_CASE(MockReferencingByString)
{
    struct Mock {
        Mock(std::string const id) : id_{id} {}

        void foo() {
            auto const ir = InterfaceRecord(&Mock::foo, id_);
            return InterfaceRecord(&Mock::foo, id_).Hook();
        }

        std::string const id_;
    };

    auto SUT = []{
        Mock("lol").foo();
        Mock("kek").foo();
    };

    SignalMapping("Test interface associated with string")
    .OnTrigger(SUT)
        .ObserveOn(&Mock::foo, "lol").CallCount()
        .ObserveOn(&Mock::foo, "kek").CallCount()
    .Test( 1, 1 )
    ;
}
/*
```

## Parametrization

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
        .ObserveOn (&Mock::log)

    .Test
        ( All(Contains(Name/*(3)*/), Contains("Sut")) )

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

## Serialization and JSON Pointer


Signal serialization allows specifying the particular subsignal with string
(we call it signal path), e. g.

```cpp
.InjectTo (sut).Args("/0/foo/bar")
.InjectTo (sut).Args("/1/foo/bar")
.ObserveOn(sut).Return("/foo/bar")
```

which refers to `<signal>.foo.bar` field on the corresponding argument or
return value. The subsignal specification in a string is a [JSON Pointer](https://tools.ietf.org/html/rfc6901),
which syntax resembles a unix path, with the main difference that `""` stands
for root, and `"/"` stands for anonymous field in a JSON structure. Array elements are
referenced by index, so `"/0"` and `"/1"` in the example means first and second
elements of the arguments tuple. Recall the default channel kind deduction rule -
now you see that *unary function args resolves to value...* means
the default signal path for unary function is `"/0"`.

This works with parametrization well - the `Args` and `Return` clauses
accepts constant expression that evaluates to JSON Pointer string or index integer.

If you need to parametrize only a part of signal path, use an expr::Fmt or a printf-like overload:

```cpp
.InjectTo (sut).Args("/%d/%s/bar"|Fmt(Index, Field))
// or
.InjectTo (sut).Args("/%d/%s/bar", Index, Field)
```

Considering the *Index* and *Field* are parameter keys, the signal path here is
evaluated to an actual string by the model resolver when the parameter values are known.
Parameters can be composed with other expressions, e.g. `Index|Add(1)` will be evaluated with +1 offset.

The example below shows how to use JSON serialization with custom types.
```c++
*/

namespace { // user data

enum class Foo { A, B, C };

struct Bar {
    Foo foo;
    int x;

    Bar() = delete;
    Bar(Foo br, int x) : foo{br}, x{x} {}
};

BOOST_DESCRIBE_ENUM(Foo, A, B, C)//(1)
BOOST_DESCRIBE_STRUCT(Bar, (void), (foo, x))//(2)

ZMBT_INJECT_JSON_TAG_INVOKE//(3)
}

ZMBT_DEFINE_CUSTOM_INIT(Bar, (Foo::A, 0))//(4)

BOOST_AUTO_TEST_CASE(UserDataTypes)
{
    auto sut = [](Bar a, Bar b) {
        Foo foo = a.x > b.x ? a.foo : b.foo;
        int x = a.x + b.x;
        return Bar {foo, x};
    };

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .InjectTo (sut).Args("/0/x")
        .InjectTo (sut).Args("/1/x")
        .InjectTo (sut).Args("/0/foo")
        .InjectTo (sut).Args("/1/foo")
        .ObserveOn(sut).Return("/foo")
    .Test
        (1, 0, Foo::A, Foo::B, Foo::A)
        (0, 1, Foo::A, Foo::B, Foo::B)
    ;
}
/*
```

1. The [Boost.Describe](https://www.boost.org/doc/libs/master/libs/describe/doc/html/describe.html) library enables type reflection.
   Must be placed in the same namespace as the type definition.
2. (void) token here tells that there are no parent classes for this type.
    It is a patched version to avoid empty varargs errors, the original API is just `()` for this case.
3. Enable argument-dependent lookup for JSON serialization. Must be placed in the same
   namespace as the type definition.
4. Custom initialization macro, useful for classes with no default ctor
    Must be placed in the global namespace.


## Registering non-serializable components

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
        .ObserveOn("Mock::log")
    .Test
        ( Eq(Method) )
    .Zip
        (Method, "Sut::foo", "Sut::bar", "Sut::baz")
    ;
}
/*
```

In the same way tasks and operators may be registered and referenced by key.

Benefits from this approach are:

 - meaningful interface names in test report.
 - enabled deferred string formatting for interfaces.
 - reuse string parameters in different roles (like `Method` in the example)


## Operator overloading

The predicate expressions described above know nothing about actual
operations - to apply them to a particular value, they need an operator handler.

By default they use generic JSON operators which evaluates most of operations in an intuitive
way, allowing order checks on strings and array-like structures.

Sometimes you need to apply certain type-specific operators - for such case use the Overload or Cast expressions:

```cpp
Overload(type<std::complex>, f)
```

The operator handler constructed from `type<std::complex>` will provide to the `f`
expression all existing operators for this type. If specific type does not support certain operators,
they will be substituted with stubs that will fail in runtime instead of blocking compilation.

Operator handler is propagated downstream to all terminal subexpressions.
If some of subexpressions returns type other than T in JSON representation, it may lead to undesired
behavior, e.g., boolean result from predicate will be casted to T if passed further:

```cpp
Overload(type<T>, Eq(x)) | Not // good
Overload(type<T>, Eq(x) | Not) // bad
```

## Type decoration

The `Decorate` (alias `Cast`) expression allows utilizing type decorators in place of actual signal types:

```c++
*/
BOOST_AUTO_TEST_CASE(DecorUnderlying)
{

    auto const id = [](int x) -> int { return x; };
    using decor::Underlying;

    SignalMapping("Decorate int -> int as Foo -> Foo")
    .OnTrigger(id)
        .InjectTo  (id) .As(Underlying<Foo>)
        .ObserveOn (id) .As(Underlying<Foo>)
    .Test
        (Foo::A, Foo::A    )
        (Foo::B, Foo::B    )
        (Foo::A, Eq(Foo::B)|Not)
        (Foo::B, Eq(Foo::C)|Not)
    ;
}

/*
```

In this example we utilize a signal decorator `zmbt::decor::Underlying`,
that maps input Foo to int before passing to trigger function. Without this cast
the model will throw exception on attempt to cast string representation of Foo:A to integer.


## Signal batch testing

Batch test conditions may be more convenient, expressive, and resource efficient
then atomic conditions like in examples before.

The SignalMapping model provides the following functionality for batch testing:

 - Generating input functions
 - `Repeat` clause for triggers.
 - `CallRange` and `Call` clauses for output filters.
 - Output combination clauses like `With` and `Union`.
 - `ThreadId`, `Timestamp`, `Alias` - clauses for additional information useful in batch tests.
 - `Keep` and `Expect` channels clauses for fixed expressions.

### Generating functions

In the previous examples the input conditions were specified as JSON literals,
which intuitively reads as an instruction for the test runner to set the input value
on a corresponding channel.

More generally the input signals are specified with [generating functions](https://en.wikipedia.org/wiki/Generating_function).
When an interface specified in the input channels is called multiple times in the test,
the intput conditions are evaluated from the corresponding expressions with 0-based call index as an argument.
The [constant](user-guide/expressions/#syntax) functions are considered here a subset of generating functions,
as they discard input argument.
The top-level JSON literals in the input context of SignalMapping are treated as constants for brevity.


```c++
*/
BOOST_AUTO_TEST_CASE(BatchInputs)
{
    struct Mock {
        double produce() {
            return InterfaceRecord(&Mock::produce).Hook();
        }
    } mock {};

    auto sut = [&mock](std::size_t count) {
        std::vector<double> result;
        for (size_t i = 0; i < count; i++)
        {
            result.push_back(mock.produce());
        }
        return result;
    };

    SignalMapping("Input expressions")
    .OnTrigger(sut)
        .InjectTo (sut)
        .InjectTo (&Mock::produce)
        .ObserveOn(sut)
    .Test
        (2, 42/*(1)*/            , {42, 42}             )
        (2, Pi/*(2)*/            , At(0)|Approx(3.14159))
        (3, Add(42)/*(3)*/       , {42, 43, 44}         )
        (4, Lookup({1,2,3})|D(42), {1,2,3,42}           )//(4)
    ;
}
/*
```

1. Top level JSON literal input stands for constant, a canonical form would be `C(42)`
2. Mathematical constant
3. Simple generating function
4. Lookup function with Default 42 as generator


Note that if same the interface is used with different subsignals specified with JSON Pointer at `Args` or `Return` clause
parameters, the corresponding injections expression are evaluated in order of appearance. See [more details below](#order-matters).

### Fixed channels

Both input and output channels can be excluded from the test matrix
by providing a fixed input generator or an expectation matcher directly
at channel definition with `Keep` and `Expect` clauses.

This techique allows to reduce the test matrix size, omitting invariant conditions,
or even use only the fixed channels, switching to a more functional style:

```c++
*/
BOOST_AUTO_TEST_CASE(FixedChannels)
{
    struct Mock {
        double produce() {
            return InterfaceRecord(&Mock::produce).Hook();
        }
    } mock {};

    auto sut = [&mock]() { return mock.produce(); };

    SignalMapping("Keep clause")
    .OnTrigger(sut).Repeat(250)
        .InjectTo (&Mock::produce)  .Keep(Add(1))
        .ObserveOn(sut).CallRange() .Expect(Slide(2) | Each(Lt))
    ;
}
/*
```



### CallRange and Repeat

In the previous example we used a dynamic number of mock calls as a test condition,
and the resulting vector was populated in the trigger.

We can have a batch test a simpler trigger, using `Repeat` clause for input and
`CallRange` on output.

`Repeat` is a unique clause following `OnTrigger`. When omitted, it's default value is 1.

The `CallRange` and `Call` clauses are mutually exclusive variant filters specific to output channels,
either trigger or mock. Default is `Call(-1)`, which stand for the latest captured call.

When `CallRange()` with no parameters specified, the output matcher will get an array of all
captured signals as an argument:

```c++
*/
BOOST_AUTO_TEST_CASE(CallRange)
{
    struct Mock {
        void consume(boost::json::value const& x) {
            return InterfaceRecord(&Mock::consume).Hook(x);
        }

        boost::json::value produce() {
            return InterfaceRecord(&Mock::produce).Hook();
        }
    } mock {};

    auto sut = [&mock]() {
        mock.consume(mock.produce());
    };


    SignalMapping("Repeat and CallRange clauses")
    .OnTrigger(sut).Repeat(3)
        .InjectTo  (&Mock::produce)
        .ObserveOn (&Mock::consume).CallRange()
    .Test
        ( Add(1)                               , {1, 2, 3}          )
        ( Recur(Add(1), 0)                     , {0, 1, 2}          )
        ( Lookup({{"1","foo"}})|D("bar")/*(1)*/, {"bar","foo","bar"})
    ;
/*
```

1. Input generator from lookup table - inject "foo" at second call, or "bar" by default.

Same example with parametric trigger repeats and more complex matchers:
```c++
*/
    Param const N {"N"};

    SignalMapping("Batch test with complex expressions for N=%d", N)
    .OnTrigger(sut).Repeat(N)
        .InjectTo  (&Mock::produce)
        .ObserveOn (&Mock::consume).CallRange()
    .Test
        (Add(1)       , (N | Div(2) | Add(0.5)) & Avg | Eq      /*(1)*/)
        (Flip(Sub(N)) , Slide(2) | Map(Sub|Eq(1)) | Reduce(And) /*(2)*/)
    .Zip
        (N, 3, 42, 100)
    .Description("Verify that avg(1,2,...,N,N+1) == N/2+0.5")
    ;
}
/*
```

1. Matcher expression:
    1. `N | Div(2) | Add(0.5)` produces N/2+0.5, ignoring input
    2. `Avg` computes average of the input `#!js [1,2,...,N,N+1]`
    3. `(N/2+0.5) & Avg | Eq` produces `true`
2. Matcher expression:
    1. `Slide(2)` over `#!js [N+1,N,...,2,1]`
        produces `#!js [[N+1,N],[N,N-1],...,[2,1]]`
    2. `Map(Sub|Eq(1))` produces `[true, true, ...]`
    3. `Reduce(And)` produces `true`


### Filtering specific calls

! TODO

`CallRange` supports slice semantic, s.t. `CallRange(-1, 0, -1)`
will provide reversed list of call data. Both `CallRange` and `Call` use
0-based indexation. The difference on observation channels is that `CallRange`
will return all captures that falls in the scpecified span or an empty array,
while `Call` will throw an exception if the call index is outside of the captures span.

The `Call` clause is a useful filter when only a specific output capture is interesting.

```c++
*/
BOOST_AUTO_TEST_CASE(Call)
{
    struct Mock {
        int get_value() {
            return InterfaceRecord(&Mock::get_value).Hook();
        }
    };

    auto sut = []() {
        Mock mock{};
        int x = mock.get_value();
        int y = mock.get_value();
        return x + y;
    };

    SignalMapping("Test Call")
    .OnTrigger(sut)
        .InjectTo (&Mock::get_value)
        .ObserveOn(sut)
    .Test
        (Lookup({2,  2}), 4)
        (Lookup({2, -2}), 0)
    ;
}
/*
```

### CallCount

`CallCount` channel specifier allows to test how many times the corresponding
mock interface was invoked from the SUT, efficiently replacing the combination of `CallRange()`
with `Size` expression:

```c++
*/
BOOST_AUTO_TEST_CASE(CallCount)
{
    struct Mock {
        Bar foo() {
            return InterfaceRecord(&Mock::foo).Hook();
        }
    };

    auto sut = []() {
        Mock mock{};
        mock.foo();
    };

    Param const N{"Trigger runs"};
    Param const X{"Call Count expectation"};

    SignalMapping("CallCount clause")
    .OnTrigger(sut).Repeat(N)
        .ObserveOn (&Mock::foo).CallCount()
    .Test(X)
    .Zip
        (N, 0   , 42)
        (X, Nil , 42)
    ;
}
/*
```

### With and Union

When testing the relationship between different channels, it may be necessary to put their outputs into a single column in the test matrix.
It can be made with clauses like `With` or `Union`, which begin an output channel definition similar to `ObserveOn`.
It is possible to chain multiple channels in a single combo, but mixing different combo types in a chain will not work.
The `ObserveOn` clause will break a chain, starting a new channel or combo.

The test runner will combine the captured outputs from the chained channels according to certain rules.
The result is passed then to the matcher expression from the corresponding test matrix column or the `Expect` clause argument.

The `Union` chain merges captured samples in a time series, producing a list of pairs `[alias, signal]`, sorted by timestamp.
Alias, if not defined explicitly with `Alias` clause, is a channel index (counting each channel in a chain separately).

In combination with `Saturate` or other custom matchers, the `Union` output can be used for testing a strict or partial order on mock calls.

```c++
*/
BOOST_AUTO_TEST_CASE(TestOrderUnion)
{
    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    };

    auto test = [](int count){
        Mock mock{};
        bool flip {};
        while (--count, count >=0)
        {
            if (flip = !flip) mock.foo(count);
            else mock.bar(count);
        }
    };

    SignalMapping("Test order with Union channel combo")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).CallRange().Alias("f")
            .Union (&Mock::bar).CallRange().Alias("b")

    .Test
        (  2, Serialize | R"([["f",1],["b",0]])"                 )
        ( 42, Map(At(0)) | All(Count("f")|21, Count("b")|21)     )
        ( 42, Map(At(1)) | Slide(2) | Map(Sub) | Count(1) | 41   )
    ;
}
/*
```

The `With` clause will simply pack the outputs of the combined channels into an
array.

```c++
*/
BOOST_AUTO_TEST_CASE(TestWithAutoArgs)
{
    struct Mock {
        void foo(int x) {
            return InterfaceRecord(&Mock::foo).Hook(x);
        }
        void bar(int x) {
            return InterfaceRecord(&Mock::bar).Hook(x);
        }
    };

    auto test = [](int reps, int x, int y){
        Mock mock{};
        for (int i = 0; i < reps; ++i)
        {
            mock.foo(x);
            mock.bar(y);
        }
    };

    SignalMapping("Test join with auto expr match on pair")
    .OnTrigger(test)
        .InjectTo  (test)
        .ObserveOn (&Mock::foo).CallRange()
             .With (&Mock::bar).CallRange()

    .Test
        ({1, 42, 13}, Flatten | Gt            )
        ({3, 42, 13}, {{42,42,42}, {13,13,13}})
        ({0, 42, 13}, Serialize | "[[],[]]"   )
    ;
}
/*
```

As it is stated previously, the channel list acts as a table header for the test matrix(1),
however, the fixing or combination clauses will break the 1-to-1 indexation.
It is important to keep this in mind when defining the test matrix or investigating the output reports.
{ .annotate }

1. Defined with the `Test` clause


## Order matters

The input injections are evaluated in the order of their definition in a channel list.
The injection may be overwritten fully or partially by the following injections when channels share the same interface.
However, all the fixed inputs(1) are evaluated before the non-fixed,
so it is recommended to group them at the beginning for clarity.
{ .annotate }

1. Defined with the `Keep` clause

The input evaluation order can be utilized to do some preparations before injecting at a specific subsignal.
It is especially useful with dynamic structures like STL containers or variant types like boost::json::value itself.

E.g. having an interface with vector input,
```c++
auto sut = [](std::vector<int> const& x){ ... };
```
we may need to initialize a non-empty input before injecting to speciic elements:

```c++
    .InjectTo (sut) .Args("/0").Keep(0|Repeat(64))
    .InjectTo (sut) .Args("/0/1") // (1)
```

1. Injecting to the second element will fail without the previous line, as the default vector is empty.

## Fixture tasks

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
        .InjectTo (&Mock::get_value)
        .ObserveOn(sut)
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
For such a such simple logic as in the given example it is recommended to use [fixed channels](#fixed-channels)
instead, but the tasks can be useful in handling side-effects not accessible in Expressions,
like SUT reset or extra logging.

## Diagnostic output

Consider the following example:
```cpp
*/
BOOST_AUTO_TEST_CASE(ExpressionDiagnostics)
{
auto id = [](boost::json::value const& x){ return x; };

SignalMapping("SignalMapping test")
.OnTrigger(id)
    .InjectTo  (id)
    .ObserveOn (id)
.Test
    ("1:5"|Arange, Reduce(Add) & Size | Div | Eq(2.5)) //(1)
;
}
/*
```

1. Naive average computation - use Avg instead for real tasks.

We can add negation at the matcher end as `| Not` to fail the test and check the log message:

```yaml
  - ZMBT FAIL:
      model: "SignalMapping test"
      message: "expectation match failed"
      expected: {":compose":[":not",{":eq":2.5E0},":div",{":fork":[{":reduce":":add"},":size"]}]}
      observed: [1,2,3,4]
      condition: [3,1]
      comment: "Computing average"
      test vector: [{":apply":[":arange","1:5"]},{":compose":[":not",{":eq":2.5E0},":div",{":fork":[{":reduce":":add"},":size"]}]}]
      expression eval stack: |-
        ---
                 ┌── ":add"([1,2]) = 3
                 ├── ":add"([3,3]) = 6
                 ├── ":add"([6,4]) = 10
              ┌── {":reduce":":add"}([1,2,3,4]) = 10
              ├── ":size"([1,2,3,4]) = 4
           ┌── {":fork":[{":reduce":":add"},":size"]}([1,2,3,4]) = [10,4]
           ├── ":div"([10,4]) = 2.5E0
           ├── {":eq":2.5E0}(2.5E0) = true
           ├── ":not"(true) = false
        □  {":compose":[":not",{":eq":2.5E0},":div",{":fork":[{":reduce":":add"},":size"]}]}([1,2,3,4]) = false
```

To enable pretty-printing for JSON items, pass `--zmbt_log_prettify` command line argument.

*/
