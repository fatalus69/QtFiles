#!/usr/bin/env bash

set -e

# === Configuration ===
BUILD_DIR="build"
MODULE_NAME="fileops"
PYTHON_GUI_DIR="gui"

mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "Running CMake configuration..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

cmake --build . || { echo "Build failed"; exit 1; }

OUTPUT_SO=$(find . -maxdepth 1 -name "${MODULE_NAME}*.so" | head -n 1)

if [ -z "$OUTPUT_SO" ]; then
    echo "Could not find compiled module (.so)"
    exit 1
fi

echo "Copying modules to $PYTHON_GUI_DIR/"
cp "$OUTPUT_SO" "../$PYTHON_GUI_DIR/" || { echo "Failed to copy .so file"; exit 1; }

cd ..

if [ "$1" == "run" ]; then
    python "$PYTHON_GUI_DIR/main.py"
fi
