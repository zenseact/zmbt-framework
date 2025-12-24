
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")


LUA_RELEASE = "5.3.6"
SOL_RELEASE = "2.17.5" # the last version supporting C++14

LUA_BLD_FILE_CONTENT = """
cc_library(
    name = "lua_includes",
    visibility = ["//visibility:public"],
    hdrs = [
        "src/lauxlib.h",
        "src/lua.h",
        "src/luaconf.h",
        "src/lualib.h",
    ],
    includes = ["src"],
)

cc_library(
    name = "lua",
    visibility = ["//visibility:public"],
    srcs = glob(["src/l*"], exclude=["src/lua.c", "src/luac.c"]),
    hdrs = [
        "src/lauxlib.h",
        "src/lua.h",
        "src/luaconf.h",
        "src/lualib.h",
    ],
    copts = ["-w"],
    defines = ["LUA_USE_LINUX"],
    includes = ["src"],
    linkopts = [
        "-lm",
        "-ldl",
    ],
)
"""

SOL_BLD_FILE_CONTENT = """
cc_library(
    name = "sol",
    hdrs = ["single/sol/sol.hpp"],
    strip_include_prefix = "single",
    visibility = ["//visibility:public"],
)
"""


def _lua_and_sol_extension_impl(module_ctx):
    maybe(
        http_archive,
        name = "lua",
        build_file_content = LUA_BLD_FILE_CONTENT,
        sha256 = "fc5fd69bb8736323f026672b1b7235da613d7177e72558893a0bdcd320466d60",
        strip_prefix = "lua-{}".format(LUA_RELEASE),
        urls = [
            "https://www.lua.org/ftp/lua-{}.tar.gz".format(LUA_RELEASE),
        ],
    )

    maybe(
        http_archive,
        name = "sol",
        urls = ["https://github.com/ThePhD/sol2/archive/refs/tags/v{}.tar.gz".format(SOL_RELEASE)],
        sha256 = "1b017ca6228de606e5407d173cc552c86bd5b0309f38bf912c17e06943d1a0e4",
        strip_prefix = "sol2-{}".format(SOL_RELEASE),
        build_file_content = SOL_BLD_FILE_CONTENT,
    )


lua_and_sol = module_extension(
    implementation = _lua_and_sol_extension_impl,
)
