# (c) Copyright 2024 Zenseact AB

def make_unit_tests(srcs):
    for src in srcs:
        native.cc_test(
            name = src.replace(".cpp", ""),
            srcs = [src],
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
            ],
            linkopts = [
                '-lm'
            ],
            env = {
            },
            args = [
                "--color_output=no -- --zmbt_seed 42",
            ],
        )
