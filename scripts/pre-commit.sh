#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
cd "$PROJECT_ROOT"

if [ ! -d "$PROJECT_ROOT/build" ]; then
  bash "$PROJECT_ROOT/build.sh" > /dev/null 2>&1
fi

ctest --test-dir build --output-on-failure
