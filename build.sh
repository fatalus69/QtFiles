#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
BUILD_DIR="$PROJECT_ROOT/build"
TESTS_DIR="$PROJECT_ROOT/tests"

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

if [ "$clean" -eq 1 ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

cmake -G "Unix Makefiles" -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR" -j$(nproc)

if [ "$test" -eq 1 ]; then
    # Make sure testing dirs dont exist (e.g. after previously failed tests).
    [ -e "$TESTS_DIR/test_dir" ] && rm -rf "$TESTS_DIR/test_dir"
    [ -e "$TESTS_DIR/modified_dir" ] && rm -rf "$TESTS_DIR/modified_dir"

    # prettier output than ctest
    ./"$BUILD_DIR"/run_tests
fi

if [ "$run" -eq 1 ]; then
    ./"$BUILD_DIR/QtFiles"
fi
