#!/usr/bin/env bash

set -euo pipefail

PROJECT_ROOT="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
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
        --help | *)
            echo "build.sh [--clean] [--test] [--run]"
            exit 0
            ;;
    esac
done

if [[ "$clean" -eq 1 ]] && [[ -e "$BUILD_DIR" ]]; then
    rm -rf "$BUILD_DIR"
    rm -rf "$PROJECT_ROOT/.cache"
fi

mkdir -p "$BUILD_DIR"

cmake -G "Unix Makefiles" -B "$BUILD_DIR" -S "$PROJECT_ROOT" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR" -j"$(nproc)"

if [ "$test" -eq 1 ]; then
    ctest --test-dir build --output-on-failure

    # cleanup after tests
    [[ -e "$TESTS_DIR/test_dir" ]] && rm -rf "$TESTS_DIR/test_dir"
    [[ -e "$TESTS_DIR/modified_dir" ]] && rm -rf "$TESTS_DIR/modified_dir"
fi

if [[ "$run" -eq 1 ]]; then
    "$BUILD_DIR/QtFiles"
fi
