# QtFiles

**A lightweight, work‑in‑progress file explorer built with [Qt6](https://www.qt.io/product/qt6) and C++.**

## Requirements
- `Qt 6`
- `C++ 20` or newer
- `CMake` >= 3.14

## Quick start
On Debian like:
```bash
sudo apt install qt6-base-dev qt6-declarative-dev qml6-module-qtqml qml6-module-qtquick-controls qml6-module-qtquick
```
Add git hooks:
```bash
bash scripts/setup-git-hooks.sh
```

The script works the same for Windows - in case you have Qt6 installed.
So on Windows you simply run `build.ps1`
```bash
# build
bash build.sh

# build and run
bash build.sh --run

# run binary after build
./build/QtFiles

# run tests
bash build.sh --test # can be combined with the --run flag
./build/run_tests

# build clean, run test and run
bash build.sh --clean --test --run
```
