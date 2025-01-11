<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

Reflection API
==============

*** note
*This section is in progress*
***

Namespace: `zmbt::reflect`

To operate on test data and interfaces in test model, the library requires the following reflection metadata/metafunctions:

- `zmbt::reflect::serialization` - signal type JSON serialization and deserialization.
- `zmbt::reflect::initialization` - default value for signal types with no default constructor.
- `zmbt::reflect::invocation` - interface invocation compile-time metadata and run-time apply method.

## Serialization
Default serialization reflection is covered by Boost.JSON for builtin types, and STL and Boost containers. To simplify serialization for the user-defined plain signals, we provide default serialization for enums and classes/structs, reflected with Boost.Describe lib as follows:

> *DEPRECATION WARNING*: The Boost.JSON supports described types since 1.81
> with some differences with ZMBT, this integration is a subject of change

```cpp
// data types defined elsewhere in SUT:
struct my_ns::Point {int x, int y};
enum class my_ns::Enum {A, B};

namespace my_ns {

// ADL injection macro
ZMBT_INJECT_JSON_TAG_INVOKE

BOOST_DESCRIBE_STRUCT(
    Point,  // typename
    (),     // list of base classes
    (x, y)   // member names
)

BOOST_DESCRIBE_ENUM(
    Enum, // typename
    A, B  // memvers
)

} // my_ns
```

Other Boost serialization libs may be supported in future. A more complex
serialization may be defined by user with `reflect::custom_seialization` (see below).

## Initialization
This metafunction provides initial signal value for models and Environment.
If your signal type has no default ctor, use reflect::custom_initialization (see below).

## Invocation
This metafunction provides type info about interface invocation details, and an apply method to be used in run-time, e. g., by model trigger.

Default invocation covers most of the use cases: free functions, functor objects with single operator(), member functions, and member data. Customization may be required in some edge cases, e.g. to handle unserializable signal types (see below).

## Reflection customisation
Templates:

 - `reflect::custom_seialization`
 - `reflect::custom_initialization`
 - `reflect::custom_invocation`

The library provides a uniform way to inject needed specializations into reflection metafunctions to customize their behavior:

Example:

```cpp
template <>
struct zmbt::reflect::custom_serialization<MySignal>
{

    static boost::json::value json_from(MySignal const& t)
    {
        // ...
    }

    static MySignal dejsonize(boost::json::value const& jv)
    {
        // ...
    }
}

template <>
struct zmbt::reflect::custom_initialization<MySignal>
{

    static constexpr MySignal init()
    {
        return MySignal { /* parameters for default test value */};
    }
}
// macros for simple cases:
ZMBT_DEFINE_CUSTOM_INIT(MySignal, (...))



template <>
struct zmbt::reflect::custom_invocation<void(*)(MySignal*)>
{
    using type     = void(*)(MySignal*);
    using host_t   = void;
    using return_t = void;
    using args_t   = std::tuple<MySignal&>;

    template <class Ignore>
    static return_t apply(Ignore, type ifc, args_t args)
    {
        MySignal& value_out  = std::get<0>(args);
        ifc(&value_out);
    }
}
```

The last example shows the transformation of the `(MySignal*) -> void` function
to `(MySignal&) -> void`. This allows environment and model APIs to handle the interface,
not supported by default because of pointers.

Then an environment interface hook may looks like

```cpp
void function_to_mock(MySignal* value_out)
{
    *value_out = zmbt::api::InterfaceRecord(function_to_mock).Hook();
}
```

If a straitforward specialization is not enough, all policies accept additional
SFINAE-enabling parameter, which can be used for more complex partitions, e. g.:

```cpp
template <class T>
struct zmbt::reflect::custom_serialization<T, std::enable_if_t<my_trait<T>::value>>
{
    // my implementation
};
```
