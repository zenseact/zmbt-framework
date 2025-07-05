#!/bin/bash

set -euox pipefail

readonly COMPILER=${1:-"clang-17"}
readonly DEV_CONTAINER="horolsky/zmbt-dev:20.04"
readonly ROOT_DIR="$(realpath $(dirname ${0}))"

mkdir -p /tmp/build_output/

docker run \
  -e USER="$(id -u)" \
  -v="${ROOT_DIR}:/src" \
  -v="/tmp/build_output:/tmp/build_output" \
  -w="/src" \
  --rm \
  --env="CC=/usr/bin/$COMPILER" \
  --env=CXXFLAGS="-Werror -Wdeprecated" \
  -it \
  --entrypoint=bash \
  ${DEV_CONTAINER}
