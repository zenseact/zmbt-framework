# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

workspace(name = "com_github_zenseact_zmbt_framework")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


##############################
# Bazel packaging
##############################

http_archive(
    name = "rules_pkg",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
        "https://github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
    ],
    sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
)
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
rules_pkg_dependencies()

##############################
# GCC toolchain
##############################

gcc_toolchain_version = "0.6.0"
gcc_toolchain_sha = "8aa7129247f06e12ab356797f79eea13f2387f32ae8f7184074e8e6402790299"

http_archive(
    name = "gcc_toolchain",
    urls = [
        "https://github.com/f0rmiga/gcc-toolchain/archive/refs/tags/{}.tar.gz".format(
            gcc_toolchain_version
        ),
    ],
    sha256 = gcc_toolchain_sha,
    strip_prefix = "gcc-toolchain-{}".format(gcc_toolchain_version),
)

##############################
# Boost C++ Libraries
##############################

rules_boost_sha256 = "6c15542d0d3b6f47cd4d2a1632a3054a0502b5102e471b4453354312cb474407"
rules_boost_version = "5d04542e52164931841d06d5a6b3fd2f43c436d0"
http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/{}.tar.gz".format(
        rules_boost_version,
    ),
    patch_args = ["-p1"],
    patches = [
        "//patches/rules_boost:0001-Fix-Wgnu-zero-variadic-macro-arguments-in-BOOST_DESC.patch"
    ],
    strip_prefix = "rules_boost-{}".format(rules_boost_version),
    sha256 = rules_boost_sha256,
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

##############################
# Google Test
##############################

http_archive(
  name = "googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
  sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)

##############################
# Bazel IWYU
##############################

storypku_bazel_iwyu_version = '0.20'
storypku_bazel_iwyu_sha = 'affd99c921590d3bd9032434fe5bdb5b8ceb2055fa04140f10c3bf4b07a1f684'

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_storypku_bazel_iwyu",
    strip_prefix = "bazel_iwyu-{}".format(storypku_bazel_iwyu_version),
    sha256 = storypku_bazel_iwyu_sha,
    urls = [
        "https://github.com/storypku/bazel_iwyu/archive/{}.tar.gz".format(storypku_bazel_iwyu_version),
    ],
)

load("@com_github_storypku_bazel_iwyu//bazel:dependencies.bzl", "bazel_iwyu_dependencies")
bazel_iwyu_dependencies()
