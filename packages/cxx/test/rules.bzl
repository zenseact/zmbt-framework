# (c) Copyright 2024 Zenseact AB

def unit_test_group(lib, srcs):
    for src in srcs:
        native.cc_test(
            name = src.replace(".cpp", ""),
            srcs = [src],
            deps = [
                lib,
                "//packages/cxx:main_boost",
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
