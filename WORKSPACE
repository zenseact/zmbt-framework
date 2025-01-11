# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

workspace(name = "com_github_zenseact_zmbt_framework")


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

rules_boost_sha256 = "6c15542d0d3b6f47cd4d2a1632a3054a0502b5102e471b4453354312cb474407"
rules_boost_version = "5d04542e52164931841d06d5a6b3fd2f43c436d0"
http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/{}.tar.gz".format(
        rules_boost_version,
    ),
    strip_prefix = "rules_boost-{}".format(rules_boost_version),
    sha256 = rules_boost_sha256,
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()


http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
  sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)



HERMETIC_CC_TOOLCHAIN_VERSION = "v3.1.0"

http_archive(
    name = "hermetic_cc_toolchain",
    sha256 = "df091afc25d73b0948ed371d3d61beef29447f690508e02bc24e7001ccc12d38",
    urls = [
        "https://mirror.bazel.build/github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
        "https://github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
    ],
)

load("@hermetic_cc_toolchain//toolchain:defs.bzl", zig_toolchains = "toolchains")

# Plain zig_toolchains() will pick reasonable defaults. See
# toolchain/defs.bzl:toolchains on how to change the Zig SDK version and
# download URL.
zig_toolchains()


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