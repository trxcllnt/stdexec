#!/bin/bash

realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}
CURDIR=$(realpath $(dirname "$0"))
source ${CURDIR}/_test_common.sh

startTest "clang-dev compilation"

# Create a temporary directory to store results between runs
BUILDDIR="build/gh-checks/clang-dev/"
mkdir -p "${CURDIR}/../../${BUILDDIR}"

# Run docker with action-cxx-toolkit to check our code
docker run ${DOCKER_RUN_PARAMS} \
    -e INPUT_BUILDDIR="/github/workspace/${BUILDDIR}" \
    -e INPUT_MAKEFLAGS="-j$(nproc --ignore=2)" \
    -e INPUT_IGNORE_CONAN='true' \
    -e INPUT_CC='clang' \
    -e INPUT_CXXFLAGS='-stdlib=libc++' \
    -e INPUT_CHECKS='build test install' \
    -e INPUT_PREBUILD_COMMAND="apt update && apt install -y --no-install-recommends git" \
    ghcr.io/trxcllnt/action-cxx-toolkit:clangdev-ubuntu22.04
status=$?
printStatus $status
