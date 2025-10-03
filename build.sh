#!/usr/bin/env bash

set -e

BUILD_DIR="build"

run=0

for arg in "$@"; do
    case $arg in
        --run)
            run=1
            ;;
        *)
            ;;
    esac
done

mkdir -p "$BUILD_DIR"

cmake -B "$BUILD_DIR" -S . \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build "$BUILD_DIR" -j$(nproc)


if [ $run == 1 ]; then
    ./"$BUILD_DIR/QtFiles"
fi
