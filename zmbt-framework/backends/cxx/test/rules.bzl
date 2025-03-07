# (c) Copyright 2024 Zenseact AB

def unit_test_group(lib, srcs):
    for src in srcs:
        native.cc_test(
            name = src.replace(".cpp", ""),
            srcs = [src],
            deps = [
                lib,
                "//zmbt-framework/backends/cxx:main_boost",
            ],
            copts = [
                "-std=c++14",
                # "-Wgnu-zero-variadic-macro-arguments",
                "-Werror",
                "-Wall",
                "-Wextra",
                "-Wpedantic",
            ],
            linkopts = [
                '-lm'
            ],
            env = {
            },
            args = [
                "--color_output=no",
            ],
        )
