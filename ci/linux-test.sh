#!/bin/bash

set -euox pipefail

readonly BAZEL_7_CONTAINER="gcr.io/bazel-public/bazel:7.4.1"
readonly BAZEL_LATEST_CONTAINER="gcr.io/bazel-public/bazel:latest"
ROOT_DIR="$(realpath $(dirname ${0})/..)"

# Test the Bazel 7.4.1 build with GCC
time docker run \
  -e USER="$(id -u)" \
  -u="$(id -u)" \
  -v="${ROOT_DIR}:/src" \
  -w="/src" \
  --rm \
  --env="CC=/usr/bin/gcc" \
  --env=CXXFLAGS="-Werror -Wdeprecated" \
  ${BAZEL_7_CONTAINER} \
  test //...
