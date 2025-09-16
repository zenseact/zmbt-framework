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
    "@boost//:algorithm",
    "@boost//:callable_traits",
    "@boost//:container_hash",
    "@boost//:current_function",
    "@boost//:describe",
    "@boost//:format",
    "@boost//:filesystem",
    "@boost//:json",
    "@boost//:lockfree",
    "@boost//:mp11",
    "@boost//:math",
    "@boost//:optional",
    "@boost//:phoenix",
    "@boost//:system",
    "@boost//:spirit",
    "@boost//:type_index",
    "@boost//:type_traits",
    "@boost//:program_options",
    "@boost//:unordered",
]
