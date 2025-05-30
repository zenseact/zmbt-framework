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
    "@boost//:describe",
    "@boost//:format",
    "@boost//:json",
    "@boost//:log",
    "@boost//:mp11",
    "@boost//:math",
    "@boost//:phoenix",
    "@boost//:system",
    "@boost//:spirit",
    "@boost//:type_index",
    "@boost//:type_traits",
    "@boost//:current_function",
]
