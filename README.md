# QtFiles
A WIP lightweight file explorer built with Qt6 and C++

### Requirements:
- `Qt6` installed on your system
- `C++ 17` or higher
- `CMake 3.14`

Make sure to have nlohmann/json's json.hpp in the backend folder.
````bash
wget -P ./backend https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp
````

### Usage

3. Compile C++ Code
````bash
bash build.sh
````

3. Run the UI
````bash
./build/FileExplorer
````
