#!/usr/bin/env bash

set -e

BUILD_DIR="build"

run=0
test=0
clean=0

for arg in "$@"; do
    case $arg in
        --clean)
            clean=1
            ;;
        --run)
            run=1
            ;;
        --test)
            test=1
            ;;
        *)
            ;;
    esac
done

if [ $clean == 1 ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

cmake -G "Unix Makefiles" -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR" -j$(nproc)

if [ $test == 1 ]; then
    # Make sure testing dirs dont exist (e.g. after previously failed tests).
    [ -e "./tests/test_dir" ] && rm -rf "./tests/test_dir"
    [ -e "./tests/modified_dir" ] && rm -rf "./tests/modified_dir"
    
    ctest --test-dir "$BUILD_DIR"
fi

if [ $run == 1 ]; then
    ./"$BUILD_DIR/QtFiles"
fi
