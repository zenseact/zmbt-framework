# (c) Copyright 2025 Zenseact AB

ZMBT_COPTS = [
    "-std=c++14",
    "-O2",
    "-Werror",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
]

BOOST_LIBS = [
    "@boost.algorithm//:boost.algorithm",
    "@boost.callable_traits//:boost.callable_traits",
    "@boost.container_hash//:boost.container_hash",
    "@boost.describe//:boost.describe",
    "@boost.format//:boost.format",
    "@boost.filesystem//:boost.filesystem",
    "@boost.iostreams//:boost.iostreams",
    "@boost.json//:boost.json",
    "@boost.lockfree//:boost.lockfree",
    "@boost.mp11//:boost.mp11",
    "@boost.math//:boost.math",
    "@boost.optional//:boost.optional",
    "@boost.phoenix//:boost.phoenix",
    "@boost.system//:boost.system",
    "@boost.spirit//:boost.spirit",
    "@boost.type_index//:boost.type_index",
    "@boost.type_traits//:boost.type_traits",
    "@boost.program_options//:boost.program_options",
    "@boost.unordered//:boost.unordered",
    "@boost.utility//:boost.utility",
]
