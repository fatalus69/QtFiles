# QtFiles
A WIP lightweight file explorer built with PyQt and C++

### Prequistes:
- `Python 3.10`
- `C++ 17`
- `CMake 3.14`


### Usage
1. clone pybind11 into the backend directory
````bash
git clone https://github.com/pybind/pybind11.git backend/pybind11
 ````

2. Activate python venv and install dependencies
````bash
python3 -m venv venv
source venv/bin/activate
pip-sync # If you are using pip-tools
````

3. Compile C++ Code
````bash
bash build.sh
````

3. Run the UI
````bash
python gui/main.py
````