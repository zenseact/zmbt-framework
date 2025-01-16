<!-- (c) Copyright 2024 Zenseact AB -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Reflection API

Currently, ZMBT does not require complete type reflection. The `SignalMapping` tests rely solely on signal serialization and default value initialization. Future updates will ensure interoperability with Boost C++ libraries that provide data reflection capabilities.

To operate on test data and interfaces within the test model, the framework utilizes the following metafunctions in the `zmbt::reflect` namespace:

- `serialization<T>`: JSON serialization and deserialization for type `T`.
- `signal_traits<T>`: Default values for type `T`.
- `invocation<T>`: Type traits and a runtime `apply` method for callable type `T`.

For customization, specialize the metafunctions `zmbt::reflect::custom_*<T, E = void>`, as detailed below. The template parameter `E` serves as an optional SFINAE enabler.

## Serialization

Serialization is based on Boost.JSON, and it can be enabled by following the [Boost.JSON API](https://www.boost.org/doc/libs/release/libs/json/). However, the recommended approach is to use the metafunction `zmbt::reflect::custom_serialization<T, E = void>`. This enables serialization independently of Boost.JSON, preventing argument-dependent lookup pollution in the tested system (see the example in [demo_hermetic_serialization.cpp](../packages/cxx/test/demo_hermetic_serialization.cpp)). This is particularly useful when user data already has defined serialization that is unsuitable for testing or when template types in tests are not expected to be serializable in the SUT context.

```cpp
template <class T>
struct zmbt::reflect::custom_serialization<T, std::enable_if_t<my_trait<T>>> {
    // Convert T to JSON
    static boost::json::value json_from(const T& t);
    // Convert JSON to T
    static T dejsonize(const boost::json::value& v);
};
```

For simpler serialization, use the [Boost.Describe](https://www.boost.org/doc/libs/release/libs/describe/):

```cpp
namespace sut {
BOOST_DESCRIBE_STRUCT(Point, (), (x, y))
BOOST_DESCRIBE_ENUM(Enum, A, B)
} // namespace sut
```

Support for other Boost reflection libraries may be added in the future.

Note that `zmbt::reflect::serialization` is not intended for direct use. Instead, use `zmbt::json_from` and `zmbt::dejsonize`, which fall back to Boost.JSON when no default or custom conversions are defined.

To enable full interoperability with Boost.JSON for user data types, use the following macros to inject `tag_invoke` and `operator<<`:

- `ZMBT_INJECT_JSON_TAG_INVOKE`
- `ZMBT_INJECT_OSTREAM_OPERATOR`
- `ZMBT_INJECT_SERIALIZATION` (wraps both `tag_invoke` and `operator<<`)
- `ZMBT_INJECT_JSON_TAG_INVOKE_INTO(...)` - pass nested namespaces as list
- `ZMBT_INJECT_OSTREAM_OPERATOR_INTO(...)`
- `ZMBT_INJECT_SERIALIZATION_INTO(...)`

## Signal Traits

This metafunction currently provides initial signal values for models and the environment. Use `reflect::custom_signal_traits` to provide initial values for types without a default constructor or to specify custom default values for testing.

Future updates may extend this metafunction to include other traits, such as boundary values.

```cpp
template <>
struct zmbt::reflect::custom_signal_traits<MySignal> {
    static constexpr MySignal init() {
        return MySignal{/* parameters for default test value */};
    }
};

// Macros for simpler cases:
ZMBT_DEFINE_CUSTOM_INIT(MySignal, (...))
```

## Invocation

This metafunction provides type information about interface invocation details and a runtime `apply` method, which is useful for model triggers.

The default invocation covers most use cases: free functions, functor objects with a single `operator()`, and member functions. Customization may be necessary for edge cases, such as handling unserializable signal types.

```cpp
template <>
struct zmbt::reflect::custom_invocation<void(*)(MySignal*)> {
    using type     = void(*)(MySignal*);
    using host_t   = void;
    using return_t = void;
    using args_t   = std::tuple<MySignal&>;

    template <class Ignore>
    static return_t apply(Ignore, type ifc, args_t args) {
        MySignal& value_out = std::get<0>(args);
        return ifc(&value_out);
    }
};
```

This example transforms a function of type `(MySignal*) -> void` into `(MySignal&) -> void`, allowing the framework to handle interfaces that involve pointers, which are not supported by default.
