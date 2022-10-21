#!/bin/bash

realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}
CURDIR=$(realpath $(dirname "$0"))
source ${CURDIR}/_test_common.sh

startTest "nvc++ compilation"

# Create a temporary directory to store results between runs
BUILDDIR="build/gh-checks/nvc++-22.7/"
mkdir -p "${CURDIR}/../../${BUILDDIR}"

# Run docker with action-cxx-toolkit to check our code
docker run ${DOCKER_RUN_PARAMS} \
    --runtime=nvidia \
    -e NVLOCALRC="/opt/nvidia/localrc" \
    -e INPUT_BUILDDIR="/github/workspace/${BUILDDIR}" \
    -e INPUT_MAKEFLAGS='-j 4' \
    -e INPUT_IGNORE_CONAN='true' \
    -e INPUT_CC='nvc++' \
    -e INPUT_CHECKS='build test install' \
    -e INPUT_PREBUILD_COMMAND='
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -;
apt update && apt install -y --no-install-recommends git cmake;
makelocalrc -d /opt/nvidia -x "$(dirname $(which nvc++))";' \
    ghcr.io/trxcllnt/action-cxx-toolkit:gcc11-cuda_multi-nvhpc22.7
status=$?
printStatus $status
