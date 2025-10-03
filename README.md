# QtFiles

**A lightweight, work-in-progress file explorer built with [Qt6](https://www.qt.io/product/qt6) and C++.**

### Requirements:
- `Qt6` installed on your system
- `C++ 20` or higher
- `CMake 3.14`

You’ll also need [`nlohmann/json`](https://github.com/nlohmann/json).
To fetch the single-header release:

````bash
wget -P ./backend https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp
````


### Build & Run

##### 1. Build
````bash
bash build.sh
````

##### 2. Run
````bash
./build/FileExplorer
````

#### Or alternatively to build and run
````bash
bash build.sh --run
````
