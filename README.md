# QtFiles

**A lightweight, work‑in‑progress file explorer built with Qt6 and C++.**

## Requirements
- `Qt 6`
- `C++ 20` or newer
- `CMake` >= 3.14

### Dependencies
- `qt6-base-dev`
- `qt6-declarative-dev`

<hr>

## Quick start
```bash
# Install dependencies or do it manually
bash scripts/install_deps.sh
# Add git hooks
bash scripts/setup-git-hooks.sh
```

<br>

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
