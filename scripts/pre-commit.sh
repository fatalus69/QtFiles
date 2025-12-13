#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
cd "$PROJECT_ROOT"

if [ ! -d "$PROJECT_ROOT/build" ]; then
  bash "$PROJECT_ROOT/build.sh" 
fi

ctest --test-dir build --output-on-failure
