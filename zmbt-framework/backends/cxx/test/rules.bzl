# (c) Copyright 2024 Zenseact AB

def make_unit_tests(srcs, tags = []):
    for src in srcs:
        native.cc_test(
            name = src.replace(".cpp", ""),
            srcs = [src],
            linkstatic=True,
            deps = [
                "//zmbt-framework/backends/cxx:zmbt",
                "//zmbt-framework/backends/cxx:zmbt_main_boost",
            ],
            copts = [
                "-std=c++14",
                "-Werror",
                "-Wall",
                "-Wextra",
                "-Wpedantic",
                "-Wno-parentheses", # allow `x & y | z` in tests
                "-Wno-gnu-zero-variadic-macro-arguments"
            ],
            linkopts = [
                '-lm'
            ],
            env = {
                "ZMBT_SEED": "42",
            },
            tags = tags,
        )
