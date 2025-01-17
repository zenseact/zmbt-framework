# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

workspace(name = "com_github_zenseact_zmbt_framework")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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
    strip_prefix = "rules_boost-{}".format(rules_boost_version),
    sha256 = rules_boost_sha256,
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

##############################
# Google Test
##############################

http_archive(
  name = "com_google_googletest",
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
