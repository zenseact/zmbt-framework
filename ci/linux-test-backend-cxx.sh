#!/bin/bash

set -euox pipefail

readonly COMPILER=${1:-"gcc"}
readonly TESTING_CONTAINER="horolsky/testing-zmbt:latest"
readonly ROOT_DIR="$(realpath $(dirname ${0})/..)"

echo "Testing ZMBT build with $COMPILER"

time docker run \
  -e USER="$(id -u)" \
  -u="$(id -u)" \
  -v="${ROOT_DIR}:/src" \
  -v="/tmp/build_output:/tmp/build_output" \
  -w="/src" \
  --rm \
  --env="CC=/usr/bin/$COMPILER" \
  --env=CXXFLAGS="-Werror -Wdeprecated" \
  ${TESTING_CONTAINER} \
  --output_user_root=/tmp/build_output \
  test //...
