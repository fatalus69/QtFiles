# QtFiles

**A lightweight, work‑in‑progress file explorer built with [Qt6](https://www.qt.io/product/qt6) and C++.**

## Requirements
- `Qt 6`
- `C++ 20` or newer
- `CMake` >= 3.14

## Quick start
```bash
# build
bash build.sh

# build and run
bash build.sh --run

# run binary after build
./build/QtFiles

# run tests
bash build.sh --test # can be combined with the --run flag
ctest --test-dir build
```