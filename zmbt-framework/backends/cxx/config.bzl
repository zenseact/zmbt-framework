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
    "@boost//:describe",
    "@boost//:format",
    "@boost//:filesystem",
    "@boost//:json",
    "@boost//:mp11",
    "@boost//:math",
    "@boost//:phoenix",
    "@boost//:system",
    "@boost//:spirit",
    "@boost//:type_index",
    "@boost//:type_traits",
    "@boost//:current_function",
    "@boost//:lockfree",
    "@boost//:program_options",
]
