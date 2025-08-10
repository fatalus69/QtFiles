#!/usr/bin/env bash

set -e

BUILD_DIR="build"

mkdir -p "$BUILD_DIR"

cmake -B "$BUILD_DIR" -S . \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build "$BUILD_DIR" -j$(nproc)

if [ "$1" == "run" ]; then
    "$BUILD_DIR/FileExplorer"
fi
